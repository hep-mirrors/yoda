// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2011 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_Profile1D_h
#define YODA_Profile1D_h

#include "YODA/AnalysisObject.h"
#include "YODA/ProfileBin.h"
#include "YODA/Axis.h"
#include "YODA/Exceptions.h"
#include <vector>
#include <string>
#include <map>

namespace YODA {


  /// A  one-dimensional profile histogram.
  class Profile1D : public AnalysisObject {
  public:

    /// @name Constructors
    //@{

    /// Constructor giving range and number of bins
    Profile1D(size_t nxbins, double xlower, double xupper,
              const std::string& title="",
              Binning binning=LINEAR)
      : AnalysisObject(title),
        _axis(nxbins, xlower, xupper, binning)
    { }

    /// Constructor giving explicit bin edges
    /// For n bins, binedges.size() == n+1, the last
    /// one being the upper bound of the last bin
    Profile1D(const std::vector<double>& xbinedges,
              const std::string& title="")
      : AnalysisObject(title),
        _axis(xbinedges)
    {  }

    /// Constructor giving a vector of bins
    Profile1D(const std::vector<ProfileBin>& xbins,
              const std::string& title="")
      : AnalysisObject(title),
        _axis(xbins)
    {  }


    //@}


    /// @name Persistency hooks
    //@{

    /// Get name of the analysis object type, for persisting
    std::string _aotype() const { return "Profile1D"; }

    /// Set the state of the profile object, for unpersisting
    /// @todo Need to set annotations (do that on AO), all-histo Dbns, and dbns for every bin. Delegate!
    // void _setstate() = 0;

    //@}


    /// @name Modifiers
    //@{

    /// Fill histo by value and weight
    void fill(double x, double y, double weight=1.0);

    /// @brief Reset the histogram
    /// Keep the binning but set all bin contents and related quantities to zero
    void reset() {
      _axis.reset();
    }

    /// Rescale as if all fill weights had been different by factor @a scalefactor.
    void scaleW(double scalefactor) {
      _axis.scaleW(scalefactor);
    }

    //@}


    /// @name Bin accessors
    //@{

    /// Access the bin vector
    /// @todo Convert to a sorted bin set/list
    std::vector<YODA::ProfileBin>& bins() {
      return _axis.bins();
    }

    /// Access the bin vector
    /// @todo Convert to a sorted bin set/list
    const std::vector<YODA::ProfileBin>& bins() const {
      return _axis.bins();
    }

    /// Access a bin by x-coordinate.
    ProfileBin& binByCoord(double x) {
      /// @todo If no bin, throw an exception?
      return _axis.binByCoord(x);
    }

    /// Access a bin by x-coordinate.
    const ProfileBin& binByCoord(double x) const {
      /// @todo If no bin, throw an exception?
      return _axis.binByCoord(x);
    }

    //@}


  public:

    /// @name Whole histo data
    //@{

    /// Get sum of weights in histo.
    double sumW() const;

    /// Get sum of squared weights in histo.
    double sumW2() const;

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
    Axis<ProfileBin> _axis;

    //@}

  };


  /// @name Combining profile histos: global operators
  //@{

  /// Add two profile histograms
  inline Profile1D operator + (const Profile1D& first, const Profile1D& second) {
    Profile1D tmp = first;
    tmp += second;
    return tmp;
  }

  /// Subtract two profile histograms
  inline Profile1D operator - (const Profile1D& first, const Profile1D& second) {
    Profile1D tmp = first;
    tmp -= second;
    return tmp;
  }

  //@}


}

#endif
