// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/WriterAIDA.h"
#include "YODA/Histo1D.h"

#include <iostream>
#include <fstream>
#include <typeinfo>
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
    // Use RTTI to decide which down-cast to do.
    if (typeid(ao) == typeid(Histo1D())) {
      return write(stream, static_cast<const Histo1D&>(ao));
    }
    return false;
  }


  bool WriterAIDA::write(const string& filename, const AnalysisObject& ao) {
    ofstream outstream;
    outstream.open(filename.c_str());
    bool ok = write(outstream, ao);
    outstream.close();
    return ok;
  }


  bool WriterAIDA::write(ostream& stream, const vector<AnalysisObject>& aos) {
    for (vector<AnalysisObject>::const_iterator ao = aos.begin(); ao != aos.end(); ++ao) {
      const AnalysisObject& aoref = *ao;
      bool ok = write(stream, aoref);
      if (!ok) return false;
    }
    return true;
  }


  bool WriterAIDA::write(const std::string& filename, const vector<AnalysisObject>& aos) {
    ofstream outstream;
    outstream.open(filename.c_str());
    bool ok = write(outstream, aos);
    outstream.close();
    return ok;
  }


  bool WriterAIDA::write(std::ostream& stream, 
                         const vector<AnalysisObject>::const_iterator& begin, 
                         const vector<AnalysisObject>::const_iterator& end) {
    /// @todo
    return true;
  }


  bool WriterAIDA::write(const std::string& filename,
                         const vector<AnalysisObject>::const_iterator& begin, 
                         const vector<AnalysisObject>::const_iterator& end) {
    /// @todo
    return true;
  }



  bool WriterAIDA::write(ostream& os, const Histo1D& h) {
    // <histogram1d>
    os << "<histogram1d" // name=\"" << encodeForXML(h.name()) << "\""
       << " title=\"" << encodeForXML(h.title()) << "\""
       << " path=\"" << h.path() << "\">\n";
    // <axis>
    os << "  <axis"
       << " numberOfBins=\"" << h.bins().size() << "\""
       << " direction=\"x\">\n";
    // <binBorder>
    for (vector<Bin>::const_iterator b = h.bins().begin(); b != h.bins().end(); ++b) {
      os << "    <binBorder value=\"" << b->highEdge() << "\" />\n";
    }
    os << "  </axis>\n";
    // <statistics>
    os << "  <statistics entries=\"" << h.totalArea() << "\">\n";
    os << "    <statistic mean=\"" << h.mean() << "\"" << " direction=\"x\" />\n";
    os << "  </statistics>\n";

    // Data section
    os << "  <data1d>\n";
    // Underflow and overflow
    Bin uf = h.bin(Histo1D::UNDERFLOWBIN);
    os << "    <bin1d binNum=\"UNDERFLOW\"" 
       << " entries=\"" << uf.area() << "\""
       << " height=\"" << uf.height() << "\""
       << " error=\"" << uf.heightError() << "\""
       << " weightedMean=\"" << uf.focus() << "\""
       << " />\n";
    Bin of = h.bin(Histo1D::OVERFLOWBIN);
    os << "    <bin1d binNum=\"OVERFLOW\"" 
       << " entries=\"" << of.area() << "\""
       << " height=\"" << of.height() << "\""
       << " error=\"" << of.heightError() << "\""
       << " weightedMean=\"" << of.focus() << "\""
       << " />\n";
    // Normal bins
    for (vector<Bin>::const_iterator b = h.bins().begin(); b != h.bins().end(); ++b) {
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
    /// @todo How to check if it's gone wrong?
    return true;
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
