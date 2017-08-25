// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2016 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/ReaderYODA.h"
#include "YODA/Utils/StringUtils.h"
#include "YODA/Utils/getline.h"
#include "YODA/Exceptions.h"
#include "YODA/Config/DummyConfig.h"

#include "YODA/Counter.h"
#include "YODA/Histo1D.h"
#include "YODA/Histo2D.h"
#include "YODA/Profile1D.h"
#include "YODA/Profile2D.h"
#include "YODA/Scatter1D.h"
#include "YODA/Scatter2D.h"
#include "YODA/Scatter3D.h"

#include "yaml-cpp/yaml.h"
#ifdef YAML_NAMESPACE
#define YAML YAML_NAMESPACE
#endif

#ifdef HAVE_LIBZ
#define _XOPEN_SOURCE 700
#include "zstr/src/zstr.hpp"
#endif

#include <iostream>
#include <cstring>
using namespace std;

namespace YODA {


  namespace {

    /// Fast ASCII tokenizer, extended from FastIStringStream by Gavin Salam.
    class aistringstream {
    public:
      // Constructor from char*
      aistringstream(const char* line=0) { reset(line); }
      // Constructor from std::string
      aistringstream(const string& line) { reset(line); }

      // Re-init to new line as char*
      void reset(const char* line=0) {
        _next = const_cast<char*>(line);
        _new_next = _next;
        _error = false;
      }
      // Re-init to new line as std::string
      void reset(const string& line) { reset(line.c_str()); }

      // Tokenizing stream operator (forwards to specialisations)
      template<class T>
      aistringstream& operator >> (T& value) {
        _get(value);
        if (_new_next == _next) _error = true; // handy error condition behaviour!
        _next = _new_next;
        return *this;
      }

      // Allow use of operator>> in a while loop
      operator bool() const { return !_error; }

    private:
      void _get(double& x) { x = std::strtod(_next, &_new_next); }
      void _get(float& x) { x = std::strtof(_next, &_new_next); }
      void _get(int& i) { i = std::strtol(_next, &_new_next, 10); } // force base 10!
      void _get(long& i) { i = std::strtol(_next, &_new_next, 10); } // force base 10!
      void _get(unsigned int& i) { i = std::strtoul(_next, &_new_next, 10); } // force base 10!
      void _get(long unsigned int& i) { i = std::strtoul(_next, &_new_next, 10); } // force base 10!
      void _get(string& x) {
        /// @todo If _next and _new_next become null?
        while (std::isspace(*_next)) _next += 1;
        _new_next = _next;
        while (!std::isspace(*_new_next)) _new_next += 1;
        x = string(_next, _new_next-_next);
      }

      char *_next, *_new_next;
      bool _error;
    };

  }


  void ReaderYODA::read(istream& _stream, vector<AnalysisObject*>& aos) {

    #ifdef HAVE_LIBZ
    // zstr detects if file is deflated or plain-text
    zstr::istream stream(_stream);
    #else
    auto& stream = _stream;
    #endif

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
    vector<HistoBin1D> h1binscurr; //< Current H1 bins container
    vector<HistoBin2D> h2binscurr; //< Current H2 bins container
    vector<ProfileBin1D> p1binscurr; //< Current P1 bins container
    vector<ProfileBin2D> p2binscurr; //< Current P2 bins container
    vector<Point1D> pt1scurr; //< Current Point1Ds container
    vector<Point2D> pt2scurr; //< Current Point2Ds container
    vector<Point3D> pt3scurr; //< Current Point3Ds container
    Counter* cncurr = NULL;
    Histo1D* h1curr = NULL;
    Histo2D* h2curr = NULL;
    Profile1D* p1curr = NULL;
    Profile2D* p2curr = NULL;
    Scatter1D* s1curr = NULL;
    Scatter2D* s2curr = NULL;
    Scatter3D* s3curr = NULL;
    string annscurr;

    // Loop over all lines of the input file
    aistringstream aiss;
    while (Utils::getline(stream, s)) {
      nline += 1;

      // Trim the line
      Utils::itrim(s);

      // Ignore blank lines
      if (s.empty()) continue;

      // Ignore comments (whole-line only, without indent, and still allowed for compatibility on BEGIN/END lines)
      if (s.find("#") == 0 && s.find("BEGIN") == string::npos && s.find("END") == string::npos) continue;


      // Now the context-sensitive part
      if (context == NONE) {

        // We require a BEGIN line to start a context
        if (s.find("BEGIN ") == string::npos) {
          stringstream ss;
          ss << "Unexpected line in YODA format parsing when BEGIN expected: '" << s << "' on line " << nline;
          throw ReadError(ss.str());
        }

        // Remove leading #s from the BEGIN line if necessary
        while (s.find("#") == 0) s = Utils::trim(s.substr(1));

        // Split into parts
        vector<string> parts;
        istringstream iss(s); string tmp;
        while (iss >> tmp) parts.push_back(tmp);

        // Extract context from BEGIN type
        if (parts.size() < 2 || parts[0] != "BEGIN") {
          stringstream ss;
          ss << "Unexpected BEGIN line structure when BEGIN expected: '" << s << "' on line " << nline;
          throw ReadError(ss.str());
        }

        // Second part is the context name
        const string ctxstr = parts[1];

        // Get block path if possible
        const string path = (parts.size() >= 3) ? parts[2] : "";

        // Get block format version if possible
        const string fmt = (parts.size() >= 4) ? parts[3] : "1";

        // Set the new context and create a new AO to populate
        /// @todo Use the block format version for (occasional, careful) format evolution
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
          context = SCATTER3D;
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
        if (s.find("BEGIN ") != string::npos)
          throw ReadError("Unexpected BEGIN line in YODA format parsing before ending current BEGIN..END block");

        // Clear/reset context and register AO if END line is found
        /// @todo Throw error if mismatch between BEGIN (context) and END types
        if (s.find("END ") != string::npos) {
          switch (context) {
          case COUNTER:
            break;
          case HISTO1D:
            h1curr->addBins(h1binscurr);
            h1binscurr.clear();
            break;
          case HISTO2D:
            h2curr->addBins(h2binscurr);
            h2binscurr.clear();
            break;
          case PROFILE1D:
            p1curr->addBins(p1binscurr);
            p1binscurr.clear();
            break;
          case PROFILE2D:
            p2curr->addBins(p2binscurr);
            p2binscurr.clear();
            break;
          case SCATTER1D:
            s1curr->addPoints(pt1scurr);
            pt1scurr.clear();
            break;
          case SCATTER2D:
            s2curr->addPoints(pt2scurr);
            pt2scurr.clear();
            break;
          case SCATTER3D:
            s3curr->addPoints(pt3scurr);
            pt3scurr.clear();
            break;
          case NONE:
            break;
          }
          // Set all annotations
          // YAML::Node anns = YAML::Load(annscurr);
          istringstream iss(annscurr);
          YAML::Parser parser(iss);
          YAML::Node anns;
          parser.GetNextDocument(anns);
          for (YAML::Iterator it = anns.begin(); it != anns.end(); ++it) {
            string key, val;
            it.first() >> key;
            YAML::Emitter em;
            em << it.second();
            val = em.c_str();
            // try {
            //   // Assume the value is a scalar type -- it'll throw an exception if not
            //   it.second() >> val;
            // } catch (const YAML::InvalidScalar& ex) {
            //   cout << "OOPS" << endl;
            //   // It's a list: process the entries individually into a comma-separated string
            //   string subval;
            //   for (size_t i = 0; i < it.second().size(); ++i) {
            //     it.second()[i] >> subval;
            //     val += subval + ((i < it.second().size()-1) ? "," : "");
            //   }
            // }
            // cout << "@@@ '" << key << "', '" << val << "'" << endl;
            aocurr->setAnnotation(key, val);
          }
          // Put this AO in the completed stack
          aos.push_back(aocurr);
          // Clear all current-object pointers
          aocurr = NULL;
          cncurr = NULL;
          h1curr = NULL; h2curr = NULL;
          p1curr = NULL; p2curr = NULL;
          s1curr = NULL; s2curr = NULL; s3curr = NULL;
          annscurr.clear();
          context = NONE;
          continue;
        }

        // Extract annotations (after converting to YAML syntax)
        const size_t ieq = s.find("=");
        if (ieq != string::npos) s.replace(ieq, 1, ": ");
        const size_t ico = s.find(":");
        if (ico != string::npos) {
          // cout << "@@@ '" << s << "'" << endl;
          annscurr += (annscurr.empty() ? "" : "\n") + s;
          // const string akey = s.substr(0, ieq);
          // const string aval = s.substr(ieq+1);
          continue;
        }

        //istringstream iss(s);
        aiss.reset(s);
        switch (context) {

        case COUNTER:
          {
            double sumw(0), sumw2(0); unsigned long n(0);
            aiss >> sumw >> sumw2 >> n;
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
              aiss >> xoflow1 >> xoflow2;
            } else {
              aiss >> xmin >> xmax;
            }
            // The rest is the same for overflows and in-range bins
            aiss >> sumw >> sumw2 >> sumwx >> sumwx2 >> n;
            const Dbn1D dbn(n, sumw, sumw2, sumwx, sumwx2);
            if (xoflow1 == "Total") h1curr->setTotalDbn(dbn);
            else if (xoflow1 == "Underflow") h1curr->setUnderflow(dbn);
            else if (xoflow1 == "Overflow")  h1curr->setOverflow(dbn);
            // else h1curr->addBin(HistoBin1D(std::make_pair(xmin,xmax), dbn));
            else h1binscurr.push_back(HistoBin1D(std::make_pair(xmin,xmax), dbn));
          }
          break;

        case HISTO2D:
          {
            string xoflow1, xoflow2, yoflow1, yoflow2; double xmin(0), xmax(0), ymin(0), ymax(0);
            double sumw(0), sumw2(0), sumwx(0), sumwx2(0), sumwy(0), sumwy2(0), sumwxy(0); unsigned long n(0);
            /// @todo Improve/factor this "bin" string-or-float parsing... esp for mixed case of 2D overflows
            /// @todo When outflows are treated as "infinity bins" and don't require a distinct type, string replace under/over -> -+inf
            if (s.find("Total") != string::npos) {
              aiss >> xoflow1 >> xoflow2; // >> yoflow1 >> yoflow2;
            } else if (s.find("Underflow") != string::npos || s.find("Overflow") != string::npos) {
              throw ReadError("2D histogram overflow syntax is not yet defined / handled");
            } else {
              aiss >> xmin >> xmax >> ymin >> ymax;
            }
            // The rest is the same for overflows and in-range bins
            aiss >> sumw >> sumw2 >> sumwx >> sumwx2 >> sumwy >> sumwy2 >> sumwxy >> n;
            const Dbn2D dbn(n, sumw, sumw2, sumwx, sumwx2, sumwy, sumwy2, sumwxy);
            if (xoflow1 == "Total") h2curr->setTotalDbn(dbn);
            // else if (xoflow1 == "Underflow") p1curr->setUnderflow(dbn);
            // else if (xoflow1 == "Overflow")  p1curr->setOverflow(dbn);
            else {
              assert(xoflow1.empty());
              // h2curr->addBin(HistoBin2D(std::make_pair(xmin,xmax), std::make_pair(ymin,ymax), dbn));
              h2binscurr.push_back(HistoBin2D(std::make_pair(xmin,xmax), std::make_pair(ymin,ymax), dbn));
            }
          }
          break;

        case PROFILE1D:
          {
            string xoflow1, xoflow2; double xmin(0), xmax(0);
            double sumw(0), sumw2(0), sumwx(0), sumwx2(0), sumwy(0), sumwy2(0); unsigned long n(0);
            /// @todo Improve/factor this "bin" string-or-float parsing... esp for mixed case of 2D overflows
            /// @todo When outflows are treated as "infinity bins" and don't require a distinct type, string replace under/over -> -+inf
            if (s.find("Total") != string::npos || s.find("Underflow") != string::npos || s.find("Overflow") != string::npos) {
              aiss >> xoflow1 >> xoflow2;
            } else {
              aiss >> xmin >> xmax;
            }
            // The rest is the same for overflows and in-range bins
            aiss >> sumw >> sumw2 >> sumwx >> sumwx2 >> sumwy >> sumwy2 >> n;
            const double DUMMYWXY = 0;
            const Dbn2D dbn(n, sumw, sumw2, sumwx, sumwx2, sumwy, sumwy2, DUMMYWXY);
            if (xoflow1 == "Total") p1curr->setTotalDbn(dbn);
            else if (xoflow1 == "Underflow") p1curr->setUnderflow(dbn);
            else if (xoflow1 == "Overflow")  p1curr->setOverflow(dbn);
            // else p1curr->addBin(ProfileBin1D(std::make_pair(xmin,xmax), dbn));
            else p1binscurr.push_back(ProfileBin1D(std::make_pair(xmin,xmax), dbn));
          }
          break;

        case PROFILE2D:
          {
            string xoflow1, xoflow2, yoflow1, yoflow2; double xmin(0), xmax(0), ymin(0), ymax(0);
            double sumw(0), sumw2(0), sumwx(0), sumwx2(0), sumwy(0), sumwy2(0), sumwz(0), sumwz2(0), sumwxy(0), sumwxz(0), sumwyz(0); unsigned long n(0);
            /// @todo Improve/factor this "bin" string-or-float parsing... esp for mixed case of 2D overflows
            /// @todo When outflows are treated as "infinity bins" and don't require a distinct type, string replace under/over -> -+inf
            if (s.find("Total") != string::npos) {
              aiss >> xoflow1 >> xoflow2; // >> yoflow1 >> yoflow2;
            } else if (s.find("Underflow") != string::npos || s.find("Overflow") != string::npos) {
              throw ReadError("2D profile overflow syntax is not yet defined / handled");
            } else {
              aiss >> xmin >> xmax >> ymin >> ymax;
            }
            // The rest is the same for overflows and in-range bins
            aiss >> sumw >> sumw2 >> sumwx >> sumwx2 >> sumwy >> sumwy2 >> sumwz >> sumwz2 >> sumwxy >> sumwxz >> sumwyz >> n;
            const Dbn3D dbn(n, sumw, sumw2, sumwx, sumwx2, sumwy, sumwy2, sumwz, sumwz2, sumwxy, sumwxz, sumwyz);
            if (xoflow1 == "Total") p2curr->setTotalDbn(dbn);
            // else if (xoflow1 == "Underflow") p2curr->setUnderflow(dbn);
            // else if (xoflow1 == "Overflow")  p2curr->setOverflow(dbn);
            else {
              assert(xoflow1.empty());
              // p2curr->addBin(ProfileBin2D(std::make_pair(xmin,xmax), std::make_pair(ymin,ymax), dbn));
              p2binscurr.push_back(ProfileBin2D(std::make_pair(xmin,xmax), std::make_pair(ymin,ymax), dbn));
            }
          }
          break;

        case SCATTER1D:
          {
            double x(0), exm(0), exp(0);
            aiss >> x >> exm >> exp;
            // s1curr->addPoint(Point1D(x, exm, exp));
            pt1scurr.push_back(Point1D(x, exm, exp));
          }
          break;

        case SCATTER2D:
          {
            double x(0), y(0), exm(0), exp(0), eym(0), eyp(0);
            /// @todo Need to improve this format for multi-err points
            aiss >> x >> exm >> exp >> y >> eym >> eyp;
            // s2curr->addPoint(Point2D(x, y, exm, exp, eym, eyp));
            pt2scurr.push_back(Point2D(x, y, exm, exp, eym, eyp));
          }
          break;

        case SCATTER3D:
          {
            double x(0), y(0), z(0), exm(0), exp(0), eym(0), eyp(0), ezm(0), ezp(0);
            /// @todo Need to improve this format for multi-err points
            aiss >> x >> exm >> exp >> y >> eym >> eyp >> z >> ezm >> ezp;
            // s3curr->addPoint(Point3D(x, y, z, exm, exp, eym, eyp, ezm, ezp));
            pt3scurr.push_back(Point3D(x, y, z, exm, exp, eym, eyp, ezm, ezp));
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
