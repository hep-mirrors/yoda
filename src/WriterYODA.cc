// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2011 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/WriterYODA.h"

#include <iostream>
#include <iomanip>

using namespace std;

namespace YODA {


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


  void WriterYODA::writeHisto1D(std::ostream& os, const Histo1D& h) {
    ios_base::fmtflags oldflags = os.flags();
    const int precision = 6;
    os << scientific << showpoint << setprecision(precision);

    os << "# BEGIN HISTO1D " << h.path() << "\n";
    os << "AidaPath=" << h.path() << "\n";
    os << "Title=" << h.title() << "\n";
    os << "## Mean: " << h.mean() << "\n";
    os << "## Area: " << h.area() << "\n";
    os << "## xlow\t\t xhigh\t\t yval\t\t yerr\t\t sumw\t\t sumw2\t\t sumwx\t\t sumwx2\n";
    for (vector<HistoBin>::const_iterator b = h.bins().begin(); b != h.bins().end(); ++b) {
      os << b->lowEdge() << '\t' << b->highEdge() << '\t';
      os << b->height() << '\t' << b->heightError() << '\t';
      os << b->sumW() << '\t' << b->sumW2() << '\t';
      os << b->sumWX() << '\t' << b->sumWX2() << '\n';
    }
    os << "## END HISTO1D\n\n";

    os.flags(oldflags);
  }


  void WriterYODA::writeProfile1D(std::ostream& os, const Profile1D& p) {
    ios_base::fmtflags oldflags = os.flags();
    const int precision = 6;
    os << scientific << showpoint << setprecision(precision);

    os << "## BEGIN PROFILE1D\n";
    os << "## Title: " << p.title() << "\n";
    os << "## Path: " << p.path() << "\n";
    os << "# xlow\t xhigh\t yval\t yerr\t sumw\t sumw2\t sumwx\t sumwx2\t sumwy\t sumwy2 \n";
    for (vector<ProfileBin>::const_iterator b = p.bins().begin(); b != p.bins().end(); ++b) {
      os << b->lowEdge() << "\t" << b->highEdge() << "\t";
      os << b->mean() << "\t" << b->stdErr() << "\t";
      os << b->sumW() << "\t" << b->sumW2() << "\t";
      os << b->sumWX() << "\t" << b->sumWX2() << "\t";
      os << b->sumWY() << "\t" << b->sumWY2();
      os << "\n";
    }
    os << "## END PROFILE1D\n";

    os.flags(oldflags);
  }



  // void WriterAIDA::writeScatter2D(std::ostream& stream, const Scatter2D& p) {
  //   ios_base::fmtflags oldflags = os.flags();
  //   const int precision = 6;
  //   os << scientific << showpoint << setprecision(precision);

  //   //

  //   os.flags(oldflags);
  // }


}
