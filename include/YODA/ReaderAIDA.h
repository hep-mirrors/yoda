// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2011 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_READERAIDA_H
#define YODA_READERAIDA_H

#include "YODA/AnalysisObject.h"
#include "YODA/Reader.h"

namespace YODA {


  /// @brief Persistency reader for AIDA XML format.
  class ReaderAIDA : public Reader {
  public:

    /// Singleton creation function
    static Reader& create() {
      static ReaderAIDA _instance;
      return _instance;
    }


    // Include definitions of all read methods (all fulfilled by Reader::read(...))
    //#include "YODA/ReaderMethods.icc"


  protected:

    virtual void readGenericAO(std::istream& stream) = 0;
    // virtual void readHisto(std::istream& stream, const Histo1D& h) = 0;
    // virtual void readProfile(std::istream& stream, const Profile1D& p) = 0;
    virtual void readScatter(std::istream& stream, const Scatter2D& p) = 0;


  private:

    /// Private constructor, since it's a singleton.
    ReaderAIDA() { }

  };


}

#endif
