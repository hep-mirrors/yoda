// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2012 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_Histo2D_h
#define YODA_Histo2D_h

#include "YODA/AnalysisObject.h"
#include "YODA/HistoBin2D.h"
#include "YODA/Dbn2D.h"
#include "YODA/Axis2D.h"
#include "YODA/Scatter3D.h"
#include "YODA/Exceptions.h"

#include <vector>

namespace YODA {


  // Forward declaration
  class Profile2D;

  /// Convenience typedef
  typedef Axis2D<HistoBin2D, Dbn2D> Histo2DAxis;


  /// A two-dimensional histogram.
  class Histo2D : public AnalysisObject {
  public:

    /// Convenience typedefs
    typedef Histo2DAxis Axis;
    typedef Axis::Bins Bins;
    typedef HistoBin2D Bin;
    typedef Axis::Outflows Outflows;

    /// @name Constructors
    //@{

    /// Default constructor
    Histo2D(const std::string& path="", const std::string& title="")
      : AnalysisObject("Histo2D", path, title),
        _axis()
    { }


    /// Constructor giving range and number of bins.
    Histo2D(size_t nbinsX, double lowerX, double upperX,
            size_t nbinsY, double lowerY, double upperY,
            const std::string& path="", const std::string& title="")
      : AnalysisObject("Histo2D", path, title),
        _axis(nbinsX, std::make_pair(lowerX, upperX), nbinsY, std::make_pair(lowerY, upperY))
    { }


    /// Constructor accepting the bin edges on X and Y axis.
    Histo2D(const std::vector<double>& xedges, const std::vector<double>& yedges,
            const std::string& path="", const std::string& title="")
            : AnalysisObject("Histo2D", path, title),
            _axis(xedges, yedges)
    { }


    /// Constructor accepting an explicit collection of bins.
    Histo2D(const std::vector<Bin>& bins,
            const std::string& path="", const std::string& title="")
            : AnalysisObject("Histo2D", path, title),
            _axis(bins)
    { }


    /// Copy constructor with optional new path
    Histo2D(const Histo2D& h, const std::string& path="");


    /// @todo Add binning constructors from Scatter3D and Profile2D?

    // /// A constructor from a Scatter3D's binning, with optional new path
    // Histo2D(const Scatter3D& s, const std::string& path="");

    // /// Constructor from a Profile2D's binning, with optional new path
    // Histo2D(const Profile2D& h, const std::string& path="");


    /// @brief State-setting constructor
    ///
    /// Mainly intended for internal persistency use.
    Histo2D(const std::vector<HistoBin2D>& bins,
            const Dbn2D& totalDbn,
            const Outflows& outflows,
            const std::string& path="", const std::string& title="")
      : AnalysisObject("Histo2D", path, title),
        _axis(bins, totalDbn, outflows)
    { }

    //@}


  public:

    /// @name Modifiers
    //@{

    /// @brief Fill histo with weight at (x,y)
    void fill(double x, double y, double weight=1.0);

    /// @brief Reset the histogram.
    ///
    /// Keep the binning but set all bin contents and related quantities to zero
    void reset() {
      _axis.reset();
    }

    /// Rescale as if all fill weights had been different by factor @a scalefactor.
    void scaleW(double scalefactor) {
      setAnnotation("ScaledBy", annotation<double>("ScaledBy", 1.0) * scalefactor);
      _axis.scaleW(scalefactor);
    }


    /// Normalize the (visible) histo "volume" to the @a normto value.
    ///
    /// If @a includeoverflows is true, the original normalisation is computed with
    /// the overflow bins included, so that the resulting visible normalisation can
    /// be less than @a normto. This is probably what you want.
    void normalize(double normto=1.0, bool includeoverflows=true) {
      const double oldintegral = integral(includeoverflows);
      if (oldintegral == 0) throw WeightError("Attempted to normalize a histogram with null area");
      /// @todo Check that this is the desired behaviour
      scaleW(normto / oldintegral);
    }


    /// Scale the dimensions
    void scaleXY(double scaleX = 1.0, double scaleY = 1.0) {
      _axis.scaleXY(scaleX, scaleY);
    }

    // /// Adding bins
    /// @todo TODO
    // void addBin(const std::vector<std::pair<std::pair<double,double>, std::pair<double,double> > > coords) {
    //     _axis.addBin(coords);
    // }

    // /// Adding bins which is not so eloquent
    /// @todo TODO
    // void addBin(double lowX, double lowY, double highX, double highY)   {
    //     _axis.addBin(lowX, lowY, highX, highY);
    // }

    // /// Merge the bins
    /// @todo TODO
    // void mergeBins(size_t from, size_t to) {
    //   _axis.mergeBins(from, to);
    // }

    /// Rebin the whole histo by a @a factorX in the X direction and
    /// @a factorY in the Y direction
    /// @todo TODO
    // void rebin(size_t factorX, size_t factorY){
    //   _axis.rebin(factorX, factorY);
    // }

    void eraseBin(size_t index) {
      _axis.eraseBin(index);
    }

    //@}


  public:

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


    /// Access the bin vector (non-const version)
    std::vector<YODA::HistoBin2D>& bins() {
      return _axis.bins();
    }

    /// Access the bin vector (const version)
    const std::vector<YODA::HistoBin2D>& bins() const {
      return _axis.bins();
    }


    /// Access a bin by index (non-const version)
    HistoBin2D& bin(size_t index) {
      return _axis.bin(index);
    }

    /// Access a bin by index (const version)
    const HistoBin2D& bin(size_t index) const {
      return _axis.bin(index);
    }


    /// Access a bin by coordinate (non-const version)
    HistoBin2D& binByCoord(double x, double y) {
      return _axis.binByCoord(x, y);
    }

    /// Access a bin by coordinate (const version)
    const HistoBin2D& binByCoord(double x, double y) const {
      return _axis.binByCoord(x, y);
    }


    /// Return bin index (non-const version)
    int findBinIndex(double coordX, double coordY) {
      return _axis.getBinIndex(coordX, coordY);
    }

    /// Return bin index (const version)
    const int findBinIndex(double coordX, double coordY) const {
      return _axis.getBinIndex(coordX, coordY);
    }


    /// Number of bins
    const size_t numBins() const {
      return _axis.numBins();
    }

    /// Number of bins along the x axis
    const size_t numBinsX() const {
      return _axis.numBinsX();
    }

    /// Number of bins along the y axis
    const size_t numBinsY() const{
      return _axis.numBinsY();
    }


    /// Access summary distribution, including gaps and overflows (non-const version)
    Dbn2D& totalDbn() {
      return _axis.totalDbn();
    }

    /// Access summary distribution, including gaps and overflows (const version)
    const Dbn2D& totalDbn() const {
      return _axis.totalDbn();
    }


    /// @brief Access an outflow (non-const)
    ///
    /// Two indices are used, for x and y: -1 = underflow, 0 = in-range, and +1 = overflow.
    /// (0,0) is not a valid overflow index pair, since it is in range for both x and y.
    Dbn2D& outflow(int ix, int iy) {
      return _axis.outflow(ix, iy);
    }

    /// @brief Access an outflow (const)
    ///
    /// Two indices are used, for x and y: -1 = underflow, 0 = in-range, and +1 = overflow.
    /// (0,0) is not a valid overflow index pair, since it is in range for both x and y.
    const Dbn2D& outflow(int ix, int iy) const {
      return _axis.outflow(ix, iy);
    }

    //@}


  public:

    /// @name Whole histo data
    //@{

    /// Get the total volume of the histogram
    double integral(bool includeoverflows=true) const {
      return sumW(includeoverflows);
    }

    /// Get the sum of weights in histo
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

    /// Get the standard error in x
    double xStdErr(bool includeoverflows=true) const;

    /// Get the standard error in y
    double yStdErr(bool includeoverflows=true) const;

    /// Get the RMS in x
    double xRMS(bool includeoverflows=true) const;

    /// Get the RMS in y
    double yRMS(bool includeoverflows=true) const;

    //@}


  public:

    /// @name Adding and subtracting histograms
    //@{

    /// Add another histogram to this one
    Histo2D& operator += (const Histo2D& toAdd) {
      _axis += toAdd._axis;
      return *this;
    }

    /// Subtract another histogram from this one
    Histo2D& operator -= (const Histo2D& toSubtract) {
      _axis -= toSubtract._axis;
      return *this;
    }

    bool operator == (const Histo2D& other) const {
      return _axis == other._axis;
    }

    bool operator != (const Histo2D& other) const {
        return ! operator == (other);
    }

    //@}


    // /// @name Slicing operators
    // //@{

    // /// @brief Create a Histo1D for the bin slice parallel to the x axis at the specified y coordinate
    // ///
    // /// Note that the created histogram will not have correctly filled underflow and overflow bins.
    // /// @todo It's not really *at* the specified y coord: it's for the corresponding bin row.
    // /// @todo Change the name!
    // Histo1D cutterX(double atY, const std::string& path="", const std::string& title="");


    // /// @brief Create a Histo1D for the bin slice parallel to the y axis at the specified x coordinate
    // ///
    // /// Note that the created histogram will not have correctly filled underflow and overflow bins.
    // /// @todo It's not really *at* the specified x coord: it's for the corresponding bin row.
    // /// @todo Change the name!
    // Histo1D cutterY(double atX, const std::string& path="", const std::string& title="");


    // /// @brief X-wise Profile1D creator from Histo2D
    // Profile1D mkProfileX();

    // /// @brief Y-wise Profile1D creator from Histo2D
    // Profile1D mkProfileY();
    // //@}



  private:

    /// @name Bin data
    //@{

    /// Definition of bin edges and contents
    Axis2D<HistoBin2D, Dbn2D> _axis;

    //@}

  };


  /// @name Combining histos: global operators
  //@{

  /// Add two histograms
  inline Histo2D add(const Histo2D& first, const Histo2D& second) {
    Histo2D tmp = first;
    tmp += second;
    return tmp;
  }


  /// Add two histograms
  inline Histo2D operator + (const Histo2D& first, const Histo2D& second) {
    return add(first, second);
  }


  /// Subtract two histograms
  inline Histo2D subtract(const Histo2D& first, const Histo2D& second) {
    Histo2D tmp = first;
    tmp -= second;
    return tmp;
  }


  /// Subtract two histograms
  inline Histo2D operator - (const Histo2D& first, const Histo2D& second) {
    return subtract(first, second);
  }


  /// @todo Multiply histograms?


  /// @brief Divide two histograms
  ///
  /// Keep in mind that for the following to work, two Histos must
  /// be _exactly_ the same, including the ghost bins.
  Scatter3D divide(const Histo2D& numer, const Histo2D& denom);


  /// Divide two histograms, with an uncorrelated error treatment
  inline Scatter3D operator / (const Histo2D& numer, const Histo2D& denom) {
    return divide(numer, denom);
  }

  //@}


}

#endif
