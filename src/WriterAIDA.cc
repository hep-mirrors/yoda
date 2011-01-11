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
    writeScatter2D(os, mkScatter(h));
  }


  void WriterAIDA::writeProfile1D(std::ostream& os, const Profile1D& p) {
    writeScatter2D(os, mkScatter(p));
  }


  void WriterAIDA::writeScatter2D(std::ostream& os, const Scatter2D& s) {
    /// @todo Parse the path and take the last part (use boost via StringUtils)
    const string name = s.path();
    os << "  <dataPointSet name=\"" << Utils::encodeForXML(name) << "\""
       << " title=\"" << Utils::encodeForXML(s.title()) << "\""
       << " path=\"" << s.path() << "\">\n";
    os << "  <dimension dim=\"0\" title=\"\" />\n";
    os << "  <dimension dim=\"1\" title=\"\" />\n";
    typedef pair<string,string> sspair;
    foreach (const sspair& kv, s.annotations()) {
      os << "  <annotation name=\"" << kv.first << "\" value=\"" << kv.second << "\" />\n";
    }
    foreach (Point2D pt, s.points()) {
      os << "    <dataPoint>\n";
      os << "      <measurement value=\"" << pt.x()
         << "\" errorMinus=\"" << pt.xErrMinus() << "\" errorPlus=\"" << pt.xErrPlus() << "/>\n";
      os << "      <measurement value=\"" << pt.y()
         << "\" errorMinus=\"" << pt.yErrMinus() << "\" errorPlus=\"" << pt.yErrPlus() << "/>\n";
      os << "    </dataPoint>\n";
    }
    os << "  </dataPointSet>\n";
    os << flush;
  }


}
