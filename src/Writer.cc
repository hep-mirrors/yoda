// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2010 The YODA collaboration (see AUTHORS for details)
//

#include "YODA/Writer.h"
#include <iostream>
#include <typeinfo>
#include <sstream>

using namespace std;

namespace YODA {


  void Writer::write(std::ostream& stream, const AnalysisObject& ao) {
    writeHeader(stream);
    writeBody(stream, ao);
    writeFooter(stream);
  }


  void Writer::write(const std::string& filename, const AnalysisObject& ao) {
    ofstream outstream;
    outstream.open(filename.c_str());
    write(outstream, ao);
    outstream.close();
  }


  void Writer::writeBody(std::ostream& stream, const AnalysisObject& ao) {
    const string aotype = ao._aotype();
    /// @todo Handle paths -- where to set them?
    if (aotype == "Histo1D") {
      writeHisto1D(stream, dynamic_cast<const Histo1D&>(ao), "/foo");
    } else if (aotype == "Profile1D") {
      writeProfile1D(stream, dynamic_cast<const Profile1D&>(ao), "/foo");
    } else {
      ostringstream oss;
      oss << "Unrecognised analysis object type " << aotype << " in Writer::write";
      throw Exception(oss.str());
    }
  }


}
