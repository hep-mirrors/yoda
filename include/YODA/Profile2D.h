// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2013 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_Profile2D_h
#define YODA_Profile2D_h

#include "YODA/AnalysisObject.h"
#include "YODA/ProfileBin2D.h"
#include "YODA/Dbn3D.h"
#include "YODA/Axis2D.h"
#include "YODA/Scatter3D.h"
#include "YODA/Exceptions.h"

#include <vector>

namespace YODA {


  // Forward declarations
  class Histo2D;
  class Scatter3D;

  /// Convenience typedef
  typedef Axis2D<ProfileBin2D, Dbn3D> Profile2DAxis;


  /// A two-dimensional profile histogram.
  class Profile2D : public AnalysisObject {
  public:

    /// Convenience typedefs
    typedef Profile2DAxis Axis;
    typedef Axis::Bins Bins;
    typedef ProfileBin2D Bin;
    typedef Axis::Outflows Outflows;


    /// @name Constructors
    //@{

    /// Default constructor
    Profile2D(const std::string& path="", const std::string& title="")
      : AnalysisObject("Profile2D", path, title),
        _axis()
    { }


    /// Constructor giving range and number of bins
    Profile2D(size_t nbinsX, double lowerX, double upperX,
              size_t nbinsY, double lowerY, double upperY,
              const std::string& path="", const std::string& title="")
      : AnalysisObject("Profile2D", path, title),
        _axis(nbinsX, std::make_pair(lowerX, upperX), nbinsY, std::make_pair(lowerY, upperY))
    { }


    /// Constructor giving explicit bin edges in the direction of X and Y
    Profile2D(const std::vector<double>& xedges, const std::vector<double>& yedges,
              const std::string& path="", const std::string& title="")
      : AnalysisObject("Profile2D", path, title),
        _axis(xedges, yedges)
    { }


    /// Constructor accepting an explicit collection of bins.
    Profile2D(const std::vector<Bin>& bins,
              const std::string& path="", const std::string& title="")
      : AnalysisObject("Profile2D", path, title),
        _axis(bins)
    { }


    /// A copy constructor with optional new path
    Profile2D(const Profile2D& p, const std::string& path="");

    /// A constructor from a Scatter3D's binning, with optional new path
    Profile2D(const Scatter3D& s, const std::string& path="");

    /// Constructor from a Histo2D's binning, with optional new path
    Profile2D(const Histo2D& h, const std::string& path="");

    /// A state setting constructor
    Profile2D(const std::vector<ProfileBin2D>& bins,
              const Dbn3D& totalDbn,
              const Outflows& outflows,
              const std::string& path="", const std::string& title="")
      : AnalysisObject("Profile2D", path, title),
        _axis(bins, totalDbn, outflows)
    { }


    /// Assignment operator
    Profile2D& operator = (const Profile2D& p2) {
      AnalysisObject::operator = (p2); //< AO treatment of paths etc.
      _axis = p2._axis;
      return *this;
    }

    /// Make a copy on the stack
    Profile2D clone() const {
      return Profile2D(*this);
    }

    /// Make a copy on the heap, via 'new'
    Profile2D* newclone() const {
      return new Profile2D(*this);
    }

    //@}


    /// @name Persistency hooks
    //@{

    /// Get name of the analysis object type for persisting
    std::string type() const { return "Profile2D"; }

    //@}


    /// @name Modifiers
    //@{

    /// Fill histo by value and weight
    void fill(double x, double y, double z, double weight=1.0);

    /// Fill histo x-y bin i with the given z value and weight
    void fillBin(size_t i, double z, double weight=1.0);


    /// @brief Reset the histogram
    ///
    /// Keep the binning but reset the statistics
    void reset() {
      _axis.reset();
    }

    /// Rescale as if all fill weights had been different by a @a scalefactor
    void scaleW(double scalefactor) {
      /// @todo Is this ScaledBy annotation needed?
      setAnnotation("ScaledBy", annotation<double>("ScaledBy", 1.0) * scalefactor);
      _axis.scaleW(scalefactor);
    }

    /// @todo TODO
    // /// Merge together the bin range with indices from @a from to @a to, inclusive
    // void mergeBins(size_t from, size_t to) {
    //   _axis.mergeBins(from, to);
    // }

    /// @todo TODO
    // /// Merge every group of n bins, starting from the LHS
    // void rebin(size_t n) {
    //   throw "IMPLEMENT!";
    //   //_axis.rebin(n);
    // }


    // /// @brief Bin addition operator
    // ///
    // /// Add a bin to an axis described by its x and y ranges.
    void addBin(Axis::EdgePair1D xrange, Axis::EdgePair1D yrange) {
       _axis.addBin(xrange, yrange);
     }


    /// @brief Bins addition operator
    ///
    /// Add multiple bins from edge cuts without resetting
    void addBins(const Axis::EdgeCuts &xcuts, const Axis::EdgeCuts &ycuts) {
      _axis.addBins(xcuts, ycuts);
    }


    /// @brief Bins addition operator
    ///
    /// Add multiple bins without resetting
    void addBins(const Bins &bins) {
      _axis.addBins(bins);
    }


    /// @todo TODO
    // /// @brief Bin addition operator
    // ///
    // /// Add a set of bins delimiting coordinates of which are contained
    // /// in binLimits vector.
    // void addBin(const std::vector<Segment>& binLimits) {
    //   _axis.addBin(binLimits);
    // }

    void eraseBin(size_t index) {
      _axis.eraseBin(index);
    }

    //@}


    /// @name Bin accessors
    //@{

    /// Low x edge of this histo's axis
    double lowEdgeX() const {
      return _axis.lowEdgeX();
    }
    /// Alias for lowEdgeX()
    double xMin() const {
      return lowEdgeX();
    }


    /// Low y edge of this histo's axis
    double lowEdgeY() const {
        return _axis.lowEdgeY();
    }
    /// Alias for lowEdgeY()
    double yMin() const {
      return lowEdgeY();
    }


    /// High x edge of this histo's axis
    double highEdgeX() const {
      return _axis.highEdgeX();
    }
    /// Alias for highEdgeX()
    double xMax() const {
      return highEdgeX();
    }


    /// High y edge of this histo's axis
    double highEdgeY() const {
        return _axis.highEdgeY();
    }
    /// Alias for highEdgeY()
    double yMax() const {
      return highEdgeY();
    }


    /// Access the bin vector (non-const)
    std::vector<YODA::ProfileBin2D>& bins() {
      return _axis.bins();
    }

    /// Access the bin vector (const)
    const std::vector<YODA::ProfileBin2D>& bins() const {
      return _axis.bins();
    }


    /// Access a bin by index (non-const)
    ProfileBin2D& bin(size_t index) {
      return _axis.bins()[index];
    }

    /// Access a bin by index (const)
    const ProfileBin2D& bin(size_t index) const {
      return _axis.bins()[index];
    }

    /// Access a bin index by coordinate
    int binIndexAt(double x, double y) {
      return _axis.binIndexAt(x, y);
    }

    /// Access a bin by coordinate (non-const)
    ProfileBin2D& binAt(double x, double y) {
      return _axis.binAt(x, y);
    }

    /// Access a bin by coordinate (const)
    const ProfileBin2D& binAt(double x, double y) const {
      return _axis.binAt(x, y);
    }


    /// Number of bins of this axis (not counting under/over flow)
    size_t numBins() const {
      return _axis.bins().size();
    }

    /// Number of bins along the x axis
    size_t numBinsX() const {
      return _axis.numBinsX();
    }

    /// Number of bins along the y axis
    size_t numBinsY() const{
      return _axis.numBinsY();
    }


    /// Access summary distribution, including gaps and overflows (non-const version)
    Dbn3D& totalDbn() {
      return _axis.totalDbn();
    }

    /// Access summary distribution, including gaps and overflows (const version)
    const Dbn3D& totalDbn() const {
      return _axis.totalDbn();
    }


    /// @brief Access an outflow (non-const)
    ///
    /// Two indices are used, for x and y: -1 = underflow, 0 = in-range, and +1 = overflow.
    /// (0,0) is not a valid overflow index pair, since it is in range for both x and y.
    Dbn3D& outflow(int ix, int iy) {
      return _axis.outflow(ix, iy);
    }

    /// @brief Access an outflow (const)
    ///
    /// Two indices are used, for x and y: -1 = underflow, 0 = in-range, and +1 = overflow.
    /// (0,0) is not a valid overflow index pair, since it is in range for both x and y.
    const Dbn3D& outflow(int ix, int iy) const {
      return _axis.outflow(ix, iy);
    }

    //@}


  public:

    /// @name Whole histo data
    //@{

    /// Get the number of fills
    double numEntries() const { return totalDbn().numEntries(); }

    /// Get the effective number of fills
    double effNumEntries() const { return totalDbn().effNumEntries(); }

    /// Get sum of weights in histo
    double sumW(bool includeoverflows=true) const;

    /// Get the sum of squared weights in histo
    double sumW2(bool includeoverflows=true) const;

    /// Get the mean x
    double xMean(bool includeoverflows=true) const;

    /// Get the mean y
    double yMean(bool includeoverflows=true) const;

    /// Get the variance in x
    double xVariance(bool includeoverflows=true) const;

    /// Get the variance in y
    double yVariance(bool includeoverflows=true) const;

    /// Get the standard deviation in x
    double xStdDev(bool includeoverflows=true) const {
      return std::sqrt(xVariance(includeoverflows));
    }

    /// Get the standard deviation in y
    double yStdDev(bool includeoverflows=true) const {
      return std::sqrt(yVariance(includeoverflows));
    }

    /// Get the standard error on <x>
    double xStdErr(bool includeoverflows=true) const;

    /// Get the standard error on <y>
    double yStdErr(bool includeoverflows=true) const;

    /// @todo TODO
    // /// Get the RMS in x
    // double xRMS(bool includeoverflows=true) const;

    /// @todo TODO
    // /// Get the RMS in y
    // double yRMS(bool includeoverflows=true) const;

    //@}

  public:

    /// @name Adding and subtracting histograms
    //@{

    /// Add another profile to this one
    Profile2D& operator += (const Profile2D& toAdd) {
      if (hasAnnotation("ScaledBy")) rmAnnotation("ScaledBy");
      _axis += toAdd._axis;
      return *this;
    }

    /// Subtract another profile from this one
    Profile2D& operator -= (const Profile2D& toSubtract) {
      if (hasAnnotation("ScaledBy")) rmAnnotation("ScaledBy");
      _axis -= toSubtract._axis;
      return *this;
    }

    inline bool operator == (const Profile2D& other){
      return _axis == other._axis;
    }

    inline bool operator != (const Profile2D& other){
      return ! operator == (other);
    }
    //@}-

  private:

    /// @name Bin data
    //@{

    /// The bins contained in this profile histogram
    Axis2D<ProfileBin2D, Dbn3D> _axis;

    //@}
  };


  /// @name Combining profile histos: global operators
  //@{

  /// Add two profile histograms
  inline Profile2D add(const Profile2D& first, const Profile2D& second) {
    Profile2D tmp = first;
    if (first.path() != second.path()) tmp.setPath("");
    tmp += second;
    return tmp;
  }

  /// Add two profile histograms
  inline Profile2D operator + (const Profile2D& first, const Profile2D& second) {
    return add(first,second);
  }

  /// Subtract two profile histograms
  inline Profile2D subtract(const Profile2D& first, const Profile2D& second) {
    Profile2D tmp = first;
    if (first.path() != second.path()) tmp.setPath("");
    tmp -= second;
    return tmp;
  }

  /// Subtract two profile histograms
  inline Profile2D operator - (const Profile2D& first, const Profile2D& second) {
    return subtract(first,second);
  }

  /// Divide two profile histograms
  Scatter3D divide(const Profile2D& numer, const Profile2D& denom);

  /// Divide two profile histograms
  inline Scatter3D operator / (const Profile2D& numer, const Profile2D& denom) {
    return divide(numer, denom);
  }

  //@}

}

#endif
