// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2015 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/ReaderYODA2.h"
#include "YODA/Utils/StringUtils.h"
#include "YODA/Exceptions.h"

#include "YODA/Counter.h"
#include "YODA/Histo1D.h"
#include "YODA/Histo2D.h"
#include "YODA/Profile1D.h"
#include "YODA/Profile2D.h"
#include "YODA/Scatter1D.h"
#include "YODA/Scatter2D.h"
#include "YODA/Scatter3D.h"

#include <iostream>
using namespace std;

namespace YODA {


  namespace {

    /// @todo Move to Utils?
    // Portable version of getline taken from
    // http://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
    istream& safe_getline(istream& is, string& t) {
      t.clear();

      // The characters in the stream are read one-by-one using a std::streambuf.
      // That is faster than reading them one-by-one using the std::istream.
      // Code that uses streambuf this way must be guarded by a sentry object.
      // The sentry object performs various tasks,
      // such as thread synchronization and updating the stream state.
      istream::sentry se(is, true);
      streambuf* sb = is.rdbuf();

      for (;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
          return is;
        case '\r':
          if (sb->sgetc() == '\n')
            sb->sbumpc();
          return is;
        case EOF:
          // Also handle the case when the last line has no line ending
          if (t.empty())
            is.setstate(ios::eofbit);
          return is;
        default:
          t += (char)c;
        }
      }
    }

  }


  void ReaderYODA::read(istream& stream, vector<AnalysisObject*>& aos) {

    // Data format parsing states, representing current data type
    /// @todo Extension to e.g. "bar" or multi-counter or binned-value types, and new formats for extended Scatter types
    enum Context { NONE, //< outside any data block
                   SCATTER1D, SCATTER2D, SCATTER3D,
                   COUNTER,
                   HISTO1D, HISTO2D,
                   PROFILE1D, PROFILE2D };

    /// State of the parser: line number, line, parser context, and pointer(s) to the object currently being assembled
    unsigned int nline = 0;
    string s;
    Context context = NONE;
    //
    AnalysisObject* aocurr = NULL; //< Generic current AO pointer (useful or not?)
    Counter* cncurr = NULL;
    Histo1D* h1curr = NULL;
    Histo2D* h2curr = NULL;
    Profile1D* p1curr = NULL;
    Profile2D* p2curr = NULL;
    Scatter1D* s1curr = NULL;
    Scatter2D* s2curr = NULL;
    Scatter3D* s3curr = NULL;

    // Loop over all lines of the input file
    while (safe_getline(stream, s)) {
      nline += 1;

      /// @todo Trim the line
      // ...

      // Ignore blank lines
      if (s.empty()) continue;

      // Ignore comments (whole-line only, without indent, and still allowed for compatibility on BEGIN/END lines)
      if (s.find("#") == 0 && s.find("BEGIN") == string::npos && s.find("END") == string::npos) continue;


      // Now the context-sensitive part
      if (context == NONE) {

        // We require a BEGIN line to start a context
        if (s.find("BEGIN ") == string::npos) throw ReadError("Unexpected line in YODA format parsing when BEGIN expected");

        // Split into parts
        vector<string> parts;
        istringstream iss(s); string tmp;
        while (iss >> tmp) {
          if (tmp != "#") parts.push_back(tmp);
        }

        // Extract context from BEGIN type
        assert(parts.size() >= 2 && parts[0] == "BEGIN");
        const string ctxstr = parts[1];

        // Get block path if possible
        const string path = (parts.size() >= 3) ? parts[2] : "";

        // Set the new context and create a new AO to populate
        if (ctxstr == "YODA_COUNTER") {
          context = COUNTER;
          cncurr = new Counter(path);
          aocurr = cncurr;
        } else if (ctxstr == "YODA_SCATTER1D") {
          context = SCATTER1D;
          s1curr = new Scatter1D(path);
          aocurr = s1curr;
        } else if (ctxstr == "YODA_SCATTER2D") {
          context = SCATTER2D;
          s2curr = new Scatter2D(path);
          aocurr = s2curr;
        } else if (ctxstr == "YODA_SCATTER3D") {
          context = SCATTER2D;
          s3curr = new Scatter3D(path);
          aocurr = s3curr;
        } else if (ctxstr == "YODA_HISTO1D") {
          context = HISTO1D;
          h1curr = new Histo1D(path);
          aocurr = h1curr;
        } else if (ctxstr == "YODA_HISTO2D") {
          context = HISTO2D;
          h2curr = new Histo2D(path);
          aocurr = h2curr;
        } else if (ctxstr == "YODA_PROFILE1D") {
          context = PROFILE1D;
          p1curr = new Profile1D(path);
          aocurr = p1curr;
        } else if (ctxstr == "YODA_PROFILE2D") {
          context = PROFILE2D;
          p2curr = new Profile2D(path);
          aocurr = p2curr;
        }

      } else {

        /// @todo Throw error if a BEGIN line is found

        /// @todo Clear/reset context and register AO if END line is found, throw error if mismatch between BEGIN (context) and END types

        /// @todo Extract annotations for all types

        switch (context) {
        case SCATTER1D:
        case SCATTER2D:
        case SCATTER3D:
          cout << "Scatter " << aocurr->path() << " " << nline << " " << context << endl;
          break;
        case COUNTER:
          cout << "Counter " << aocurr->path() << " " << nline << " " << context << endl;
          break;
        case HISTO1D:
        case HISTO2D:
          cout << "Histo " << aocurr->path() << " " << nline << " " << context << endl;
          break;
        case PROFILE1D:
        case PROFILE2D:
          cout << "Profile " << aocurr->path() << " " << nline << " " << context << endl;
          break;
        default:
          throw ReadError("Unknown context in YODA format parsing: how did this happen?");
        }

        // case -1: // we left YODA_HISTO1D
        //   if (contextchange) {
        //     YODA::AnalysisObject* ao = new YODA::Histo1D(_histo1d.bins, _histo1d.dbn_tot, _histo1d.dbn_uflow, _histo1d.dbn_oflow);
        //     pair<string, string> pss;  // to make boost's BOOST_FOREACH happy
        //     BOOST_FOREACH (pss, _annotations)
        //       ao->setAnnotation(pss.first, pss.second);
        //     aos.push_back(ao);
        //     cleanup();
        //     contextchange = false;
        //   }
        //   break;
        // case -2: // we left YODA_HISTO2D
        //   if (contextchange) {
        //     /// @todo For now just create 8 fake entries: needs to be greatly generalised for final form
        //     _histo2d.dbns_oflow.clear(); _histo2d.dbns_oflow.resize(8);
        //     YODA::AnalysisObject* ao = new YODA::Histo2D(_histo2d.bins, _histo2d.dbn_tot, _histo2d.dbns_oflow);
        //     pair<string, string> pss;  // to make boost's BOOST_FOREACH happy
        //     BOOST_FOREACH (pss, _annotations)
        //       ao->setAnnotation(pss.first, pss.second);
        //     aos.push_back(ao);
        //     cleanup();
        //     contextchange = false;
        //   }
        //   break;
        // case -3: // we left YODA_PROFILE1D
        //   if (contextchange) {
        //     YODA::AnalysisObject* ao = new YODA::Profile1D(_profile1d.bins, _profile1d.dbn_tot, _profile1d.dbn_uflow, _profile1d.dbn_oflow);
        //     pair<string, string> pss;  // to make boost's BOOST_FOREACH happy
        //     BOOST_FOREACH (pss, _annotations)
        //       ao->setAnnotation(pss.first, pss.second);
        //     aos.push_back(ao);
        //     cleanup();
        //     contextchange = false;
        //   }
        //   break;
        // case -4: // we left YODA_PROFILE2D
        //   if (contextchange) {
        //     /// @todo For now just create 8 fake entries: needs to be greatly generalised for final form
        //     _profile2d.dbns_oflow.clear(); _profile2d.dbns_oflow.resize(8);
        //     YODA::AnalysisObject* ao = new YODA::Profile2D(_profile2d.bins, _profile2d.dbn_tot, _profile2d.dbns_oflow);
        //     pair<string, string> pss;  // to make boost's BOOST_FOREACH happy
        //     BOOST_FOREACH (pss, _annotations)
        //       ao->setAnnotation(pss.first, pss.second);
        //     aos.push_back(ao);
        //     cleanup();
        //     contextchange = false;
        //   }
        //   break;
        // case -5: // we left YODA_SCATTER1D
        //   if (contextchange) {
        //     YODA::AnalysisObject* ao = new YODA::Scatter1D(_scatter1d.points);
        //     pair<string, string> pss;  // to make boost's BOOST_FOREACH happy
        //     BOOST_FOREACH (pss, _annotations)
        //       ao->setAnnotation(pss.first, pss.second);
        //     aos.push_back(ao);
        //     cleanup();
        //     contextchange = false;
        //   }
        //   break;
        // case -6: // we left YODA_SCATTER2D
        //   if (contextchange) {
        //     YODA::AnalysisObject* ao = new YODA::Scatter2D(_scatter2d.points);
        //     pair<string, string> pss;  // to make boost's BOOST_FOREACH happy
        //     BOOST_FOREACH (pss, _annotations)
        //       ao->setAnnotation(pss.first, pss.second);
        //     aos.push_back(ao);
        //     cleanup();
        //     contextchange = false;
        //   }
        //   break;
        //   // case -7: // we left YODA_SCATTER3D
        //   /// @todo We need to iprove the parser to read Scatter3D, since it has the same number of line items as a profile type
        //   cerr << "YODA WARNING: Scatter3D can't currently be read from .yoda format. "
        //        << "This should be fixed soon: please complain to the authors!" << endl;
        // }

      }
    }

  }


}
