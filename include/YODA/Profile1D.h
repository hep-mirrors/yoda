// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_Profile1D_h
#define YODA_Profile1D_h

#include "YODA/AnalysisObject.h"
#include "YODA/ProfileBin.h"
#include "YODA/Exception.h"
#include <vector>
#include <string>
#include <map>

namespace YODA {


  /// A  one-dimensional profile histogram.
  class Profile1D : public AnalysisObject {

  public:

    /// Enumerate the type of bins
    enum BinType { UNDERFLOWBIN, OVERFLOWBIN, VALIDBIN };


  public:
    /// @name Constructors
    //@{

    /// Constructor giving range and number of bins
    Profile1D(const std::string& path, const std::string& title,
              size_t nxbins,
              double xlower, double xupper);

    /// Constructor giving explicit bin edges
    /// For n bins, binedges.size() == n+1, the last
    /// one being the upper bound of the last bin
    Profile1D(const std::string& path, const std::string& title,
              const std::vector<double>& xbinedges);

    /// Constructor giving a vector of bins
    Profile1D(std::string path, std::string title,
              const std::vector<ProfileBin>& xbins);
    //@}
    
  public:
    /// @name Filling methods
    //@{

    /// Fill histo by value and weight
    void fill(double x, double y, double weight=1.0);

    /// Directly fill bin by bin index
    //void fillBin(size_t index, double y, double weight=1.0);
    //@}

  public:
    
    /// Reset the histogram: Keep the binning but
    /// set all bin contents and related quantities
    /// to zero
    void reset();

  public:
    /// @name Bin accessors
    //@{

    /// Access the bin vector
    const std::vector<ProfileBin>& bins() const;

    /// Access a bin by index
    const ProfileBin& bin(size_t index) const;

    /// @brief Access the underflow and overflow bins by type.
    /// Using the VALIDBIN enum value as an argument will throw an exception.
    const ProfileBin& bin(BinType binType) const;

    /// Access a bin by x-coordinate.
    const ProfileBin& binByCoord(double x) const;
    //@}
  
  public:
    /// @name Whole histo data
    //@{

    /// Get sum of weights in histo.
    double sumWeight() const;

    /// Get sum of squared weights in histo.
    double sumWeight2() const;

    //@}

  public:

    /// @name Adding and subtracting histograms
    //@{

    /// Add another histogram to this
    Histo1D& operator += (const Histo1D& toAdd);

    /// Subtract another histogram from this
    Histo1D& operator -= (const Histo1D& toSubtract);

    //@}

  private:

    /// @name Bin data
    //@{

    typedef std::vector<ProfileBin> Bins;

    /// The bins contained in this histogram
    Bins _bins;

    /// The underflow bin
    ProfileBin _underflow;

    /// The overflow bin
    ProfileBin _overflow;


  };


  /// @name Combining profile histos: global operators
  //@{

  /// Add two profile histograms
  Profile1D operator + (const Profile1D& first, const Profile1D& second);

  /// Subtract two profile histograms
  Profile1D operator - (const Profile1D& first, const Profile1D& second);

  //@}


}

#endif
