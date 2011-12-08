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
  ///
  /// @todo It would also be nice to have an *untemplated* generic Bin1D interface
  template <class DBN>
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
    ///
    /// Mainly intended for internal persistency use.
    Bin1D(double lowedge, double highedge, const DBN& dbn)
      : _edges( std::make_pair(lowedge, highedge) ),
        _dbn(dbn)
    {
      assert( _edges.second >= _edges.first );
    }


    /// Copy constructor
    Bin1D(const Bin1D<DBN>& b)
      : _edges(b._edges),
        _dbn(b._dbn)
    { }


    /// Copy assignment
    Bin1D& operator = (const Bin1D<DBN>& b) {
      _edges = b._edges;
      _dbn = b._dbn;
      return *this;
    }

    //@}


    /// @name Modifiers
    //@{

    /// Reset this bin
    virtual void reset() {
      _dbn.reset();
    }

    /// Rescale as if all fill weights had been different by factor @a scalefactor
    void scaleW(double scalefactor) {
      _dbn.scaleW(scalefactor);
    }

    /// Scale the x dimension
    void scaleX(double factor) {
      _edges.first *= factor;
      _edges.second *= factor;
      _dbn.scaleX(factor);
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
      if (!isZero(sumW())) {
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
      return _dbn.xMean();
    }

    /// The variance of x-values in the bin.
    double xVariance() const {
      return _dbn.xVariance();
    }

    /// The standard deviation (spread) of x-values in the bin.
    double xStdDev() const {
      return _dbn.xStdDev();
    }

    /// The standard error on the bin focus.
    double xStdErr() const {
      return _dbn.xStdErr();
    }

    /// The x RMS in the bin.
    double xRMS() const {
      return _dbn.xRMS();
    }

    //@}


  public:

    /// @name Raw distribution statistics
    //@{

    /// The number of entries
    unsigned long numEntries() const {
      return _dbn.numEntries();
    }

    /// The effective number of entries
    unsigned long effNumEntries() const {
      return _dbn.effNumEntries();
    }

    /// The sum of weights
    double sumW() const {
      return _dbn.sumW();
    }

    /// The sum of weights squared
    double sumW2() const {
      return _dbn.sumW2();
    }

    /// The sum of x*weight
    double sumWX() const {
      return _dbn.sumWX();
    }

    /// The sum of x^2 * weight
    double sumWX2() const {
      return _dbn.sumWX2();
    }

    //@}


  public:

    /// @name Operators
    //@{

    /// Add two bins
    Bin1D<DBN>& operator += (const Bin1D<DBN>& b) {
      return add(b);
    }

    /// Subtract one bin from another
    Bin1D<DBN>& operator -= (const Bin1D<DBN>& b) {
      return subtract(b);
    }

    //@}


    /// @name Named operators
    //@{

    /// Merge two adjacent bins
    Bin1D<DBN>& merge(const Bin1D<DBN>& b) {
      if (fuzzyEquals(_edges.second, b._edges.first)) {
        _edges.second = b._edges.second;
      } else if (fuzzyEquals(_edges.second, b._edges.first)) {
        _edges.first = b._edges.first;
      } else {
        throw LogicError("Attempted to merge two non-adjacent bins");
      }
      // std::cout << "a " << _dbn.sumW() << std::endl;
      _dbn += b._dbn;
      // std::cout << "b " << _dbn.sumW() << std::endl;
      return *this;
    }


    /// Add two bins (internal, explicitly named version)
    ///
    /// This operator is defined for adding two bins with equivalent binning.
    /// It cannot be used to merge two bins into one larger bin.
    Bin1D<DBN>& add(const Bin1D<DBN>& b) {
      if (_edges != b._edges) {
        throw LogicError("Attempted to add two bins with different edges");
      }
      _dbn += b._dbn;
      return *this;
    }


    /// Subtract one bin from another (internal, explicitly named version)
    ///
    /// This operator is defined for subtracting two bins with equivalent binning.
    /// It cannot be used to merge two bins into one larger bin.
    Bin1D<DBN>& subtract(const Bin1D<DBN>& b) {
      if (_edges != b._edges) {
        throw LogicError("Attempted to add two bins with different edges");
      }
      _dbn -= b._dbn;
      return *this;
    }

    //@}


  protected:

    /// The bin limits
    std::pair<double,double> _edges;

    // Distribution of weighted x (and perhaps y) values
    DBN _dbn;

  };



  /// Add two bins
  ///
  /// This "add" operator is defined for adding two bins with equivalent binning.
  /// It cannot be used to merge two bins into one larger bin.
  template <class DBN>
  inline Bin1D<DBN> operator + (const Bin1D<DBN>& a, const Bin1D<DBN>& b) {
    Bin1D<DBN> rtn = a;
    rtn += b;
    return rtn;
  }


  /// Subtract one bin from another
  ///
  /// This "subtraction" operator is defined for subtracting two bins with equivalent binning.
  /// It cannot be used to merge two bins into one larger bin.
  template <class DBN>
  inline Bin1D<DBN> operator - (const Bin1D<DBN>& a, const Bin1D<DBN>& b) {
    Bin1D<DBN> rtn = a;
    rtn -= b;
    return rtn;
  }


  /// Bin1Ds are compared for axis sorting by lower edge position
  template <class DBN>
  inline bool operator<(const Bin1D<DBN>& a, const Bin1D<DBN>& b) {
    return b.edges().first > a.edges().first;
  }


}



#endif
