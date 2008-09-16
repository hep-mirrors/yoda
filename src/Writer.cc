// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//

#include "YODA/Writer.h"
#include <iostream>
#include <typeinfo>
using namespace std;

namespace YODA {


  void Writer::write(std::ostream& stream, const AnalysisObject& ao) {
    // Use RTTI to decide which down-cast to do.
    writeHeader(stream);
    const Histo1D example_histo("/foo", "Title", 1, 0.0, 1.0);
    const Profile1D example_profile("/foo", "Title", 1, 0.0, 1.0);
    if (typeid(ao) == typeid(example_histo)) {
      cout << "H" << endl;
      writeHisto(stream, static_cast<const Histo1D&>(ao));
    } 
    else if (typeid(ao) == typeid(example_profile)) {
      cout << "P" << endl;
      writeProfile(stream, static_cast<const Profile1D&>(ao));
    } else {
      cerr << "OTHER" << endl;
      throw Error("Unrecognised analysis object type in Writer::write");
    }
    writeFooter(stream);
  }


  void Writer::write(const std::string& filename, const AnalysisObject& ao) {
    ofstream outstream;
    outstream.open(filename.c_str());
    write(outstream, ao);
    outstream.close();
  }

  
}