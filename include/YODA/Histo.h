#include "YODA/AnalysisObject.h"

namespace YODA {

  /// A 1-dimensional histogram.
  class Histo1D : AnalysisObject {
    
  public:
    enum ExtraBin { UNDERFLOW, OVERFLOW };
    
  public:
    /// @name Constructors
    //@{
    Histo1D(std::string name, std::string title, size_t nbins, double lower, double upper);
    Histo1D(std::string name, std::string title, vector<double> binedges);
    Histo1D(std::string name, std::string title, const_iterator<double> binedges_begin,const_iterator<double> binedges_end);
    //@}
    
  public:
    /// @name Filling methods
    //@{
    Histo1D& fill(double x, double weight=1.0);
    Histo1D& fillBin(size_t binId, double weight=1.0);
    //@}
    
    /// @todo Annotations?
    /// @todo Reset method


  public:
    /// @name Bin accessors
    //@{
    vector<Bin>& getBins();
    Bin& getBin(size_t binId);
    Bin& getBin(ExtraBin binType);
    //Bin& getBinByCoord(double coord);
    size_t coordToIndex(double coord) const;
    //@}

  public:
    /// @name Whole histo data
    //@{
    double getTotalArea();
    double getNumEntries();
    double getMean();
    double getRMS();
    //@}

  private:
    /// @name Bin data
    //@{
    vector<Bin> _bins;
    Bin _underflow;
    Bin _overflow;
    //@}

    /// @name Caching data
    //@{
    mutable vector<double> _cachedBinEdges;
    //@}
  };


  /// @name Combining histos
  //@{
  // add
  // scale
  // normalize
  //@}
  
}
