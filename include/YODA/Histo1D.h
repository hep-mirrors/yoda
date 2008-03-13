// -*- C++ -*-
#ifndef YODA_HISTO_H
#define YODA_HISTO_H 1

#include "YODA/AnalysisObject.h"
#include "YODA/Bin.h"
// #include "YODA/Exception.h"
#include <vector>
#include <string>

namespace YODA {

  /// A 1-dimensional histogram.
  class Histo1D : public AnalysisObject {
    
  public:
    enum ExtraBin { UNDERFLOW, OVERFLOW, VALID };
    
  public:
    /// @name Constructors
    //@{
    Histo1D(std::string path, std::string title, size_t nbins, double lower, double upper);
    Histo1D(std::string path, std::string title, std::vector<double> binedges);
    // Histo1D(std::string path, std::string title, const_iterator<double> binedges_begin,const_iterator<double> binedges_end);
    //@}
    
  public:
    /// @name Filling methods
    //@{
    void fill(double x, double weight=1.0);
    void fillBin(size_t index, double weight=1.0);
    //@}
    
    /// @todo Annotations?
    /// @todo Reset method


  public:
    /// @name Bin accessors
    //@{
//    std::vector<Bin>& getBins();
//    iterator<Bin>& bins_begin();
//    iterator<Bin>& bins_end();
//    const const_iterator<Bin>& bins_begin() const;
//    const const_iterator<Bin>& bins_end() const;
//    Bin& getBin(size_t index);
//    Bin& getBin(ExtraBin binType);
//    Bin& getBinByCoord(double coord);
    //@}

//  public:
//    /// @name Whole histo data
//    //@{
//    double getTotalArea();
//    double getMean();
    // @todo double getRMS()?
    //@}

  private:
    /// @name Bin data
    //@{
    std::vector<Bin> _bins;
    Bin _underflow;
    Bin _overflow;
    //@}

    /// @name Private helper methods
    //@{
    std::pair<ExtraBin, size_t> _coordToIndex(double coord) const;
    //@}

    /// @name Caching data
    //@{
    mutable std::vector<double> _cachedBinEdges;
    mutable size_t _nbins;
    //@}
  };


  /// @name Combining histos
  //@{
  // add
  // scale
  // normalize
  //@}
  
}

#endif
