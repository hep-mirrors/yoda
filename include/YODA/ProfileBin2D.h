// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2013 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_ProfileBin2D_h
#define YODA_ProfileBin2D_h

#include "YODA/Bin2D.h"
#include "YODA/Dbn3D.h"
#include "YODA/Exceptions.h"

namespace YODA {


  /// @brief A Bin1D specialised for handling profile-type information
  ///
  /// This is a 1D bin type, which supports all the operations defined for
  /// a generic Bin1D object, but also supplies the specific member functions
  /// for profile-type data, as opposed to histogram-type. This means that
  /// extra internal distribution statistics are stored for the extra
  /// "y-direction" specified in the profile fill operation.
  class ProfileBin2D : public Bin2D<Dbn3D> {
  public:

    /// @name Constructors
    //@{

    /// Make a new, empty bin with two pairs of edges.
    ProfileBin2D(double xmin, double xmax, double ymin, double ymax)
      : Bin2D<Dbn3D>(std::make_pair(xmin, xmax), std::make_pair(ymin, ymax))
    { }

    /// Constructor accepting a set of all edges of a bin
    ProfileBin2D(const std::pair<double,double>& xedges,
                 const std::pair<double,double>& yedges)
      : Bin2D<Dbn3D>(xedges, yedges)
    { }

    /// @brief Make a bin with all the components of a fill history.
    ///
    /// Mainly intended for internal persistency use.
    ProfileBin2D(const std::pair<double, double>& xedges,
                 const std::pair<double, double>& yedges, const Dbn3D& dbn)
      : Bin2D<Dbn3D>(xedges, yedges, dbn)
    { }

    /// Copy constructor
    ProfileBin2D(const ProfileBin2D& pb)
      : Bin2D<Dbn3D>(pb)
    { }

    /// Copy assignment
    ProfileBin2D& operator = (const ProfileBin2D& pb) {
      Bin2D<Dbn3D>::operator=(pb);
      return *this;
    }

    //@}


    /// @name Modifiers
    //@{

    /// Fill by x, y, z values and weight
    void fill(double x, double y, double z, double weight=1.0) {
      assert(x >= xMin() && x <= xMax() && y >= yMin() && y <= yMax());
      _dbn.fill(x, y, z, weight);
    }

    /// A fill() function accepting the x,y coordinates as std::pair
    void fill(std::pair<double,double> coords, double z, double weight=1.0) {
      _dbn.fill(coords.first, coords.second, z, weight);
    }

    /// Fill the bin at the midpoint with a given z value
    void fillBin(double z, double weight=1.0){
      fill(midpoint(), z, weight);
    }

    /// A reset function
    void reset() {
      Bin2D<Dbn3D>::reset();
    }

    //@}

  public:

    /// @name Bin content info
    //@{

    double mean() const {
      return _dbn.zMean();
    }

    double stdDev() const {
      return _dbn.zStdDev();
    }

    double variance() const {
      return _dbn.zVariance();
    }

    double stdErr() const {
      return _dbn.zStdErr();
    }

    /// @todo Add relative error
    // /// The relative size of the error (same for either area or height errors)
    // double relErr() const {
    //   return sqrt(sumW2()) / sumW();
    // }

    double rms() const {
      return _dbn.zRMS();
    }

    //@}

    /// @name Raw z distribution statistics
    //@{

    ///@todo: Check if it is correct

    /// The sum of z*weight
    double sumWZ() const {
      return _dbn.sumWZ();
    }

    double sumWZ2() const {
      return _dbn.sumWZ2();
    }

    //@}

  public:

    /// Add two bins (for use by Profile2D)
    ProfileBin2D& operator += (const ProfileBin2D& toAdd) {
      return add(toAdd);
    }

    ProfileBin2D& operator -= (const ProfileBin2D& toSubtract) {
      return subtract(toSubtract);
    }

  protected:

    /// Add two bins
    ProfileBin2D& add(const ProfileBin2D& pb) {
      Bin2D<Dbn3D>::add(pb);
      return *this;
    }

    /// Subtract one bin from another
    ProfileBin2D& subtract(const ProfileBin2D& pb) {
      Bin2D<Dbn3D>::subtract(pb);
      return *this;
    }
  };


  /// Bin addition operator
  inline ProfileBin2D operator + (const ProfileBin2D& a, const ProfileBin2D& b) {
    ProfileBin2D rtn(a);
    rtn += b;
    return rtn;
  }


  /// Bin subtraction operator
  inline ProfileBin2D operator - (const ProfileBin2D& a, const ProfileBin2D& b) {
    ProfileBin2D rtn(a);
    rtn -= b;
    return rtn;
  }

}

#endif
