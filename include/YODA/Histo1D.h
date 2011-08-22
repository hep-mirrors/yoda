// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2011 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_Histo1D_h
#define YODA_Histo1D_h

#include "YODA/AnalysisObject.h"
#include "YODA/HistoBin1D.h"
#include "YODA/Dbn1D.h"
#include "YODA/Scatter2D.h"
#include "YODA/Axis1D.h"
#include "YODA/Exceptions.h"
#include <vector>
#include <string>
#include <map>

namespace YODA {


  /// Convenience typedef
  typedef Axis1D<HistoBin1D, Dbn1D> Histo1DAxis;


  /// A  one-dimensional histogram.
  class Histo1D : public AnalysisObject {

  public:

    /// Convenience typedefs
    typedef Histo1DAxis Axis;
    typedef Axis::Bins Bins;


    /// @name Constructors
    //@{

    /// Constructor giving range and number of bins.
    Histo1D(size_t nbins, double lower, double upper,
            const std::string& path="", const std::string& title="")
      : AnalysisObject("Histo1D", path, title),
        _axis(nbins, lower, upper)
    { }


    /// @brief Constructor giving explicit bin edges.
    ///
    /// For n bins, binedges.size() == n+1, the last one being the upper bound
    /// of the last bin
    Histo1D(const std::vector<double>& binedges,
            const std::string& path="", const std::string& title="")
      : AnalysisObject("Histo1D", path, title),
        _axis(binedges)
    { }


    /// Copy constructor with optional new path
    Histo1D(const Histo1D& h, const std::string& path="");


    /// Constructor from a Scatter2D's binning, with optional new path
    Histo1D(const Scatter2D& s, const std::string& path="");


    /// Constructor from a Profile1D's binning, with optional new path
    Histo1D(const Profile1D& p, const std::string& path="");


    /// @brief State-setting constructor
    ///
    /// Intended principally for internal persistency use.
    Histo1D(const std::vector<HistoBin1D>& bins,
            const Dbn1D& dbn_tot, const Dbn1D& dbn_uflow, const Dbn1D& dbn_oflow,
            const std::string& path="", const std::string& title="")
      : AnalysisObject("Histo1D", path, title),
        _axis(bins, dbn_tot, dbn_uflow, dbn_oflow)
    { }


    /// Assignment operator
    Histo1D& operator = (const Histo1D& h1) {
      setPath(h1.path());
      setTitle(h1.title());
      _axis = h1._axis;
      return *this;
    }

    //@}


  public:

    /// @name Modifiers
    //@{

    /// Fill histo by value and weight
    void fill(double x, double weight=1.0);

    /// @brief Reset the histogram.
    /// Keep the binning but set all bin contents and related quantities to zero
    virtual void reset() {
      _axis.reset();
    }

    /// Rescale as if all fill weights had been different by factor @a scalefactor.
    void scaleW(double scalefactor) {
      _axis.scaleW(scalefactor);
    }

    //@}


  public:

    /// @name Bin accessors
    //@{

    /// Number of bins on this axis (not counting under/overflow)
    size_t numBins() const {
      return bins().size();
    }

    /// Low edge of this histo's axis
    double lowEdge() const {
      return _axis.lowEdge();
    }

    /// High edge of this histo's axis
    double highEdge() const {
      return _axis.highEdge();
    }

    /// Access the bin vector
    /// @todo Actually, it's a Histo
    std::vector<YODA::HistoBin1D>& bins() {
      return _axis.bins();
    }

    /// Access the bin vector (const version)
    const std::vector<YODA::HistoBin1D>& bins() const {
      return _axis.bins();
    }

    /// Access a bin by index (non-const version)
    HistoBin1D& bin(size_t index) {
      return _axis.bins()[index];
    }

    /// Access a bin by index (const version)
    const HistoBin1D& bin(size_t index) const {
      return _axis.bins()[index];
    }

    /// Access a bin by coordinate (non-const version)
    HistoBin1D& binByCoord(double x) {
      return _axis.binByCoord(x);
    }

    /// Access a bin by coordinate (const version)
    const HistoBin1D& binByCoord(double x) const {
      return _axis.binByCoord(x);
    }

    /// Access underflow (non-const version)
    Dbn1D& underflow() {
      return _axis.underflow();
    }

    /// Access underflow (const version)
    const Dbn1D& underflow() const {
      return _axis.underflow();
    }

    /// Access overflow (non-const version)
    Dbn1D& overflow() {
      return _axis.overflow();
    }

    /// Access overflow (const version)
    const Dbn1D& overflow() const {
      return _axis.overflow();
    }

    //@}


  public:

    /// @name Whole histo data
    //@{

    /// Get the total area of the histogram
    double integral(bool includeoverflows=true) const {
      return sumW(includeoverflows);
    }

    /// Get the integrated area of the histogram between bins @a binindex1 and @a binindex2.
    double integral(size_t binindex1, size_t binindex2) const {
      assert(binindex1 > binindex2);
      if (binindex1 < 0 || binindex1 >= numBins()) throw RangeError("binindex1 is out of range");
      if (binindex2 < 0 || binindex2 >= numBins()) throw RangeError("binindex2 is out of range");
      double rtn = 0;
      for (size_t i = binindex1; i < binindex2; ++i) {
        rtn += bin(i).area();
      }
      return rtn;
    }

    /// Get sum of weights in histo
    double sumW(bool includeoverflows=true) const;

    /// Get sum of squared weights in histo
    double sumW2(bool includeoverflows=true) const;

    /// Get the mean
    double mean(bool includeoverflows=true) const;

    /// Get the variance
    double variance(bool includeoverflows=true) const;

    /// Get the standard deviation
    double stdDev(bool includeoverflows=true) const {
      return std::sqrt(variance(includeoverflows));
    }

    //@}


  public:

    /// @name Adding and subtracting histograms
    //@{

    /// Add another histogram to this
    Histo1D& operator += (const Histo1D& toAdd) {
      _axis += toAdd._axis;
      return *this;
    }

    /// Subtract another histogram from this
    Histo1D& operator -= (const Histo1D& toSubtract) {
      _axis -= toSubtract._axis;
      return *this;
    }

    //@}


  private:

    /// @name Bin data
    //@{

    /// Definition of bin edges and contents
    Axis1D<HistoBin1D, Dbn1D> _axis;

    //@}

  };


  /// @name Combining histos: global operators
  //@{

  /// Add two histograms
  inline Histo1D add(const Histo1D& first, const Histo1D& second) {
    Histo1D tmp = first;
    tmp += second;
    return tmp;
  }


  /// Add two histograms
  inline Histo1D operator + (const Histo1D& first, const Histo1D& second) {
    return add(first, second);
  }


  /// Subtract two histograms
  inline Histo1D subtract(const Histo1D& first, const Histo1D& second) {
    Histo1D tmp = first;
    tmp -= second;
    return tmp;
  }


  /// Subtract two histograms
  inline Histo1D operator - (const Histo1D& first, const Histo1D& second) {
    return subtract(first, second);
  }


  /// Divide two histograms
  ///
  /// The
  Scatter2D divide(const Histo1D& numer, const Histo1D& denom,
                   ErrorCombination erropt=QUAD);


  /// Divide two histograms, with an uncorrelated error treatment
  inline Scatter2D operator / (const Histo1D& numer, const Histo1D& denom) {
    return divide(numer, denom);
  }

  //@}


}

#endif
