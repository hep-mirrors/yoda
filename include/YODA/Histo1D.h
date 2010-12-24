// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2010 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_Histo1D_h
#define YODA_Histo1D_h

#include "YODA/AnalysisObject.h"
#include "YODA/HistoBin.h"
#include "YODA/Axis.h"
#include "YODA/Exception.h"
#include <vector>
#include <string>
#include <map>

namespace YODA {


  /// A  one-dimensional histogram.
  class Histo1D : public AnalysisObject {

  public:
    /// @name Constructors
    //@{

    /// Constructor giving range and number of bins.
    Histo1D(size_t nbins, double lower, double upper,
            const std::string& title="",
            Binning binning=LINEAR)
    : AnalysisObject(title),
      _axis(nbins, lower, upper, binning)
    { }

    /// @brief Constructor giving explicit bin edges.
    /// For n bins, binedges.size() == n+1, the last
    /// one being the upper bound of the last bin
    Histo1D(const std::vector<double>& binedges,
            const std::string& title="")
      : AnalysisObject(title),
        _axis(binedges)
    { }

    /// Constructor giving a vector of bins.
    /// @todo Allow any iterable of bins (use Boost::Range?)
    Histo1D(const std::vector<HistoBin>& bins,
            const std::string& title="")
      : AnalysisObject(title),
        _axis(bins)
    { }

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

    /// Access the bin vector
    std::vector<YODA::HistoBin>& bins() {
      return _axis.bins();
    }

    /// Access the bin vector (const version)
    const std::vector<YODA::HistoBin>& bins() const {
      return _axis.bins();
    }

    /// Access a bin by coordinate (non-const version)
    HistoBin& binByCoord(double x) {
      return _axis.binByCoord(x);
    }

    /// Access a bin by coordinate (const version)
    const HistoBin& binByCoord(double x) const {
      return _axis.binByCoord(x);
    }

    //@}


  public:

    /// @name Whole histo data
    //@{

    /// Get the total area
    /// @deprecated Use integral() instead
    double area() const {
      return integral();
    }

    /// Get the total area of the histogram
    /// @todo Differentiate between using all fills, and only fills which landed in bins
    double integral() const {
      return sumW();
    }

    /// Get sum of weights in histo (same as area)
    /// @todo Really same as area? Aren't heights sqrt(sum(w**2))?
    /// @todo Differentiate between using all fills, and only fills which landed in bins
    double sumW() const;

    /// Get the mean
    /// @todo Differentiate between using all fills, and only fills which landed in bins
    double mean() const;

    /// Get the variance
    /// @todo Differentiate between using all fills, and only fills which landed in bins
    double variance() const;

    /// Get the standard deviation
    /// @todo Differentiate between using all fills, and only fills which landed in bins
    double stdDev() const {
      return std::sqrt(variance());
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
    Axis<HistoBin> _axis;

    //@}

  };


  /// @name Combining histos: global operators
  //@{

  /// Add two histograms
  inline Histo1D operator + (const Histo1D& first, const Histo1D& second) {
    Histo1D tmp = first;
    tmp += second;
    return tmp;
  }

  /// Subtract two histograms
  inline Histo1D operator - (const Histo1D& first, const Histo1D& second) {
    Histo1D tmp = first;
    tmp -= second;
    return tmp;
  }

  //@}


}

#endif
