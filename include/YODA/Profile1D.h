// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2011 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_Profile1D_h
#define YODA_Profile1D_h

#include "YODA/AnalysisObject.h"
#include "YODA/ProfileBin1D.h"
#include "YODA/Scatter2D.h"
#include "YODA/Dbn2D.h"
#include "YODA/Axis1D.h"
#include "YODA/Exceptions.h"
#include <vector>
#include <string>
#include <map>

namespace YODA {


  // Forward declarations
  class Histo1D;
  class Scatter2D;


  /// Convenience typedef
  typedef Axis1D<ProfileBin1D, Dbn2D> Profile1DAxis;


  /// A one-dimensional profile histogram.
  class Profile1D : public AnalysisObject {
  public:

    /// Convenience typedefs
    typedef Profile1DAxis Axis;
    typedef Axis::Bins Bins;


    /// @name Constructors
    //@{

    /// Constructor giving range and number of bins
    Profile1D(size_t nxbins, double xlower, double xupper,
              const std::string& path="", const std::string& title="")
      : AnalysisObject("Profile1D", path, title),
        _axis(nxbins, xlower, xupper)
    { }


    /// Constructor giving explicit bin edges
    ///
    /// For n bins, binedges.size() == n+1, the last one being the upper bound
    /// of the last bin
    Profile1D(const std::vector<double>& xbinedges,
              const std::string& path="", const std::string& title="")
      : AnalysisObject("Profile1D", path, title),
        _axis(xbinedges)
    { }


    /// Copy constructor with optional new path
    Profile1D(const Profile1D& p, const std::string& path="");


    /// Constructor from a Scatter2D's binning, with optional new path
    Profile1D(const Scatter2D& s, const std::string& path="");


    /// Constructor from a Histo1D's binning, with optional new path
    Profile1D(const Histo1D& h, const std::string& path="");


    /// @brief State-setting constructor.
    ///
    /// Intended principally for internal persistency use.
    Profile1D(const std::vector<ProfileBin1D>& bins,
              const Dbn2D& dbn_tot, const Dbn2D& dbn_uflow, const Dbn2D& dbn_oflow,
              const std::string& path="", const std::string& title="")
      : AnalysisObject("Profile1D", path, title),
        _axis(bins, dbn_tot, dbn_uflow, dbn_oflow)
    { }


    /// Assignment operator
    Profile1D& operator = (const Profile1D& p1) {
      setPath(p1.path());
      setTitle(p1.title());
      _axis = p1._axis;
      return *this;
    }

    //@}


    /// @name Persistency hooks
    //@{

    /// Get name of the analysis object type, for persisting
    std::string type() const { return "Profile1D"; }

    /// Set the state of the profile object, for unpersisting
    /// @todo Need to set annotations (do that on AO), all-histo Dbns, and dbns for every bin. Delegate!
    // void _setstate() = 0;

    //@}


    /// @name Modifiers
    //@{

    /// Fill histo by value and weight
    void fill(double x, double y, double weight=1.0);


    /// @brief Reset the histogram
    ///
    /// Keep the binning but set all bin contents and related quantities to zero
    void reset() {
      _axis.reset();
    }


    /// Rescale as if all fill weights had been different by factor @a scalefactor.
    void scaleW(double scalefactor) {
      _axis.scaleW(scalefactor);
    }


    /// Merge together the bin range with indices from @a from to @a to, inclusive
    void mergeBins(size_t from, size_t to) {
      _axis.mergeBins(from, to);
    }


    /// Merge every group of n bins, starting from the LHS
    void rebin(int n) {
      _axis.rebin(n);
    }

    //@}


    /// @name Bin accessors
    //@{

    /// Number of bins on this axis (not counting under/overflow)
    size_t numBins() const {
      return bins().size();
    }


    /// Access the bin vector
    std::vector<YODA::ProfileBin1D>& bins() {
      return _axis.bins();
    }

    /// Access the bin vector
    const std::vector<YODA::ProfileBin1D>& bins() const {
      return _axis.bins();
    }


    /// Access a bin by index (non-const version)
    ProfileBin1D& bin(size_t index) {
      return _axis.bins()[index];
    }

    /// Access a bin by index (const version)
    const ProfileBin1D& bin(size_t index) const {
      return _axis.bins()[index];
    }


    /// Access a bin by x-coordinate.
    ProfileBin1D& binByCoord(double x) {
      return _axis.binByCoord(x);
    }

    /// Access a bin by x-coordinate.
    const ProfileBin1D& binByCoord(double x) const {
      return _axis.binByCoord(x);
    }


    /// Access underflow (non-const version)
    Dbn2D& underflow() {
      return _axis.underflow();
    }

    /// Access underflow (const version)
    const Dbn2D& underflow() const {
      return _axis.underflow();
    }


    /// Access overflow (non-const version)
    Dbn2D& overflow() {
      return _axis.overflow();
    }

    /// Access overflow (const version)
    const Dbn2D& overflow() const {
      return _axis.overflow();
    }

    //@}


  public:

    /// @name Whole histo data
    //@{

    /// @todo Add integrals? Or are they too ambiguous to make a core function?

    /// Get sum of weights in histo.
    double sumW(bool includeoverflows=true) const;

    /// Get sum of squared weights in histo.
    double sumW2(bool includeoverflows=true) const;

    //@}


  public:

    /// @name Adding and subtracting histograms
    //@{

    /// Add another histogram to this
    Profile1D& operator += (const Profile1D& toAdd) {
      _axis += toAdd._axis;
      return *this;
    }

    /// Subtract another histogram from this
    Profile1D& operator -= (const Profile1D& toSubtract) {
      _axis -= toSubtract._axis;
      return *this;
    }

    //@}


  private:

    /// @name Bin data
    //@{

    /// The bins contained in this profile histogram
    Axis1D<ProfileBin1D, Dbn2D> _axis;

    //@}

  };


  /// @name Combining profile histos: global operators
  //@{

  /// Add two profile histograms
  inline Profile1D add(const Profile1D& first, const Profile1D& second) {
    Profile1D tmp = first;
    tmp += second;
    return tmp;
  }


  /// Add two profile histograms
  inline Profile1D operator + (const Profile1D& first, const Profile1D& second) {
    return add(first, second);
  }


  /// Subtract two profile histograms
  inline Profile1D subtract(const Profile1D& first, const Profile1D& second) {
    Profile1D tmp = first;
    tmp -= second;
    return tmp;
  }


  /// Subtract two profile histograms
  inline Profile1D operator - (const Profile1D& first, const Profile1D& second) {
    return subtract(first, second);
  }


  /// Divide two profile histograms
  Scatter2D divide(const Profile1D& numer, const Profile1D& denom);


  /// Divide two profile histograms
  inline Scatter2D operator / (const Profile1D& numer, const Profile1D& denom) {
    return divide(numer, denom);
  }

  //@}


}

#endif
