// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_ProfileBin_h
#define YODA_ProfileBin_h

#include "YODA/Exception.h"
#include <string>
#include <utility>

namespace YODA {


  /// A  one-dimensional profile histogram.
  class ProfileBin {

    /// Profile1D is a friend to add/subtract bins
    friend class Profile1D;

  public:
    /// @name Constructors
    //@{

    /// @name Constructor giving bin low and high edges.
    //@{
    ProfileBin(double lowedge, double highedge);
    ProfileBin(std::pair<double,double> edges);
    //@}
    
  private:

    /// Fill histo by value and weight.
    void fill(double x, double y, double weight=1.0);
    
    /// Reset the bin.
    void reset();


    /// Get sum of weights in histo.
    double sumWeight() const;

    /// Get sum of squared weights in histo.
    double sumWeight2() const;

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

    /// @name Bin content info
    //@{
    double meanY() const;

    double stdDevY() const;

    double varianceY() const;

    double stdErrY() const;
    //@}

  private:

    /// Add two bins (for use by Profile1D).
    ProfileBin& operator += (const ProfileBin&);

    /// Subtract two bins
    ProfileBin& operator -= (const ProfileBin&);

  private:

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

    /// The sum of y*weight
    double _sumYWeight;
    double sumYWeight() const;

    /// The sum of y^2 * weight
    double _sumY2Weight;
    double sumY2Weight() const;

  };

  /// @todo operator+ etc.

}

#endif
