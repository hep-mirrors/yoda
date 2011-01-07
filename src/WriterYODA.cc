// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2010 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/WriterYODA.h"

#include <iostream>
#include <iomanip>

using namespace std;

namespace YODA {


  Writer* WriterYODA::_instance = 0;


  void WriterYODA::writeHeader(std::ostream& os) {
    // os <<
    //   "# BEGIN PLOT\n"
    //   "LogY=0\n"
    //   "Title=Test Histo\n"
    //   "# END PLOT\n\n";
  }


  void WriterYODA::writeFooter(std::ostream& os) {
    os << flush;
  }


  void WriterYODA::writeHisto1D(std::ostream& os, const Histo1D& h, const std::string& path) {
    ios_base::fmtflags oldflags = os.flags();

    const int precision = 6;
    os << scientific << showpoint << setprecision(precision);

    os << "# BEGIN HISTO1D " << path << "\n";
    os << "AidaPath=" << path << "\n";
    os << "Title=" << h.title() << "\n";
    os << "## Mean: " << h.mean() << "\n";
    os << "## Area: " << h.area() << "\n";
    os << "## xlow\t\t xhigh\t\t yval\t\t yerr\t\t sumw\t\t sumw2\t\t sumwx\t\t sumwx2\n";
    //HistoBin uf = h.bin(Bin::UNDERFLOWBIN);
    for (vector<HistoBin>::const_iterator b = h.bins().begin(); b != h.bins().end(); ++b) {
      os << b->lowEdge() << '\t' << b->highEdge() << '\t';
      os << b->height() << '\t' << b->heightError() << '\t';
      os << b->sumW() << '\t' << b->sumW2() << '\t';
      os << b->sumWX() << '\t' << b->sumWX2() << '\n';
    }
    //HistoBin of = h.bin(Bin::OVERFLOWBIN);
    os << "## END HISTO1D\n\n";

    os.flags(oldflags);
  }


  void WriterYODA::writeProfile1D(std::ostream& os, const Profile1D& p, const std::string& path) {
    cerr << "WriterYODA::writeProfile() NOT UPDATED YET\n;";
    os << "## BEGIN PROFILE1D\n";
    os << "## Title: " << p.title() << "\n";
    os << "## Path: " << path << "\n";
    os << "# xlow\t xhigh\t yval\t yerr\t sumw\t sumw2\t sumwx\t sumwx2\t sumwy\t sumwy2 \n";
    //HistoBin uf = p.bin(Bin::UNDERFLOWBIN);
    for (vector<ProfileBin>::const_iterator b = p.bins().begin(); b != p.bins().end(); ++b) {
      os << b->lowEdge() << "\t" << b->highEdge() << "\t";
      os << b->mean() << "\t" << b->stdErr() << "\t";
      os << b->sumW() << "\t" << b->sumW2() << "\t";
      os << b->sumWX() << "\t" << b->sumWX2() << "\t";
      os << b->sumWY() << "\t" << b->sumWY2();
      os << "\n";
    }
    //HistoBin of = p.bin(Bin::OVERFLOWBIN);
    os << "## END PROFILE1D\n";
  }


  // void writeScatterData1D(std::ostream& os, const Scatter& s, const std::string& path) {
  //   const Scatter1D& s1 = dynamic_cast<const Scatter1D&>(s);
  //   for (vector<Point1D>::const_iterator p = s1.points().begin(); p != s1.points().end(); ++p) {
  //     /// @todo Write all errors, plus annotations & type
  //     os << p->value(0) << "+-(" << p->symmError(0) << ")";
  //     os << "\n";
  //   }
  // }


  // void writeScatterData2D(std::ostream& os, const Scatter& s, const std::string& path) {
  //   const Scatter2D& s2 = dynamic_cast<const Scatter2D&>(s);
  //   for (vector<Point2D>::const_iterator p = s2.points().begin(); p != s2.points().end(); ++p) {
  //     /// @todo Write all errors, plus annotations & type
  //     os << p->value(0) << "+-(" << p->symmError(0) << ")";
  //     os << "\t";
  //     os << p->value(1) << "+-(" << p->symmError(1) << ")";
  //     os << "\n";
  //   }
  // }


  // void WriterYODA::writeScatter(std::ostream& os, const Scatter& s, const std::string& path) {
  //   os << "## BEGIN SCATTER\n";
  //   os << "## Title: " << s.title() << "\n";
  //   os << "## Path: " << path << "\n";
  //   os << "## NDims: " << s.numDims() << "\n";
  //   os << "# x1+-(m1,p1)\t x2+-(m1,p1)\t ...\n";
  //   if (s.numDims() == 1) {
  //     writeScatterData1D(os, s);
  //   } else if (s.numDims() == 2) {
  //     writeScatterData2D(os, s);
  //   }
  //   os << "## END SCATTER\n";
  //   os << flush;
  // }


}
