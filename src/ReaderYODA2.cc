// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2015 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/ReaderYODA2.h"
#include "YODA/Utils/StringUtils.h"
#include "YODA/Utils/getline.h"
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
    AnalysisObject* aocurr = NULL; //< Generic current AO pointer
    Counter* cncurr = NULL;
    Histo1D* h1curr = NULL;
    Histo2D* h2curr = NULL;
    Profile1D* p1curr = NULL;
    Profile2D* p2curr = NULL;
    Scatter1D* s1curr = NULL;
    Scatter2D* s2curr = NULL;
    Scatter3D* s3curr = NULL;

    // Loop over all lines of the input file
    while (Utils::getline(stream, s)) {
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
        // cout << aocurr->path() << " " << nline << " " << context << endl;

      } else {
        /// @todo Flatten conditional blocks with more else-ifs?

        // Throw error if a BEGIN line is found
        if (s.find("BEGIN ") != string::npos) throw ReadError("Unexpected BEGIN line in YODA format parsing before ending current BEGIN..END block");

        // Clear/reset context and register AO if END line is found
        /// @todo Throw error if mismatch between BEGIN (context) and END types
        if (s.find("END ") != string::npos) {
          aos.push_back(aocurr);
          context = NONE;
          aocurr = NULL; cncurr = NULL;
          h1curr = NULL; h2curr = NULL; p1curr = NULL; p2curr = NULL;
          s1curr = NULL; s2curr = NULL; s3curr = NULL;
          continue; ///< @todo Improve... would be good to avoid these continues
        }

        // Extract annotations for all types
        const size_t ieq = s.find("=");
        if (ieq != string::npos) {
          const string akey = s.substr(0, ieq);
          const string aval = s.substr(ieq+1);
          aocurr->setAnnotation(akey, aval);
          continue; ///< @todo Improve... would be good to avoid these continues
        }

        // Populate the data lines for points, bins, etc.
        // string xoflow1, xoflow2, yoflow1, yoflow2; double xmin(0), xmax(0), ymin(0), ymax(0);
        // double sumw(0), sumw2(0), sumwx(0), sumwx2(0), sumwy(0), sumwy2(0), sumwz(0), sumwz2(0), sumwxy(0), sumwxz(0), sumwyz(0); unsigned long n(0);
        // double x(0), y(0), z(0), exm(0), exp(0), eym(0), eyp(0), ezm(0), ezp(0);
        istringstream iss(s);
        switch (context) {

        case COUNTER:
          {
            double sumw(0), sumw2(0); unsigned long n(0);
            iss >> sumw2 >> sumw2 >> n;
            cncurr->setDbn(Dbn0D(n, sumw, sumw2));
          }
          break;

        case HISTO1D:
          {
            string xoflow1, xoflow2; double xmin(0), xmax(0);
            double sumw(0), sumw2(0), sumwx(0), sumwx2(0); unsigned long n(0);
            /// @todo Improve/factor this "bin" string-or-float parsing... esp for mixed case of 2D overflows
            /// @todo When outflows are treated as "infinity bins" and don't require a distinct type, string replace under/over -> -+inf
            if (s.find("Total") != string::npos || s.find("Underflow") != string::npos || s.find("Overflow") != string::npos) {
              iss >> xoflow1 >> xoflow2;
            } else {
              iss >> xmin >> xmax;
            }
            // The rest is the same for overflows and in-range bins
            iss >> sumw >> sumw2 >> sumwx >> sumwx2 >> n;
            const Dbn1D dbn(n, sumw, sumw2, sumwx, sumwx2);
            if (xoflow1 == "Total") h1curr->setTotalDbn(dbn);
            else if (xoflow1 == "Underflow") h1curr->setUnderflow(dbn);
            else if (xoflow1 == "Overflow")  h1curr->setOverflow(dbn);
            else h1curr->addBin(HistoBin1D(std::make_pair(xmin,xmax), dbn));
          }
          break;

        case HISTO2D:
          {
            string xoflow1, xoflow2, yoflow1, yoflow2; double xmin(0), xmax(0), ymin(0), ymax(0);
            double sumw(0), sumw2(0), sumwx(0), sumwx2(0), sumwy(0), sumwy2(0), sumwxy(0); unsigned long n(0);
            /// @todo Improve/factor this "bin" string-or-float parsing... esp for mixed case of 2D overflows
            /// @todo When outflows are treated as "infinity bins" and don't require a distinct type, string replace under/over -> -+inf
            if (s.find("Total") != string::npos) {
              iss >> xoflow1 >> xoflow2 >> yoflow1 >> yoflow2;
            } else if (s.find("Underflow") != string::npos || s.find("Overflow") != string::npos) {
              throw ReadError("2D histogram overflow syntax is not yet defined / handled");
            } else {
              iss >> xmin >> xmax >> ymin >> ymax;
            }
            // The rest is the same for overflows and in-range bins
            iss >> sumw >> sumw2 >> sumwx >> sumwx2 >> sumwy >> sumwy2 >> sumwxy >> n;
            /// @todo Make and fill bins/overflows
          }
          break;

        case PROFILE1D:
          {
            string xoflow1, xoflow2; double xmin(0), xmax(0);
            double sumw(0), sumw2(0), sumwx(0), sumwx2(0), sumwy(0), sumwy2(0); unsigned long n(0);
            /// @todo Improve/factor this "bin" string-or-float parsing... esp for mixed case of 2D overflows
            /// @todo When outflows are treated as "infinity bins" and don't require a distinct type, string replace under/over -> -+inf
            if (s.find("Total") != string::npos || s.find("Underflow") != string::npos || s.find("Overflow") != string::npos) {
              iss >> xoflow1 >> xoflow2;
            } else {
              iss >> xmin >> xmax;
            }
            // The rest is the same for overflows and in-range bins
            iss >> sumw >> sumw2 >> sumwx >> sumwx2 >> sumwy >> sumwy2 >> n;
            const double DUMMYWXY = 0;
            const Dbn2D dbn(n, sumw, sumw2, sumwx, sumwx2, sumwy, sumwy2, DUMMYWXY);
            if (xoflow1 == "Total") p1curr->setTotalDbn(dbn);
            else if (xoflow1 == "Underflow") p1curr->setUnderflow(dbn);
            else if (xoflow1 == "Overflow")  p1curr->setOverflow(dbn);
            else p1curr->addBin(ProfileBin1D(std::make_pair(xmin,xmax), dbn));
          }
          break;

        case PROFILE2D:
          {
            string xoflow1, xoflow2, yoflow1, yoflow2; double xmin(0), xmax(0), ymin(0), ymax(0);
            double sumw(0), sumw2(0), sumwx(0), sumwx2(0), sumwy(0), sumwy2(0), sumwz(0), sumwz2(0), sumwxy(0), sumwxz(0), sumwyz(0); unsigned long n(0);
            /// @todo Improve/factor this "bin" string-or-float parsing... esp for mixed case of 2D overflows
            /// @todo When outflows are treated as "infinity bins" and don't require a distinct type, string replace under/over -> -+inf
            if (s.find("Total") != string::npos) {
              iss >> xoflow1 >> xoflow2 >> yoflow1 >> yoflow2;
            } else if (s.find("Underflow") != string::npos || s.find("Overflow") != string::npos) {
              throw ReadError("2D profile overflow syntax is not yet defined / handled");
            } else {
              iss >> xmin >> xmax >> ymin >> ymax;
            }
            // The rest is the same for overflows and in-range bins
            iss >> sumw >> sumw2 >> sumwx >> sumwx2 >> sumwy >> sumwy2 >> sumwz >> sumwz2 >> sumwxy >> sumwxz >> sumwyz >> n;
            /// @todo Make and fill bins/overflows
          }
          break;

        case SCATTER1D:
          {
            double x(0), exm(0), exp(0);
            iss >> x >> exm >> exp;
            s1curr->addPoint(Point1D(x, exm, exp));
          }
          break;

        case SCATTER2D:
          {
            double x(0), y(0), exm(0), exp(0), eym(0), eyp(0);
            /// @todo Need to improve this format for multi-err points
            iss >> x >> exm >> exp >> y >> eym >> eyp;
            s2curr->addPoint(Point2D(x, y, exm, exp, eym, eyp));
          }
          break;

        case SCATTER3D:
          {
            double x(0), y(0), z(0), exm(0), exp(0), eym(0), eyp(0), ezm(0), ezp(0);
            /// @todo Need to improve this format for multi-err points
            iss >> x >> exm >> exp >> y >> eym >> eyp >> z >> ezm >> ezp;
            s3curr->addPoint(Point3D(x, y, z, exm, exp, eym, eyp, ezm, ezp));
          }
          break;

        default:
          throw ReadError("Unknown context in YODA format parsing: how did this happen?");
        }

        // cout << "AO CONTENT " << nline << endl;
        // cout << "  " << xmin << " " << xmax << " " << ymin << " " << ymax << " / '" << xoflow1 << "' '" << xoflow2 << "' '" << yoflow1 << "' '" << yoflow2 << "'" << endl;
        // cout << "  " << sumw << " " << sumw2 << " " << sumwx << " " << sumwx2 << " " << sumwy << " " << sumwy2 << " " << sumwz << " " << sumwz2 << " " << sumwxy << " " << sumwxz << " " << sumwyz << " " << n << endl;
        // cout << "  " << x << " " << y << " " << z << " " << exm << " " << exp << " " << eym << " " << eyp << " " << ezm << " " << ezp << endl;

      }
    }

  }


}
