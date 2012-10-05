// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2012 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_Bin2D_h
#define YODA_Bin2D_h

#include "YODA/Bin.h"
#include "YODA/Dbn2D.h"
#include <string>
#include <utility>
#include <cassert>

namespace YODA {


  /// @brief A generic 2D bin type
  ///
  /// This is a generic 2D bin type which supplies the accessors for the two "x"
  /// and "y" axis directions in which it is defined. Bin2D is not intended to be
  /// directly instantiated: it is inherited from to make specific histogram and
  /// profile bin types as HistoBin2D and ProfileBin2D.
  /// The lower bin edges in x and y are inclusive. This base class provides no fill
  /// method, since the signatures for standard and profile histos differ.
  template <class DBN>
  class Bin2D : public Bin {
  public:

    /// @name Constructors
    //@{

    // /// Make a new, empty bin with two pairs of edges.
    // Bin2D(double xmin, double ymin, double xmax, double ymax)
    //   : _xedges( std::make_pair(xmin, xmax) ),
    //     _yedges( std::make_pair(ymin, ymax) )
    // {
    //   if (_xedges.second < _xedges.first) {
    //     throw RangeError("The bin x-edges are wrongly defined!");
    //   }
    //   if (_yedges.second < _yedges.first) {
    //     throw RangeError("The bin y-edges are wrongly defined!");
    //   }
    // }


   /// Make a new, empty bin with two pairs of edges.
    Bin2D(const std::pair<double, double>& xedges, const std::pair<double, double>& yedges)
      : _xedges(xedges), _yedges(yedges)
    {
      if (_xedges.second < _xedges.first) {
        throw RangeError("The bin x-edges are wrongly defined!");
      }
      if (_yedges.second < _yedges.first) {
        throw RangeError("The bin y-edges are wrongly defined!");
      }
    }


    /// @brief Make a bin with all the components of a fill history.
    ///
    /// Mainly intended for internal persistency use.
    Bin2D(const std::pair<double, double>& xedges,
          const std::pair<double, double>& yedges, const DBN& dbn)
      : _xedges(xedges), _yedges(yedges), _dbn(dbn)
    {
      if (_xedges.second < _xedges.first) {
        throw RangeError("The bin x-edges are wrongly defined!");
      }
      if (_yedges.second < _yedges.first) {
        throw RangeError("The bin y-edges are wrongly defined!");
      }
    }


    /// Copy constructor
    Bin2D(const Bin2D<DBN>& b)
      : _xedges(b._xedges),
        _yedges(b._yedges),
        _dbn(b._dbn)
    { }


    /// Copy assignment
    Bin2D<DBN>& operator = (const Bin2D<DBN>& b) {
      _xedges = b._xedges;
      _yedges = b._yedges;
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

    /// Scale the x and y coordinates and distributions.
    void scaleXY(double scaleX, double scaleY) {
      _xedges.first *= scaleX;
      _xedges.second *= scaleX;

      _yedges.first *= scaleY;
      _yedges.second *= scaleY;

      _dbn.scaleXY(scaleX, scaleY);
    }

    //@}


  public:

    /// @name X-axis info
    //@{

    /// Lower x limit of the bin (inclusive).
    double lowEdgeX() const {
      return _xedges.first;
    }
    /// Synonym for lowEdgeX()
    double xMin() const {
      return lowEdgeX();
    }

    /// Upper x limit of the bin (exclusive).
    double highEdgeX() const {
      return _xedges.second;
    }
    /// Synonym for highEdgeX()
    double xMax() const {
      return highEdgeX();
    }

    /// Get the {low,high} edges as an STL @c pair.
    std::pair<double,double> xedges() const {
      return _xedges;
    }

    /// Width of the bin in x
    double widthX() const {
      return xMax() - xMin();
    }



    /// Lower y limit of the bin (inclusive).
    double lowEdgeY() const {
      return _yedges.first;
    }
    /// Synonym for lowEdgeY()
    double yMin() const {
      return lowEdgeY();
    }

    /// Upper y limit of the bin (exclusive).
    double highEdgeY() const {
      return _yedges.second;
    }
    /// Synonym for highEdgeY()
    double yMax() const {
      return highEdgeY();
    }

    /// Get the {low,high} edges as an STL @c pair.
    std::pair<double,double> yedges() const {
      return _yedges;
    }

    /// Width of the bin in y
    double widthY() const {
      return yMax() - yMin();
    }

    /// The mean position in the bin, or the midpoint if that is not available.
    std::pair<double, double> focus() const {
      if (!isZero(sumW())) {
        return std::make_pair(xMean(), yMean());
      } else {
        return midpoint();
      }
    }

    /// Geometric centre of the bin, i.e. high+low/2.0
    std::pair<double, double> midpoint() const {
      return std::make_pair((xMax() + xMin())/2, (yMax() + yMin())/2);
    }

    //@}


  public:

    /// @name Distribution statistics
    //@{

    /// Mean value of x-values in the bin.
    double xMean() const {
      return _dbn.xMean();
    }

    /// Mean value of y-values in the bin.
    double yMean() const {
      return _dbn.yMean();
    }

    /// The variance of x-values in the bin.
    double xVariance() const {
      return _dbn.xVariance();
    }

    /// The variance of y-values in the bin.
    double yVariance() const {
      return _dbn.yVariance();
    }

    /// The standard deviation (spread) of x-values in the bin.
    double xStdDev() const {
      return _dbn.xStdDev();
    }

    /// The standard deviation (spread) of y-values in the bin.
    double yStdDev() const {
      return _dbn.yStdDev();
    }

    /// The standard error on the bin x focus.
    double xStdErr() const {
      return _dbn.xStdErr();
    }

    /// The standard error on the bin y focus.
    double yStdErr() const {
      return _dbn.yStdErr();
    }

    /// The x RMS in the bin.
    double xRMS() const {
      return _dbn.xRMS();
    }

    /// The y RMS in the bin.
    double yRMS() const {
      return _dbn.yRMS();
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

    /// The sum of y*weight
    double sumWY() const {
      return _dbn.sumWY();
    }

    /// The sum of x*y*weight
    double sumWXY() const {
      return _dbn.sumWXY();
    }

    /// The sum of x^2 * weight
    double sumWX2() const {
      return _dbn.sumWX2();
    }

    /// The sum of y^2 * weight
    double sumWY2() const {
      return _dbn.sumWY2();
    }

    //@}


  public:

    /// @name Operators
    //@{

    /// Add two bins
    Bin2D<DBN>& operator += (const Bin2D<DBN>& b) {
      return add(b);
    }

    /// Subtract one bin from another
    Bin2D<DBN>& operator -= (const Bin2D<DBN>& b) {
      return subtract(b);
    }

    //@}


    /// @name Named operators
    //@{

    /// Merge two adjacent bins
    // @TODO: We still need to add a merge method


    /// Add two bins (internal, explicitly named version)
    ///
    /// This operator is defined for adding two bins with equivalent binning.
    /// It cannot be used to merge two bins into one larger bin.
    Bin2D<DBN>& add(const Bin2D<DBN>& b) {
      if (_xedges != b._xedges || _yedges != b._yedges) {
        throw LogicError("Attempted to add two bins with different edges");
      }
      _dbn += b._dbn;
      return *this;
    }


    /// Subtract one bin from another (internal, explicitly named version)
    ///
    /// This operator is defined for subtracting two bins with equivalent binning.
    /// It cannot be used to merge two bins into one larger bin.
    Bin2D<DBN>& subtract(const Bin2D<DBN>& b) {
      if (_xedges != b._xedges || _yedges != b._yedges) {
        throw LogicError("Attempted to subtract two bins with different edges");
      }
      _dbn -= b._dbn;
      return *this;
    }

    //@}


  protected:

    /// The bin limits
    std::pair<double,double> _xedges;
    std::pair<double,double> _yedges;

    // Distribution of weighted x (and perhaps y) values
    DBN _dbn;

  };



  /// Add two bins
  ///
  /// This "add" operator is defined for adding two bins with equivalent binning.
  /// It cannot be used to merge two bins into one larger bin.
  template <class DBN>
  inline Bin2D<DBN> operator + (const Bin2D<DBN>& a, const Bin2D<DBN>& b) {
    Bin2D<DBN> rtn = a;
    rtn += b;
    return rtn;
  }


  /// Subtract one bin from another
  ///
  /// This "subtraction" operator is defined for subtracting two bins with equivalent binning.
  /// It cannot be used to merge two bins into one larger bin.
  template <class DBN>
  inline Bin2D<DBN> operator - (const Bin2D<DBN>& a, const Bin2D<DBN>& b) {
    Bin2D<DBN> rtn = a;
    rtn -= b;
    return rtn;
  }


  /// Bin2Ds are compared for axis sorting by lower edge position in first x and then y directions
  template <class DBN>
  inline bool operator<(const Bin2D<DBN>& a, const Bin2D<DBN>& b) {
    if (!fuzzyEquals(a.xedges().first, b.xedges().first))
      return b.xedges().first > a.xedges().first;
    return b.yedges().first > a.yedges().first;
  }



}



#endif
