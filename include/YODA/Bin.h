// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_Bin_h
#define YODA_Bin_h

#include "YODA/Dbn1D.h"
#include <string>
#include <utility>

namespace YODA {


  /// @brief Base class for bins in 1D normal and profile histograms.
  /// The lower bin edge is inclusive. This base class provides no fill 
  /// method, since the signatures for standard and profile histos differ.
  class Bin {

  public:

    /// @name Constructors, giving bin low and high edges.
    //@{
    Bin(double lowedge, double highedge);
    
    Bin(std::pair<double,double> edges);
    //@}
    
    /// Reset this bin
    void reset();
    
  public:

    /// @name X-axis info
    //@{
    /// Lower limit of the bin (inclusive).
    double lowEdge() const;

    /// Upper limit of the bin (exclusive).
    double highEdge() const;

    /// Get the {low,high} edges as an STL @c pair.
    std::pair<double,double> edges() const;

    /// Separation of low and high edges, i.e. high-low.
    double width() const;

    /// The mean position in the bin, or the midpoint if that is not available.
    double focus() const;

    /// Geometric centre of the bin, i.e. high+low/2.0
    double midpoint() const;
    //@}


    /// @name X distribution statistics
    //@{

    /// Mean value of x-values in the bin.
    double xMean() const {
      return _xdbn.mean();
    }

    /// The variance of x-values in the bin.
    double xVariance() const {
      return _xdbn.variance();
    }
    
    /// The standard deviation (spread) of x-values in the bin.
    double xStdDev() const {
      return _xdbn.stdDev();
    }

    /// The standard error on the bin focus. 
    double xStdError() const {
      return _xdbn.stdErr();
    }
    //@}

  public:

    /// The number of entries
    unsigned long numEntries() const {
      return _xdbn.numEntries();
    }

    /// The sum of weights
    double sumW() const {
      return _xdbn.sumW();
    }

    /// The sum of weights squared
    double sumW2() const {
      return _xdbn.sumW2();
    }

    /// The sum of x*weight
    double sumWX() const {
      return _xdbn.sumWX();
    }

    /// The sum of x^2 * weight
    double sumWX2() const {
      return _xdbn.sumWX2();
    }

  protected:

    /// The bin limits
    std::pair<double,double> _edges;

    // Distribution of weighted x values    
    Dbn1D _xdbn;

  };

}

#endif
