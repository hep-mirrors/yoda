// -*- C++ -*-
#ifndef YODA_Histo1D_h
#define YODA_Histo1D_h

#include "YODA/AnalysisObject.h"
#include "YODA/Bin.h"
#include "YODA/Exception.h"
#include <vector>
#include <string>
#include <map>

namespace YODA {

  /// A  one-dimensional histogram.
  class Histo1D : public AnalysisObject {
    
  public:

    /// Enumerate the type of bins
    enum ExtraBin { UNDERFLOWBIN, OVERFLOWBIN, VALIDBIN };
    
  public:
    /// @name Constructors
    //@{

    /// Constructor giving range and number of bins
    Histo1D(const std::string& path, const std::string& title,
            size_t nbins,
            double lower, double upper,
            bool log = false);

    /// Constructor giving explicit bin edges
    /// For n bins, binedges.size() == n+1, the last
    /// one being the upper bound of the last bin
    Histo1D(const std::string& path, const std::string& title,
	    const std::vector<double>& binedges);

    /// Constructor giving a vector of bins
    Histo1D(std::string path, std::string title,
	    const std::vector<Bin>& bins);
    //@}
    
  public:
    /// @name Filling methods
    //@{

    /// Fill histo by value and weight
    void fill(double x, double weight=1.0);

    /// Directly fill bin by bin index
    void fillBin(size_t index, double weight=1.0);
    //@}

  public:
    
    /// Reset the histogram: Keep the binning but
    /// set all bin contents and related quantities
    /// to zero
    virtual void reset ();

  public:
    /// @name Bin accessors
    //@{

    /// Access the bin vector
    std::vector<Bin>& getBins();

    /// Access a bin by index
    Bin& getBin(size_t index);

    /// Access a bin by type
    Bin& getBin(ExtraBin binType);

    /// Access a bin bu coordinate
    Bin& getBinByCoord(double x);
    //@}
  
  public:
    /// @name Whole histo data
    //@{

    /// Get the total area
    double getTotalArea();

    /// Get the mean
    double getMean();

    /// Get the sigma
    double getSigma();
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

    /// The bins contained in this histogram
    std::vector<Bin> _bins;

    /// The underflow bin
    Bin _underflow;

    /// The overflow bin
    Bin _overflow;

    //@}

    /// @name Private helper methods
    //@{

    /// Bin lookup: convert value to corresponding bin index
    std::pair<ExtraBin, size_t> _coordToIndex(double coord) const;

    //@}

    /// @name Bin edge data and lookup tables
    //@{

    /// Bin edges: lower edges, except lats entry,
    /// which is the high edge of the last bin
    std::vector<double> _cachedBinEdges;

    /// The number of bins
    size_t _nbins;

    /// Map for fast bin lookup
    std::map<double,size_t> _binHash;

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

#endif // YODA_Histo1D_h
