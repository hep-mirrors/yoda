// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2011 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_Reader_h
#define YODA_Reader_h

#include "YODA/AnalysisObject.h"
#include "YODA/Histo1D.h"
#include "YODA/Profile1D.h"
#include "YODA/Scatter2D.h"
#include <string>
#include <fstream>
#include <vector>

namespace YODA {


  /// Pure virtual base class for various output writers.
  class Reader {
  public:

    /// Virtual destructor
    virtual ~Reader() {}


    /// @name Reading multiple analysis objects,
    //@{

    /// Read in a collection of objects @a objs from output stream @a stream.
    void read(std::istream& stream, std::vector<AnalysisObject>& aos) {
      read(stream, aos);
    }

    /// Read in a collection of objects @a objs from file @a filename.
    void read(const std::string& filename, std::vector<AnalysisObject>& aos) {
      std::ifstream instream;
      instream.open(filename.c_str());
      read(instream, aos);
      instream.close();
    }

    //@}



  protected:
    // virtual void readGenericAO(std::istream& stream) = 0;
    // // virtual void readHisto(std::istream& stream, const Histo1D& h) = 0;
    // // virtual void readProfile(std::istream& stream, const Profile1D& p) = 0;
    // virtual void readScatter(std::istream& stream, const Scatter2D& p) = 0;

  };

}

#endif
