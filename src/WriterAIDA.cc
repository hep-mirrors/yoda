// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/WriterAIDA.h"

#include <iostream>
using namespace std;

namespace YODA {


  Writer* WriterAIDA::_instance = 0;

  
  string encodeForXML(const string& in) {
    string out = in;
    typedef pair<string, string> CharsToEntities;
    vector<CharsToEntities> cs2es;
    cs2es.push_back(make_pair("&", "&amp;"));
    cs2es.push_back(make_pair("<", "&lt;"));
    cs2es.push_back(make_pair(">", "&gt;"));

    for (vector<CharsToEntities>::const_iterator c2e = cs2es.begin();
         c2e != cs2es.end(); ++c2e) {
      string::size_type pos = -1;
      while ( ( pos = out.find(c2e->first, pos + 1) ) != string::npos ) {
        out.replace(pos, 1, c2e->second);
      }
    }
    return out;
  }


  void WriterAIDA::writeHeader(std::ostream& stream) { 
    stream << "<aida>" << "\n";
  }

  void WriterAIDA::writeFooter(std::ostream& stream) { 
    stream << "</aida>" << "\n";
  }

  void WriterAIDA::writeHisto(std::ostream& os, const Histo1D& h) {
    // <histogram1d>
    os << "<histogram1d" // name=\"" << encodeForXML(h.name()) << "\""
       << " title=\"" << encodeForXML(h.title()) << "\""
       << " path=\"" << h.path() << "\">\n";
    // <axis>
    os << "  <axis"
       << " numberOfBins=\"" << h.bins().size() << "\""
       << " direction=\"x\">\n";
    // <binBorder>
    for (vector<HistoBin>::const_iterator b = h.bins().begin(); b != h.bins().end(); ++b) {
      os << "    <binBorder value=\"" << b->highEdge() << "\" />\n";
    }
    os << "  </axis>\n";
    // <statistics>
    os << "  <statistics entries=\"" << h.area() << "\">\n";
    os << "    <statistic mean=\"" << h.mean() << "\"" << " direction=\"x\" />\n";
    os << "  </statistics>\n";

    // Data section
    os << "  <data1d>\n";
    // Underflow and overflow
    HistoBin uf = h.bin(Bin::UNDERFLOWBIN);
    os << "    <bin1d binNum=\"UNDERFLOW\"" 
       << " entries=\"" << uf.area() << "\""
       << " height=\"" << uf.height() << "\""
       << " error=\"" << uf.heightError() << "\""
       << " weightedMean=\"" << uf.focus() << "\""
       << " />\n";
    HistoBin of = h.bin(Bin::OVERFLOWBIN);
    os << "    <bin1d binNum=\"OVERFLOW\"" 
       << " entries=\"" << of.area() << "\""
       << " height=\"" << of.height() << "\""
       << " error=\"" << of.heightError() << "\""
       << " weightedMean=\"" << of.focus() << "\""
       << " />\n";
    // Normal bins
    for (vector<HistoBin>::const_iterator b = h.bins().begin(); b != h.bins().end(); ++b) {
      os << "    <bin1d"
         << " entries=\"" << b->area() << "\""
         << " height=\"" << b->height() << "\""
         << " error=\"" << b->heightError() << "\""
         << " weightedMean=\"" << b->focus() << "\""
         << " />\n";
    }
    os << "  </data1d>\n";
    os << "</histogram1d>\n";
    os << flush;
  }


  void WriterAIDA::writeProfile(std::ostream& stream, const Profile1D& p) { 
    //
  }


  // void WriterAIDA::writeScatter(std::ostream& stream, const Scatter& p) {
  //   //
  // }


}
