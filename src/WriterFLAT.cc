// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2014 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/WriterFLAT.h"

#include "YODA/Histo1D.h"
#include "YODA/Histo2D.h"
#include "YODA/Profile1D.h"
#include "YODA/Scatter2D.h"

#include <iostream>
#include <iomanip>

using namespace std;

namespace YODA {


  void WriterFLAT::writeHeader(std::ostream& os) {
    os << flush;
  }


  void WriterFLAT::writeFooter(std::ostream& os) {
    os << flush;
  }


  void WriterFLAT::_writeAnnotations(std::ostream& os, const AnalysisObject& ao) {
    os << scientific << setprecision(_precision);
    foreach (const string& a, ao.annotations()) {
      if (a.empty()) continue;
      /// @todo Should write out floating point annotations as scientific notation...
      os << a << "=" << ao.annotation(a) << "\n";
    }
  }


  void WriterFLAT::writeCounter(std::ostream& os, const Counter& c) {
    /// @todo Currently not supported
    os << flush;
    //Scatter1D tmp = mkScatter(c);
    //tmp.setAnnotation("Type", "Counter");
    //writeScatter1D(os, tmp);
  }


  void WriterFLAT::writeHisto1D(std::ostream& os, const Histo1D& h) {
    Scatter2D tmp = mkScatter(h);
    tmp.setAnnotation("Type", "Histo1D");
    writeScatter2D(os, tmp);
  }


  void WriterFLAT::writeHisto2D(std::ostream& os, const Histo2D& h) {
    /// @todo Currently not supported
    os << flush;
    //Scatter3D tmp = mkScatter(h);
    //tmp.setAnnotation("Type", "Histo2D");
    //writeScatter3D(os, tmp);
  }


  void WriterFLAT::writeProfile1D(std::ostream& os, const Profile1D& p) {
    Scatter2D tmp = mkScatter(p);
    tmp.setAnnotation("Type", "Profile1D");
    writeScatter2D(os, tmp);
  }


  void WriterFLAT::writeProfile2D(std::ostream& os, const Profile2D& h) {
    /// @todo Currently not supported
    os << flush;
    //Scatter3D tmp = mkScatter(h);
    //tmp.setAnnotation("Type", "Histo2D");
    //writeScatter3D(os, tmp);
  }


  // void WriterFLAT::writeScatter1D(std::ostream& os, const Scatter1D& h) {
  //   /// @todo Currently not supported
  //   os << flush;
  // }


  void WriterFLAT::writeScatter2D(std::ostream& os, const Scatter2D& s) {
    ios_base::fmtflags oldflags = os.flags();
    os << scientific << showpoint << setprecision(_precision);

    os << "# BEGIN HISTOGRAM " << s.path() << "\n";
    _writeAnnotations(os, s);
    os << "# xlow\t xhigh\t val\t errminus\t errplus\n";
    foreach (Point2D pt, s.points()) {
      os << pt.x()-pt.xErrMinus() << "\t" << pt.x()+pt.xErrPlus() << "\t";
      os << pt.y() << "\t" << pt.yErrMinus() << "\t" << pt.yErrPlus() << "\n";
    }
    os << "# END HISTOGRAM\n\n";

    os << flush;
    os.flags(oldflags);
  }


  /*void WriterFLAT::writeScatter3D(std::ostream& os, const Scatter3D& s) {
    ios_base::fmtflags oldflags = os.flags();
    os << scientific << showpoint << setprecision(_precision);

    os << "# BEGIN YODA_SCATTER3D " << s.path() << "\n";
    _writeAnnotations(os, s);
    os << "# xval\t xerr-\t xerr+\t yval\t yerr-\t yerr+\t zval\t zerr-\t zerr+\n";
    foreach (Point3D pt, s.points()) {
      os << pt.x() << "\t" << pt.xErrMinus() << "\t" << pt.xErrMinus() << "\t";
      os << pt.y() << "\t" << pt.yErrMinus() << "\t" << pt.yErrMinus() << "\t";
      os << pt.z() << "\t" << pt.zErrMinus() << "\t" << pt.zErrMinus() << "\n";
    }
    os << "# END YODA_SCATTER2D\n";

    os << flush;
    os.flags(oldflags);
  }*/


}
