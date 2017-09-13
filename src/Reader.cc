// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2017 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Reader.h"
#include "YODA/ReaderYODA.h"
#include "YODA/ReaderAIDA.h"
#include "YODA/ReaderFLAT.h"
#include "YODA/Config/DummyConfig.h"

using namespace std;

namespace YODA {


  Reader& mkReader(const string& name) {
    /// @todo Handle possible double-extensions better
    const size_t lastdot = name.find_last_of(".");
    const size_t lastbutonedot = (lastdot == string::npos) ? string::npos : name.find_last_of(".", lastdot-1);
    const string fmt = Utils::toLower((lastdot == string::npos) ? name : name.substr(lastdot+1));
    const string fmtex = Utils::toLower((lastbutonedot == string::npos) ? name : name.substr(lastbutonedot+1));
    // cout << "File extension: " << fmt << endl;
    if (fmt == "yoda") return ReaderYODA::create();
    if (fmt == "aida") return ReaderAIDA::create();
    if (fmt == "dat" || fmt == "flat")  return ReaderFLAT::create();
    if (fmtex == "yoda.gz") {
      #ifdef HAVE_LIBZ
      return ReaderYODA::create();
      #else
      throw UserError("YODA was compiled without zlib support");
      #endif
    }
    throw UserError("Format cannot be identified from string '" + name + "'");
  }


}
