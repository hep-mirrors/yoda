// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2012 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_Plot_h
#define YODA_Plot_h

#include "YODA/AnalysisObject.h"

namespace YODA {


  /// A plot object: really just a handler for plot style annotations in data files.
  class Plot : public AnalysisObject {
  public:

    /// @name Constructors
    //@{

    /// Default constructor
    Plot()
      : AnalysisObject("Plot", "", "")
    { }

    //@}


    /// Make this class non-abstract
    void reset() {};

  };


}

#endif
