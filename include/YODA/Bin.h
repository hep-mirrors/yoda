// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_Bin_h
#define YODA_Bin_h

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

    /// The mean position in the bin.
    double focus() const;

    /// Geometric centre of the bin, i.e. high+low/2.0
    double midpoint() const;
    //@}


    /// @name X error info
    //@{
    /// The variance of x-values in the bin.
    double xVariance() const;

    /// The standard deviation (spread) of x-values in the bin.
    double xStdDev() const;

    /// The standard error on the bin focus. 
    double xStdError() const;
    //@}

    public:

  protected:

    /// The bin limits
    std::pair<double,double> _edges;

    /// The number of entries
    unsigned long _numEntries;

    /// The sum of weights
    double _sumWeight;
    double sumWeight() const;

    /// The sum of weights squared
    double _sumWeight2;
    double sumWeight2() const;

    /// The sum of x*weight
    double _sumXWeight;
    double sumXWeight() const;

    /// The sum of x^2 * weight
    double _sumX2Weight;
    double sumX2Weight() const;

  };

}

#endif
