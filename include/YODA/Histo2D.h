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
#include "YODA/Axis2D.h"
#include "YODA/Scatter3D.h"
#include "YODA/Exceptions.h"
#include "YODA/Histo1D.h"
#include <vector>
#include <string>
#include <map>

namespace YODA {


  /// Convenience typedef
  typedef Axis2D<HistoBin2D> Histo2DAxis;


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
    Histo2D(const Histo2D& h, const std::string& path="");

    //@}


  public:

    /// @name Persistency hooks
    //@{

    /// Get name of the analysis object type, for persisting
    std::string _aotype() const { return "Histo2D"; }

    /// Set the state of the histo object, for unpersisting
    /// @todo Need to set annotations (do that on AO), all-histo Dbns, and dbns for every bin. Delegate!
    // void _setstate() = 0;

    //@}


    /// @name Modifiers
    //@{

    /// Fill histo by value and weight
    int fill(double x, double y, double weight=1.0);

    /// @brief Reset the histogram.
    /// Keep the binning but set all bin contents and related quantities to zero
    virtual void reset() {
      _axis.reset();
    }

    ///Check if is a grid:
    int isGriddy() {
        return _axis.isGriddy();
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

    //@}

  public:

    /// @name Bin accessors
    //@{

    /// Low edges of this histo's axis
    double lowEdgeX() const {
      return _axis.lowEdgeX();
    }

    double lowEdgeY() const {
        return _axis.lowEdgeY();
    }

    /// High edges of this histo's axis
    double highEdgeX() const {
      return _axis.highEdgeX();
    }

    double highEdgeY() const {
        return _axis.highEdgeY();
    }

    /// Access the bin vector
    /// @todo Actually, it's a Histo
    std::vector<YODA::HistoBin2D>& bins() {
      return _axis.bins();
    }

    /// Access the bin vector (const version)
    const std::vector<YODA::HistoBin2D>& bins() const {
      return _axis.bins();
    }

    /// Access a bin by index (non-const version)
    HistoBin2D& bin(size_t index) {
      return _axis.bins()[index];
    }

    /// Access a bin by index (const version)
    const HistoBin2D& bin(size_t index) const {
      return _axis.bins()[index];
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

    /// Underflow (const version)
    const Dbn2D& underflow() const {
        return _axis.underflow();
    }

    /// Return underflow (non-const version)
    Dbn2D& underflow() {
        return _axis.underflow();
    }

    /// Return overflow (const version)
    const Dbn2D& overflow() const {
        return _axis.overflow();
    }

    /// Return overflow (non-const version)
    Dbn2D& overflow() {
        return _axis.overflow();
    }

    /// Return a total number of bins in Histo(non-const version)
    unsigned int numBinsTotal() {
        return _axis.numBinsTotal();
    }

    const unsigned int numBinsTotal() const {
        return _axis.numBinsTotal();
    }

    /// Hash returner (non-const version)
    /// @todo This needs a typedef
   /* std::pair<Utils::cachedvector<pair<double,std::vector<pair<size_t, pair<double,double> > > > >,
              Utils::cachedvector<pair<double,std::vector<pair<size_t, pair<double,double> > > > > > getHash() {
        return _axis.getHash();
    }

    /// Hash returner (const version)
    /// @todo This needs a typedef
    const std::pair<Utils::cachedvector<pair<double,std::vector<pair<size_t, pair<double,double> > > > >,
                    Utils::cachedvector<pair<double,std::vector<pair<size_t, pair<double,double> > > > > > getHash() const {
        return _axis.getHash();
    }*/
    //@}

  public:

    /// @name Whole histo data
    //@{

    /// Get the total volume of the histogram
    // @todo This does not work!! (AB: Explain what you mean!)
    double integral(bool includeoverflows=true) const {
      return sumW(includeoverflows);
    }

    /// Get sum of weights in histo
    double sumW(bool includeoverflows=true) const;

    /// Get sum of squared weights in histo
    double sumW2(bool includeoverflows=true) const;

    /// Get the mean
    double xMean(bool includeoverflows=true) const;
    double yMean(bool includeoverflows=true) const;

    /// Get the variance
    double xVariance(bool includeoverflows=true) const;
    double yVariance(bool includeoverflows=true) const;

    /// Get the standard deviation
    double xStdDev(bool includeoverflows=true) const {
      return std::sqrt(xVariance(includeoverflows));
    }
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

    /*bool operator == (const Histo2D& other) {
      return _axis == other._axis;
    }

    bool operator != (const Histo2D& other) {
        return ! operator == (other);
    }
*/
    //@}


    /// @name Slicing operators
    //@{

    /// @brief Create a Histo1D for the bin slice parallel to the x axis at the specified y coordinate
    /// Note that the created histogram will not have correctly filled underflow and overflow bins.
    /// @todo It's not really *at* the specified y coord: it's for the corresponding bin row.
    /// @todo Need to check that there is a continuous row for this y
    /// @todo Change the name!
    Histo1D cutterX(double atY, const std::string& path="", const std::string& title="") {
      if (atY < lowEdgeY() || atY > highEdgeY()) throw RangeError("Y is outside the grid");
      vector<HistoBin1D> tempBins;
      /// @todo Make all Bin1D constructions happen in loop, to reduce code duplication
      const HistoBin2D& first = binByCoord(lowEdgeX(), atY);
      const Dbn1D dbn(first.numEntries(), first.sumW(), first.sumW2(), first.sumWX(), first.sumWX2());
      tempBins.push_back(HistoBin1D(first.lowEdgeX(), first.highEdgeX(), dbn));
      for (double i = first.xMax() + first.widthX()/2; i < highEdgeX(); i += first.widthX()) {
        const HistoBin2D& b2 = binByCoord(i, atY);
        const Dbn1D dbn2(b2.numEntries(), b2.sumW(), b2.sumW2(), b2.sumWX(), b2.sumWX2());
        tempBins.push_back(HistoBin1D(b2.lowEdgeX(), b2.highEdgeX(), dbn2));
      }
      /// @todo Think about the total, underflow and overflow distributions
      /// @todo Create total dbn from input bins
      return Histo1D(tempBins, Dbn1D(), Dbn1D(), Dbn1D(), path, title);
    }


    /// Create a Histo1D for the bin slice parallel to the y axis at the specified x coordinate
    /// Note that the created histogram will not have correctly filled underflow and overflow bins.
    /// @todo It's not really *at* the specified x coord: it's for the corresponding bin row.
    /// @todo Need to check that there is a continuous column for this x
    /// @todo Change the name!
    Histo1D cutterY(double atX, const std::string& path="", const std::string& title="") {
      if (atX < lowEdgeX() || atX > highEdgeX()) throw RangeError("X is outside the grid");
      vector<HistoBin1D> tempBins;
      /// @todo Make all Bin1D constructions happen in loop, to reduce code duplication
      const HistoBin2D& first = binByCoord(atX, lowEdgeY());
      const Dbn1D dbn(first.numEntries(), first.sumW(), first.sumW2(), first.sumWX(), first.sumWX2());
      tempBins.push_back(HistoBin1D(first.lowEdgeY(), first.highEdgeY(), dbn));
      for (double i = first.yMax() + first.widthY()/2; i < highEdgeY(); i += first.widthY()) {
        const HistoBin2D& b2 = binByCoord(atX, i);
        const Dbn1D dbn2(b2.numEntries(), b2.sumW(), b2.sumW2(), b2.sumWX(), b2.sumWX2());
        tempBins.push_back(HistoBin1D(b2.lowEdgeY(), b2.highEdgeY(), dbn2));
      }
      /// @todo Think about the total, underflow and overflow distributions
      /// @todo Create total dbn from input bins
      return Histo1D(tempBins, Dbn1D(), Dbn1D(), Dbn1D(), path, title);
    }


    /// @todo Create x-wise and y-wise conversions to Profile1D


    //@}



  private:

    /// @name Bin data
    //@{

    /// Definition of bin edges and contents
    Axis2D<HistoBin2D> _axis;

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

  //Scatter3D operator / (const Histo2D& numer, const Histo2D& denom);

  //@}


}

#endif
