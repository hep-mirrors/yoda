// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2011 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_ProfileBin1D_h
#define YODA_ProfileBin1D_h

#include "YODA/Bin1D.h"
#include "YODA/Exceptions.h"

namespace YODA {


  /// @brief A Bin1D specialised for handling profile-type information
  ///
  /// This is a 1D bin type, which supports all the operations defined for
  /// a generic Bin1D object, but also supplies the specific member functions
  /// for profile-type data, as opposed to histogram-type. This means that
  /// extra internal distribution statistics are stored for the extra
  /// "y-direction" specified in the profile fill operation.
  class ProfileBin1D : public Bin1D {

    /// Profile1D is a friend to add/subtract bins
    friend class Profile1D;


  public:

    /// @name Constructors
    //@{

    /// Constructor giving bin low and high edges.
    ProfileBin1D(double lowedge, double highedge)
      : Bin1D(lowedge, highedge)
    { }

    /// Constructor giving bin low and high edges as a pair.
    ProfileBin1D(const std::pair<double,double>& edges)
      : Bin1D(edges)
    { }

    /// @brief Init a profile bin with all the components of a fill history.
    /// Mainly intended for internal persistency use.
    ProfileBin1D(double lowedge, double highedge,
                 const Dbn1D& dbnx, const Dbn1D& dbny)
      : Bin1D(lowedge, highedge, dbnx),
        _ydbn(dbny)
    {  }

    //@}


    /// @name Modifiers
    //@{

    /// Fill histo by x and y values and weight.
    void fill(double x, double d, double weight=1.0) {
      assert( _edges.first < _edges.second );
      assert( x >= _edges.first && x < _edges.second );
      _xdbn.fill(x, weight);
      _ydbn.fill(d, weight);
    }

    /// Fill histo with @a weight and y-value @c d at x = bin midpoint.
    void fillBin(double d, double weight=1.0) {
      fill(midpoint(), d, weight);
    }

    /// Reset the bin.
    void reset() {
      Bin1D::reset();
      _ydbn.reset();
    }

    /// Rescale as if all fill weights had been different by factor @a scalefactor.
    void scaleW(double scalefactor) {
      _xdbn.scaleW(scalefactor);
      _ydbn.scaleW(scalefactor);
    }

    //@}


  public:

    /// @name Bin content info
    //@{

    double mean() const {
      return _ydbn.mean();
    }

    double stdDev() const {
      return _ydbn.stdDev();
    }

    double variance() const {
      return _ydbn.variance();
    }

    double stdErr() const {
      return _ydbn.stdErr();
    }

    //@}


  public:

    /// Add two bins (for use by Profile1D).
    ProfileBin1D& operator += (const ProfileBin1D& toAdd) {
      return add(toAdd);
    }

    /// Subtract two bins
    ProfileBin1D& operator -= (const ProfileBin1D& toSubtract) {
      return subtract(toSubtract);
    }


  protected:

    /// Add two bins (internal, explicitly named version)
    ProfileBin1D& add(const ProfileBin1D& pb) {
      Bin1D::add(pb);
      _ydbn += pb._ydbn;
      return *this;
    }

    /// Subtract one bin from another (internal, explicitly named version)
    ProfileBin1D& subtract(const ProfileBin1D& pb) {
      Bin1D::subtract(pb);
      _ydbn -= pb._ydbn;
      return *this;
    }


  public:

    /// The sum of y*weight
    double sumWY() const {
      return _ydbn.sumWX();
    }

    /// The sum of y^2 * weight
    double sumWY2() const {
      return _ydbn.sumWX2();
    }


  private:

    // Distribution of weighted data values
    Dbn1D _ydbn;


  };


  inline ProfileBin1D operator + (const ProfileBin1D& a, const ProfileBin1D& b) {
    ProfileBin1D rtn(a);
    rtn += a;
    return rtn;
  }

  inline ProfileBin1D operator - (const ProfileBin1D& a, const ProfileBin1D& b) {
    ProfileBin1D rtn(a);
    rtn -= a;
    return rtn;
  }


}

#endif
