// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Writer.h"

#include <iostream>
#include <fstream>
#include <typeinfo>
using namespace std;

namespace YODA {


  bool Writer::write(std::ostream& stream, const AnalysisObject& ao) {
    // Use RTTI to decide which down-cast to do.
    if (typeid(ao) == typeid(Histo1D())) {
      return writeHisto(stream, static_cast<const Histo1D&>(ao));
    }
    return false;
  }


  bool Writer::write(const std::string& filename, const AnalysisObject& ao) {
    ofstream outstream;
    outstream.open(filename.c_str());
    bool ok = write(outstream, ao);
    outstream.close();
    return ok;
  }


  bool Writer::write(std::ostream& stream, const vector<AnalysisObject>& aos) {
    return write(stream, aos.begin(), aos.end());
  }


  bool Writer::write(const std::string& filename, const vector<AnalysisObject>& aos) {
    ofstream outstream;
    outstream.open(filename.c_str());
    bool ok = write(outstream, aos);
    outstream.close();
    return ok;
  }


  /// @todo Do these bool returns serve any real purpose?
  bool Writer::write(std::ostream& stream, 
                         const std::vector<AnalysisObject>::const_iterator& begin, 
                         const std::vector<AnalysisObject>::const_iterator& end) {
    bool ok = true;
    for (std::vector<AnalysisObject>::const_iterator ao = begin; ao != end; ++ao) {
      ok = write(stream, *ao);
      if (!ok) {
        throw Exception("Error when writing to output stream");
      }
    }
    return ok;
  }


  bool Writer::write(const std::string& filename,
                         const std::vector<AnalysisObject>::const_iterator& begin, 
                         const std::vector<AnalysisObject>::const_iterator& end) {
    ofstream outstream;
    outstream.open(filename.c_str());
    bool ok = write(outstream, begin, end);
    outstream.close();
    return ok;
  }

}
