// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
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

    /// Constructor giving range and number of bins
    Histo1D(const std::string& path, const std::string& title,
            size_t nbins,
            double lower, double upper);

    /// @brief Constructor giving explicit bin edges.
    /// For n bins, binedges.size() == n+1, the last
    /// one being the upper bound of the last bin
    Histo1D(const std::string& path, const std::string& title,
            const std::vector<double>& binedges);

    /// Constructor giving a vector of bins
    Histo1D(std::string path, std::string title,
            const std::vector<HistoBin>& bins);

    /// Constructor giving range and number of bins
    Histo1D(size_t nbins, double lower, double upper);

    /// @brief Constructor giving explicit bin edges.
    /// For n bins, binedges.size() == n+1, the last
    /// one being the upper bound of the last bin
    Histo1D(const std::vector<double>& binedges);

    /// Constructor giving a vector of bins
    Histo1D(const std::vector<HistoBin>& bins);
    //@}


  private:
    /// @name Constructor helper methods
    //@{
    void _ctorFromBins();
    void _ctorFromEdges();
    void _ctorFromAxis();
    //@}

    
  public:
    /// @name Filling methods
    //@{

    /// Fill histo by value and weight
    void fill(double x, double weight=1.0);

    /// Directly fill bin by bin index
    void fillBin(size_t index, double weight=1.0);

    /// @brief Reset the histogram. 
    /// Keep the binning but set all bin contents and related quantities to zero
    virtual void reset();
    //@}


  public:
    /// @name Bin accessors
    //@{

    /// Access the bin vector
    std::vector<YODA::HistoBin>& bins();

    /// Access the bin vector (const version)
    const std::vector<YODA::HistoBin>& bins() const;

    /// Access a bin by index
    HistoBin& bin(size_t index);

    /// Access a bin by index (const version)
    const HistoBin& bin(size_t index) const;

    /// @brief Access the underflow and overflow bins by type
    /// Using the VALIDBIN enum value as an argument will throw an exception.
    HistoBin& bin(Bin::BinType binType);

    /// @brief Access the underflow and overflow bins by type (const version)
    /// Using the VALIDBIN enum value as an argument will throw an exception.
    const HistoBin& bin(Bin::BinType binType) const;

    /// Access a bin by coordinate (non-const version)
    HistoBin& binByCoord(double x);

    /// Access a bin by coordinate (const version)
    const HistoBin& binByCoord(double x) const;


    //@}


  protected:

  
  public:
    /// @name Whole histo data
    //@{

    /// Get the total area
    double area() const;

    /// Get sum of weights in histo (same as area)
    double sumWeight() const;

    /// Get the mean
    double mean() const;

    /// Get the variance
    double variance() const;

    /// Get the standard deviation
    double stdDev() const;
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
    typedef std::vector<HistoBin> Bins;

    /// Definition of bin edges
    Axis _axis;

    /// The bins contained in this histogram
    Bins _bins;

    /// The underflow bin
    HistoBin _underflow;

    /// The overflow bin
    HistoBin _overflow;

    //@}
  };


  /// @name Combining histos: global operators
  //@{

  /// Add two histograms
  Histo1D operator + (const Histo1D& first, const Histo1D& second);

  /// Subtract two histograms
  Histo1D operator - (const Histo1D& first, const Histo1D& second);

  //@}


}

#endif
