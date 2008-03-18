// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/WriterAIDA.h"

#include <iostream>
using namespace std;


namespace YODA {
  
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


  bool WriterAIDA::write(ostream& stream, const AnalysisObject& ao) {
    // RTTI goes here
  }
  bool WriterAIDA::write(const string& filename, const AnalysisObject& ao) {}


  bool WriterAIDA::write(ostream& stream, const Histo1D& h) {
    // <histogram1d>
    os << "<histogram1d name=\"" << encodeForXML(h.name()) << "\""
       << " title=\"" << encodeForXML(h.title()) << "\""
       << " path=\"" << h.path() << "\">\n";
    // <axis>
    os << "  <axis max=\"" << ax->upperEdge() << "\""
       << " numberOfBins=\"" << ax->bins() << "\""
       << " min=\"" << ax->lowerEdge() << "\""
       << " direction=\"x\">\n";
    // <binBorder>
    for (int i = 0, N = ax->bins() - 1; i < N; ++i ) { //< @todo Replace with iterator over bins
      os << "    <binBorder value=\"" << ax->binUpperEdge(i) << "\" />\n";
    }
    os << "  </axis>\n";
    // <statistics>
    os << "  <statistics entries=\"" << totalArea() << "\">\n";
    os << "    <statistic mean=\"" << mean() << "\"" << " direction=\"x\" />\n";
    os << "  </statistics>\n";

    // Data section
    os << "  <data1d>\n";
    // Underflow and overflow
    Bin uf = h.getBin(UNDERFLOW);
    os << "    <bin1d binNum=\"UNDERFLOW\"" 
       << " entries=\"" << uf.area() << "\""
       << " height=\"" << uf.height() << "\""
       << " error=\"" << uf.error() << "\""
       << " weightedMean=\"" << uf.focus() << "\""
       << " />\n";
    Bin of = h.getBin(OVERFLOW);
    os << "    <bin1d binNum=\"OVERFLOW\"" 
       << " entries=\"" << of.area() << "\""
       << " height=\"" << of.height() << "\""
       << " error=\"" << of.error() << "\""
       << " weightedMean=\"" << of.focus() << "\""
       << " />\n";
    // Normal bins
    for (int i = 0; i < ax->bins() + 2; ++i) { //< @todo Replace with iterator over bins
      Bin b = h.getBin(i);
      os << "    <bin1d binNum=\"" << i << "\""
         << " entries=\"" << b.area() << "\""
         << " height=\"" << b.height() << "\""
         << " error=\"" << b.error() << "\""
         << " weightedMean=\"" << b.focus() << "\""
         << " />\n";
    }
    os << "  </data1d>\n";
    os << "</histogram1d>\n";
    os << flush;
  }


  
  // bool WriterAIDA::write(ostream& stream, const string& format, const vector<AnalysisObject>& ao) {}
  // bool WriterAIDA::write(const string& filename, const string& format, const vector<AnalysisObject>& ao) {}
  // bool WriterAIDA::write(const string& filename, const vector<AnalysisObject>& ao) {}

  // bool WriterAIDA::write(ostream& stream, const string& format, 
  //            const iterator<AnalysisObject>& begin, const iterator<AnalysisObject>& end) {}
  // bool WriterAIDA::write(const string& filename, const string& format, 
  //            const iterator<AnalysisObject>& begin, const iterator<AnalysisObject>& end) {}
  // bool WriterAIDA::write(const string& filename, 
  //            const iterator<AnalysisObject>& begin, const iterator<AnalysisObject>& end) {}

}
