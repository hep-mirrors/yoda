#include "YODA/AnalysisObject.h"


/// A 1-dimensional histogram.
class Histo : AnalysisObject {

public:
  enum ExtraBin { UNDERFLOW, OVERFLOW };

public:
  /// @name Constructors
  //@{
  Histo(double lower, double upper, size_t nbins);
  Histo(vector<double> binedges);
  Histo(const_iterator<double> binedges_begin,const_iterator<double> binedges_end);
  //@}

public:
  /// @name Filling methods
  //@{
  Histo& fill(double x, double weight=1.0);
  Histo& fillBin(size_t binId, double weight=1.0);
  //@}

public:
  /// @name Bin accessors
  //@{
  vector<Bin>& getBins();
  Bin& getBin(size_t binId);
  Bin& getBin(ExtraBin binType);
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
