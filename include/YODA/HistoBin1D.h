// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2011 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_HistoBin1D_h
#define YODA_HistoBin1D_h

#include "YODA/Bin1D.h"
#include "YODA/Exceptions.h"
#include <cmath>
#include <cassert>

namespace YODA {


  /// @brief A Bin1D specialised for handling histogram-type information
  ///
  /// This is a 1D bin type, which supports all the operations defined for
  /// a generic Bin1D object, but also supplies the specific member functions
  /// for histogram-type data, as opposed to profile-type.
  class HistoBin1D : public Bin1D<Dbn1D> {

  public:

    /// @name Constructor giving bin low and high edges.
    //@{

    /// Init a new, empty bin with a pair of edges.
    HistoBin1D(double lowedge, double highedge)
      : Bin1D<Dbn1D>(lowedge, highedge)
    { }


    /// Init a new, empty bin with a pair of edges.
    HistoBin1D(const std::pair<double,double>& edges)
      : Bin1D<Dbn1D>(edges)
    { }


    /// @brief Init a bin with all the components of a fill history.
    ///
    /// Mainly intended for internal persistency use.
    HistoBin1D(double lowedge, double highedge, const Dbn1D& dbnx)
      : Bin1D<Dbn1D>(lowedge, highedge, dbnx)
    { }


    /// Copy constructor
    HistoBin1D(const HistoBin1D& hb)
      : Bin1D<Dbn1D>(hb)
    { }


    /// Copy assignment
    HistoBin1D& operator = (const HistoBin1D& hb) {
      Bin1D<Dbn1D>::operator=(hb);
      return *this;
    }

    //@}


  public:

    /// @name Modifiers
    //@{

    /// @brief Fill this bin with weight @a weight at position @a coord.
    void fill(double x, double weight=1.0) {
      assert( _edges.first < _edges.second );
      assert( x >= _edges.first && x < _edges.second );
      _dbn.fill(x, weight);
    }

    /// @brief Fill this bin with weight @a weight.
    void fillBin(double weight=1.0) {
      fill(midpoint(), weight);
    }

    //@}


  public:

    /// @name Bin content info
    //@{

    /// The area is the sum of weights in the bin, i.e. the
    /// width of the bin has no influence on this figure.
    double area() const {
      return sumW();
    }

    /// The height is defined as area/width.
    double height() const {
      return area() / width();
    }

    //@}


    /// @name Error info
    //@{

    /// Error computed using binomial statistics on the sum of bin weights,
    /// i.e. err_area = sqrt{sum{weights}}
    double areaErr() const {
      return sqrt(sumW2());
    }

    /// As for the height vs. area, the height error includes a scaling factor
    /// of the bin width, i.e. err_height = sqrt{sum{weights}} / width.
    double heightErr() const {
      return areaErr() / width();
    }

    //@}


  public:

    /// Add two bins (for use by Histo1D).
    HistoBin1D& operator += (const HistoBin1D& toAdd) {
      return add(toAdd);
    }

    /// Subtract two bins
    HistoBin1D& operator -= (const HistoBin1D& toSubtract) {
      return subtract(toSubtract);
    }


  protected:

    /// Add two bins (internal, explicitly named version)
    HistoBin1D& add(const HistoBin1D& hb) {
      Bin1D<Dbn1D>::add(hb);
      return *this;
    }

    /// Subtract one bin from another (internal, explicitly named version)
    HistoBin1D& subtract(const HistoBin1D& hb) {
      Bin1D<Dbn1D>::subtract(hb);
      return *this;
    }

  };


  /// Add two bins
  inline HistoBin1D operator + (const HistoBin1D& a, const HistoBin1D& b) {
    HistoBin1D rtn(a);
    rtn += a;
    return rtn;
  }

  /// Subtract two bins
  inline HistoBin1D operator - (const HistoBin1D& a, const HistoBin1D& b) {
    HistoBin1D rtn(a);
    rtn -= a;
    return rtn;
  }


}

#endif
