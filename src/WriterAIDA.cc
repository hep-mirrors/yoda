// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2011 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/WriterAIDA.h"
#include "YODA/Utils/StringUtils.h"

#include <iostream>
using namespace std;

namespace YODA {


  void WriterAIDA::writeHeader(std::ostream& stream) {
    stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    stream << "<!DOCTYPE aida SYSTEM \"http://aida.freehep.org/schemas/3.0/aida.dtd\">\n";
    stream << "<aida>\n";
    stream << "  <implementation version=\"1.0\" package=\"YODA\"/>\n";
  }

  void WriterAIDA::writeFooter(std::ostream& stream) {
    stream << "</aida>\n";
  }

  void WriterAIDA::writeHisto1D(std::ostream& os, const Histo1D& h) {
    /// @todo Parse the path and take the last part (use boost)
    const string name = h.path();
    os << "  <dataPointSet name=\"" << Utils::encodeForXML(name) << "\""
       << " title=\"" << Utils::encodeForXML(h.title()) << "\""
       << " path=\"" << h.path() << "\">\n";
    os << "  <dimension dim=\"0\" title=\"\" />\n";
    os << "  <dimension dim=\"1\" title=\"\" />\n";
    for (vector<HistoBin>::const_iterator b = h.bins().begin(); b != h.bins().end(); ++b) {
      os << "    <dataPoint>\n";
      const double x = b->focus();
      const double ex_m = b->focus() - b->lowEdge();
      const double ex_p = b->highEdge() - b->focus();
      os << "      <measurement value=\"" << x
         << "\" errorMinus=\"" << ex_m << "\" errorPlus=\"" << ex_p << "/>\n";
      const double y = b->height();
      const double ey = b->heightError();
      os << "      <measurement value=\"" << y
         << "\" errorMinus=\"" << ey << "\" errorPlus=\"" << ey << "/>\n";
      os << "    </dataPoint>\n";
    }
    os << "  </dataPointSet>\n";
    os << flush;
  }


  void WriterAIDA::writeProfile1D(std::ostream& os, const Profile1D& p) {
    /// @todo Parse the path and take the last part (use boost)
    const string name = p.path();
    os << "  <dataPointSet name=\"" << Utils::encodeForXML(name) << "\""
       << " title=\"" << Utils::encodeForXML(p.title()) << "\""
       << " path=\"" << p.path() << "\">\n";
    os << "  <dimension dim=\"0\" title=\"\" />\n";
    os << "  <dimension dim=\"1\" title=\"\" />\n";
    for (vector<ProfileBin>::const_iterator b = p.bins().begin(); b != p.bins().end(); ++b) {
      os << "    <dataPoint>\n";
      const double x = b->focus();
      const double ex_m = b->focus() - b->lowEdge();
      const double ex_p = b->highEdge() - b->focus();
      os << "      <measurement value=\"" << x
         << "\" errorMinus=\"" << ex_m << "\" errorPlus=\"" << ex_p << "/>\n";
      const double y = b->mean();
      const double ey = b->stdErr();
      os << "      <measurement value=\"" << y
         << "\" errorMinus=\"" << ey << "\" errorPlus=\"" << ey << "/>\n";
      os << "    </dataPoint>\n";
    }
    os << "  </dataPointSet>\n";
    os << flush;
  }


  // void WriterAIDA::writeScatter2D(std::ostream& stream, const Scatter2D& p) {
  //   //
  // }


}
