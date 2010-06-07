// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2009 The YODA collaboration (see AUTHORS for details)
//

#include "YODA/Writer.h"
#include <iostream>
#include <typeinfo>
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
    /// @todo Do this for real!

    // Use RTTI to decide which down-cast to do.
    const Histo1D example_histo(1, 0.0, 1.0, "Hist1D title");
    const Profile1D example_profile(1, 0.0, 1.0, "Prof1D title");
    if (typeid(ao) == typeid(example_histo)) {
      cout << "H" << endl;
      writeHisto(stream, static_cast<const Histo1D&>(ao), "/foo");
    }
    else if (typeid(ao) == typeid(example_profile)) {
      cout << "P" << endl;
      writeProfile(stream, static_cast<const Profile1D&>(ao), "/foo");
    } else {
      cerr << "OTHER" << endl;
      throw Exception("Unrecognised analysis object type in Writer::write");
    }
  }


}
