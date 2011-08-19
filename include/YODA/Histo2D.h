// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2011 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_Histo2D_h
#define YODA_Histo2D_h

#include "YODA/AnalysisObject.h"
#include "YODA/HistoBin2D.h"
#include "YODA/HistoBin1D.h"
#include "YODA/Dbn2D.h"
#include "YODA/Axis2D.h"
#include "YODA/Profile1D.h"
#include "YODA/Exceptions.h"
#include "YODA/Histo1D.h"

#include <vector>

namespace YODA {


  // Forward declaration
  class Scatter3D;

  /// Convenience typedef
  typedef Axis2D<HistoBin2D, Dbn2D> Histo2DAxis;


  /// A  one-dimensional histogram.
  class Histo2D : public AnalysisObject {

  public:

    /// Convenience typedefs
    typedef Histo2DAxis Axis;
    typedef Axis::Bins Bins;


    /// @name Constructors
    //@{

    /// Constructor giving range and number of bins.
    Histo2D(size_t nbinsX, double lowerX, double upperX,
            size_t nbinsY, double lowerY, double upperY,
            const std::string& path="", const std::string& title="")
      : AnalysisObject("Histo2D", path, title),
        _axis(nbinsX, lowerX, upperX, nbinsY, lowerY, upperY)
    { }

    /// A default constructor. One needs to provide two points
    /// (top-right and bottom-left) for each rectangular bin that
    /// is created. It is assumed that the binedges vector is nonempty (why btw?).
    /// @todo Drop the pairings -- it is more natural, less contrived, and cleaner in code as xlow/high and ylow/high
    Histo2D(const std::vector<std::pair<std::pair<double,double>, pair<double,double> > >& binedges,
            const std::string& path="",
            const std::string& title="")
      : AnalysisObject("Histo2D", path, title),
      _axis(binedges)
    { }


    /// Copy constructor with optional new path
    Histo2D(const Histo2D& h, const std::string& path="")
      : AnalysisObject("Histo2D", (path.size() == 0) ? h.path() : path, h, h.title())
    {
      _axis = h._axis;
    }


    /// @todo Add binning constructors from Scatter3D (and Profile2D when it exists)


    /// @brief State-setting constructor
    /// Mainly intended for internal persistency use.
    Histo2D(const std::vector<std::pair<HistoBin2D, bool> >& bins,
            const std::vector<std::vector<Dbn2D> >& outflows,
            const Dbn2D& totalDbn,
            const std::string& path="", const std::string& title="")
      : AnalysisObject("Histo2D", path, title), _axis(bins, outflows, totalDbn)
    { }

    //@}


  public:


    /// @name Modifiers
    //@{

    /// @brief Fill histo by value and weight
    ///
    /// It relies on Axis2D for searching procedures and complains loudly if no
    /// bin was found.
    int fill(double x, double y, double weight=1.0);

    /// @brief Reset the histogram.
    /// Keep the binning but set all bin contents and related quantities to zero
    void reset() {
      _axis.reset();
    }

    /// Rescale as if all fill weights had been different by factor @a scalefactor.
    void scaleW(double scalefactor) {
      _axis.scaleW(scalefactor);
    }

    /// Scale the dimensions
    void scaleXY(double scaleX = 1.0, double scaleY = 1.0) {
      _axis.scaleXY(scaleX, scaleY);
    }

    /// Adding bins
    void addBin(const vector<pair<pair<double,double>, pair<double,double> > > coords) {
        _axis.addBin(coords);
    }

    /// Adding bins which is not so eloquent
    void addBin(double lowX, double lowY, double highX, double highY)   {
        _axis.addBin(lowX, lowY, highX, highY);
    }

    /// Merge the bins
    void mergeBins(size_t from, size_t to) {
      _axis.mergeBins(from, to);
    }

    //@}


  public:

    /// @name Bin accessors
    //@{

    /// Low x edge of this histo's axis
    double lowEdgeX() const {
      return _axis.lowEdgeX();
    }

    /// Low y edge of this histo's axis
    double lowEdgeY() const {
        return _axis.lowEdgeY();
    }

    /// High x edge of this histo's axis
    double highEdgeX() const {
      return _axis.highEdgeX();
    }

    /// High y edge of this histo's axis
    double highEdgeY() const {
        return _axis.highEdgeY();
    }

    /// Access the bin vector (non-const version)
    std::vector<std::pair<YODA::HistoBin2D, bool> >& bins() {
      return _axis.bins();
    }

    /// Access the bin vector (const version)
    const std::vector<std::pair<YODA::HistoBin2D, bool > >& bins() const {
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

    /// Return a total number of bins in Histo(non-const version)
    size_t numBinsTotal() {
      return _axis.numBinsTotal();
    }

    const size_t numBinsTotal() const {
      return _axis.numBinsTotal();
    }

    /// Return number of bins along X axis
    const size_t numBinsX() const {
      return _axis.numBinsX();
    }

    /// Return the number of bins along Y axis
    const size_t numBinsY() const{
      return _axis.numBinsY();
    }

    //@}


  public:

    /// @name Whole histo data
    //@{

    /// Get the total volume of the histogram
    // @todo This does not work!! (AB: Explain what you mean!)
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


    /// @name Slicing operators
    //@{

    /// @brief Create a Histo1D for the bin slice parallel to the x axis at the specified y coordinate
    ///
    /// Note that the created histogram will not have correctly filled underflow and overflow bins.
    /// @todo It's not really *at* the specified y coord: it's for the corresponding bin row.
    /// @todo Change the name!
    Histo1D cutterX(double atY, const std::string& path="", const std::string& title="") {
      if (!_axis.isGrid()) throw GridError("Attempt to cut a Histo2D that is not a grid!");

      if (atY < lowEdgeY() || atY > highEdgeY()) throw RangeError("Y is outside the grid");
      vector<HistoBin1D> tempBins;

      for (double i = binByCoord(lowEdgeX(), atY).xMin(); i < highEdgeX(); i += binByCoord(i, atY).widthX()) {
        const HistoBin2D& b2 = binByCoord(i, atY);
        const Dbn1D dbn2(b2.numEntries(), b2.sumW(), b2.sumW2(), b2.sumWX(), b2.sumWX2());
        tempBins.push_back(HistoBin1D(b2.lowEdgeX(), b2.highEdgeX(), dbn2));
      }

      /// Setting under/over flows
      Dbn2D underflow;
      underflow += _axis.outflows()[7][_axis.getBinRow(_axis.getBinIndex(lowEdgeX(), atY))];

      Dbn2D overflow;
      overflow += _axis.outflows()[3][_axis.getBinRow(_axis.getBinIndex(lowEdgeX(), atY))];

      return Histo1D(tempBins, _axis.totalDbn().transformX(), underflow.transformX(), overflow.transformX(), path, title);

    }


    /// @brief Create a Histo1D for the bin slice parallel to the y axis at the specified x coordinate
    ///
    /// Note that the created histogram will not have correctly filled underflow and overflow bins.
    /// @todo It's not really *at* the specified x coord: it's for the corresponding bin row.
    /// @todo Change the name!
    Histo1D cutterY(double atX, const std::string& path="", const std::string& title="") {
      if (!_axis.isGrid()) throw GridError("Attempt to cut a Histo2D that is not a grid!");

      if (atX < lowEdgeX() || atX > highEdgeX()) throw RangeError("X is outside the grid");
      vector<HistoBin1D> tempBins;

      for (double i = binByCoord(atX, lowEdgeY()).yMin(); i < highEdgeY(); i += binByCoord(atX, i).widthY()) {
        const HistoBin2D& b2 = binByCoord(atX, i);
        const Dbn1D dbn2(b2.numEntries(), b2.sumW(), b2.sumW2(), b2.sumWX(), b2.sumWX2());
        tempBins.push_back(HistoBin1D(b2.lowEdgeY(), b2.highEdgeY(), dbn2));
      }

      /// Setting under/over flows
      Dbn2D underflow;
      underflow += _axis.outflows()[1][_axis.getBinColumn(_axis.getBinIndex(atX, lowEdgeY()))];

      Dbn2D overflow;
      overflow += _axis.outflows()[5][_axis.getBinColumn(_axis.getBinIndex(atX, lowEdgeY()))];
      Dbn2D total = _axis.totalDbn();

      /// Making sure that we rotate our distributions, as we are cutting paralell to Y axis now
      total.flipXY();
      underflow.flipXY();
      overflow.flipXY();

      return Histo1D(tempBins, total.transformX(), underflow.transformX(), overflow.transformX(), path, title);
    }


    /// @brief X-wise Profile1D creator from Histo2D
    Profile1D mkProfileX() {
      if (!_axis.isGrid()) throw GridError("Profile1D cannot be made from a histogram that is not a grid!");

      vector<ProfileBin1D> prof;
      for(int i = lowEdgeX() + _axis.bin(0).midpoint().first; i < highEdgeX(); i+= _axis.bin(0).widthX()) {
        HistoBin2D& bin(_axis.binByCoord(i, lowEdgeY()));
        HistoBin2D composite(bin.xMin(), bin.xMax(), bin.yMin(), bin.yMax()) ;
        for(int j = lowEdgeY() + _axis.bin(0).midpoint().second; j < highEdgeY(); j += _axis.bin(0).widthY()) {
          composite += _axis.binByCoord(i, j);
        }
      prof.push_back(composite.transformX());
      }

      vector<vector<Dbn2D> >& outflows = _axis.outflows();

      /// Properly setting an underflow
      Dbn2D underflow;
      underflow += outflows[0][0]; underflow += outflows[6][0];
      for(size_t i = 0; i < outflows[7].size(); ++i) {
        underflow += outflows[7][i];
      }

      /// Setting an overflow
      Dbn2D overflow;
      overflow += outflows[2][0]; overflow += outflows[4][0];
      for(size_t i = 0; i < outflows[3].size(); ++i) {
        overflow += outflows[3][i];
      }

      /// And constructing a profile 1D from all this data
      Profile1D ret(prof, _axis.totalDbn(), underflow, overflow);
      return ret;

    }

    /// @brief Y-wise Profile1D creator from Histo2D
    Profile1D mkProfileY() {
      if (!_axis.isGrid()) throw GridError("Profile1D cannot be made from a histogram that is not a grid!");

      vector<ProfileBin1D> prof;
      for(int i = lowEdgeY() + _axis.bin(0).midpoint().second; i < highEdgeY(); i+= _axis.bin(0).widthY()) {
        HistoBin2D& bin(_axis.binByCoord(i, lowEdgeY()));
        HistoBin2D composite(bin.xMin(), bin.xMax(), bin.yMin(), bin.yMax()) ;
        for(int j = lowEdgeX() + _axis.bin(0).midpoint().first; j < highEdgeX(); j += _axis.bin(0).widthX()) {
          composite += _axis.binByCoord(i, j);
        }
      prof.push_back(composite.transformY());
      }

      vector<vector<Dbn2D> >& outflows = _axis.outflows();

      /// Properly setting an underflow
      Dbn2D underflow;
      underflow += outflows[0][0]; underflow += outflows[2][0];
      for(size_t i = 0; i < outflows[1].size(); ++i) {
        underflow += outflows[1][i];
      }

      /// Setting an overflow
      Dbn2D overflow;
      overflow += outflows[6][0]; overflow += outflows[4][0];
      for(size_t i = 0; i < outflows[5].size(); ++i) {
        overflow += outflows[5][i];
      }

      /// Setting a flipped total distribution
      Dbn2D td = _axis.totalDbn();
      td.flipXY();

      /// And constructing a profile 1D from all this data
      Profile1D ret(prof, td, underflow, overflow);
      return ret;

    }

    //@}



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
  inline Histo2D operator + (const Histo2D& first, const Histo2D& second) {
    Histo2D tmp = first;
    tmp += second;
    return tmp;
  }

  /// Subtract two histograms
  inline Histo2D operator - (const Histo2D& first, const Histo2D& second) {
    Histo2D tmp = first;
    tmp -= second;
    return tmp;
  }

  /// @todo Multiply histograms?

  /// @brief Divide two histograms
  ///
  /// Keep in mind that for the following to work, two Histos must
  /// be _exactly_ the same, including the ghost bins.
  Scatter3D operator / (const Histo2D& numer, const Histo2D& denom);

  //@}


}

#endif
