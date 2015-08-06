// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2015 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_READERFLAT_H
#define YODA_READERFLAT_H

#include "YODA/AnalysisObject.h"
#include "YODA/Reader.h"
#include <YODA/Counter.h>
#include <YODA/Scatter1D.h>
#include <YODA/Scatter2D.h>
#include <YODA/Scatter3D.h>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace YODA {


  /// Persistency reader from YODA flat text data format.
  class ReaderFLAT : public Reader {
  public:

    /// Singleton creation function
    static Reader& create() {
      static ReaderFLAT _instance;
      return _instance;
    }

    void read(std::istream& stream, std::vector<AnalysisObject*>& aos);

  private:

    /// Private constructor, since it's a singleton.
    ReaderFLAT() { }

  };

}

#endif
