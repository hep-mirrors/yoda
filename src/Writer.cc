// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2017 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Writer.h"
#include "YODA/WriterYODA.h"
#include "YODA/WriterAIDA.h"
#include "YODA/WriterFLAT.h"
#include "YODA/Config/BuildConfig.h"

#ifdef HAVE_LIBZ
#define _XOPEN_SOURCE 700
#include "zstr/src/zstr.hpp"
#endif

#include <iostream>
#include <typeinfo>
#include <sstream>
using namespace std;

namespace YODA {


  Writer& mkWriter(const string& name) {
    /// @todo There's got to be a better way of handling possible double-extensions...
    const size_t lastdot = name.find_last_of(".");
    const size_t lastbutonedot = (lastdot == string::npos) ? string::npos : name.find_last_of(".", lastdot-1);
    const string fmt = Utils::toLower((lastdot == string::npos) ? name : name.substr(lastdot+1));
    const string fmtex = Utils::toLower((lastbutonedot == string::npos) ? name : name.substr(lastbutonedot+1));

    // cout << "File extension: " << fmt << endl;
    #define FMTCHK(f) (fmt == f || Utils::startswith(fmtex, f))
    if (FMTCHK("yoda")) return WriterYODA::create();
    if (FMTCHK("aida")) return WriterAIDA::create();
    if (FMTCHK("dat") || FMTCHK("flat")) return WriterFLAT::create();
    #undef FMTCHK
    throw UserError("Format cannot be identified from string '" + name + "'");
  }


  void Writer::write(ostream& stream, const AnalysisObject& ao) {
    writeHeader(stream);
    writeBody(stream, ao);
    writeFooter(stream);
  }


  void Writer::write(const string& filename, const AnalysisObject& ao) {
    ostream* stream = nullptr;
    if (Utils::endswith(filename, ".gz")) {
      #ifdef HAVE_LIBZ
      stream = new zstr::ofstream(filename);
      #else
      throw WriteError("Not compiled with libz, so can't write compressed file " + filename);
      #endif
    } else {
      ofstream* ofs = new ofstream();
      ofs->open(filename.c_str());
      stream = ofs;
    }
    stream->exceptions(ifstream::failbit | ifstream::badbit);
    try {
      write(*stream, ao);
      //stream->close();
      delete stream;
    } catch(ifstream::failure e) {
      throw WriteError("Writing to filename " + filename + " failed: " + e.what());
    }
  }

  void Writer::writeBody(ostream& stream, const AnalysisObject* ao) {
    if (!ao) throw WriteError("Attempting to write a null AnalysisObject");
    writeBody(stream,*ao);
  }

  void Writer::writeBody(ostream& stream, const AnalysisObject& ao) {
    const string aotype = ao.type();
    if (aotype == "Counter") {
      writeCounter(stream, dynamic_cast<const Counter&>(ao));
    } else if (aotype == "Histo1D") {
      writeHisto1D(stream, dynamic_cast<const Histo1D&>(ao));
    } else if (aotype == "Histo2D") {
      writeHisto2D(stream, dynamic_cast<const Histo2D&>(ao));
    } else if (aotype == "Profile1D") {
      writeProfile1D(stream, dynamic_cast<const Profile1D&>(ao));
    } else if (aotype == "Profile2D") {
      writeProfile2D(stream, dynamic_cast<const Profile2D&>(ao));
    } else if (aotype == "Scatter1D") {
      writeScatter1D(stream, dynamic_cast<const Scatter1D&>(ao));
    } else if (aotype == "Scatter2D") {
      writeScatter2D(stream, dynamic_cast<const Scatter2D&>(ao));
    } else if (aotype == "Scatter3D") {
      writeScatter3D(stream, dynamic_cast<const Scatter3D&>(ao));
    } else if (aotype[0] == '_') {
      // Skip writing AO types with underscore prefixes (needed e.g. for Rivet wrappers)
      // maybe write a comment line in the output
    } else {
      ostringstream oss;
      oss << "Unrecognised analysis object type " << aotype << " in Writer::write";
      throw Exception(oss.str());
    }
  }


}
