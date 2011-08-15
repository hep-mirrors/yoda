// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2011 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_Bin1D_h
#define YODA_Bin1D_h

#include "YODA/Bin.h"
#include "YODA/Dbn1D.h"
#include <string>
#include <utility>
#include <cassert>

namespace YODA {


  /// @brief A generic 1D bin type
  ///
  /// This is a generic 1D bin type which supplies the accessors for the two "x"
  /// and "y" axis directions in which it is defined. Bin1D is not intended to be
  /// directly instantiated: it is inherited from to make specific histogram and
  /// profile bin types as HistoBin1D and ProfileBin1D.
  /// The lower bin edge is inclusive. This base class provides no fill
  /// method, since the signatures for standard and profile histos differ.
  class Bin1D : public Bin {
  public:

    /// @name Constructors
    //@{

    /// Init a new, empty bin with a pair of edges.
    Bin1D(double lowedge, double highedge)
      : _edges( std::make_pair(lowedge, highedge) )
    {
      assert( _edges.second > _edges.first );
    }


    /// Init a new, empty bin with a pair of edges.
    Bin1D(const std::pair<double,double>& edges)
      : _edges( edges )
    {
      assert( _edges.second >= _edges.first );
    }


    /// @brief Init a bin with all the components of a fill history.
    /// Mainly intended for internal persistency use.
    Bin1D(double lowedge, double highedge, const Dbn1D& dbnx)
      : _edges( std::make_pair(lowedge, highedge) ),
        _xdbn(dbnx)
    {
      assert( _edges.second >= _edges.first );
    }

    //@}


    /// @name Modifiers
    //@{

    /// Reset this bin
    virtual void reset() {
      _xdbn.reset();
    }

    /// Scale
    void scaleX(double factor) {
      _edges.first *= factor;
      _edges.second *= factor;
      _xdbn.scaleX(factor);
    }

    //@}


  public:

    /// @name X-axis info
    //@{

    /// Lower limit of the bin (inclusive).
    double lowEdge() const {
      return _edges.first;
    }
    /// Synonym for lowEdge
    double xMin() const {
      return lowEdge();
    }

    /// Upper limit of the bin (exclusive).
    double highEdge() const {
      return _edges.second;
    }
    /// Synonym for highEdge
    double xMax() const {
      return highEdge();
    }

    /// Get the {low,high} edges as an STL @c pair.
    std::pair<double,double> edges() const {
      return _edges;
    }

    /// Separation of low and high edges, i.e. high-low.
    double width() const {
      return _edges.second - _edges.first;
    }

    /// The mean position in the bin, or the midpoint if that is not available.
    double focus() const {
      if (_xdbn.sumW() != 0) {
        return xMean();
      } else {
        return midpoint();
      }
    }

    /// Geometric centre of the bin, i.e. high+low/2.0
    double midpoint() const {
      return ( _edges.second + _edges.first ) / 2;
    }

    //@}


  public:

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

    /// @name Raw x distribution statistics
    //@{

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

    //@}


  public:


    /// @name Operators
    //@{

    /// Add two bins
    Bin1D& operator += (const Bin1D& b) {
      return add(b);
    }

    /// Subtract one bin from another
    Bin1D& operator -= (const Bin1D& b) {
      return subtract(b);
    }

    //@}


  protected:

    /// @name Named operators
    //@{

    /// Add two bins (internal, explicitly named version)
    Bin1D& add(const Bin1D& b) {
      assert(_edges == b._edges);
      _xdbn += b._xdbn;
      return *this;
    }

    /// Subtract one bin from another (internal, explicitly named version)
    Bin1D& subtract(const Bin1D& b) {
      assert(_edges == b._edges);
      _xdbn -= b._xdbn;
      return *this;
    }

    //@}


  protected:

    /// The bin limits
    std::pair<double,double> _edges;

    // Distribution of weighted x values
    Dbn1D _xdbn;

  };


  /// Add two bins
  inline Bin1D operator + (const Bin1D& a, const Bin1D& b) {
    Bin1D rtn = a;
    rtn += b;
    return rtn;
  }


  /// Subtract one bin from another
  inline Bin1D operator - (const Bin1D& a, const Bin1D& b) {
    Bin1D rtn = a;
    rtn -= b;
    return rtn;
  }


  /// Bin1Ds are compared for axis sorting by lower edge position
  /// @todo Check for overlaps somewhere... on Axis1D, I guess.
  inline bool operator<(const Bin1D& a, const Bin1D& b) {
    return b.edges().first > a.edges().first;
  }


}



#endif
