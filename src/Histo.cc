#include "YODA/Histo.h"

using namespace YODA;
using std::pair;
using std::vector;
using std::string;
using std::make_pair;

Histo1D::Histo1D(string path, string title, vector<double> binedges) :
                AnalysisObject ( path, title ),
                _cachedBinEdges( binedges ),
                _nbins ( binedges.size()-1 ),
                _bins (),
                _underflow ( Bin(0,1) ),
                _overflow ( Bin(0,1) )
{
  sort(_cachedBinEdges.begin(), _cachedBinEdges.end());
  for (size_t i = 0; i < _nbins; i++)
    _bins.push_back( Bin(_cachedBinEdges[i], _cachedBinEdges[i+1]) );
};


Histo1D::Histo1D(string path, string title, size_t nbins, double lower, double upper) :
                AnalysisObject ( path, title ),
                _cachedBinEdges(),
                _nbins ( nbins ),
                _bins (),
                _underflow ( Bin(0,1) ),
                _overflow ( Bin(0,1) )
{
  for (size_t i = 0; i <= _nbins; i++)
    _cachedBinEdges.push_back(lower+(upper-lower)*i/_nbins);
  for (size_t i = 0; i < _nbins; i++)
    _bins.push_back( Bin(_cachedBinEdges[i], _cachedBinEdges[i+1]) );
};

// Histo1D::Histo1D(string path, string title, const_iterator<double> binedges_begin,const_iterator<double> binedges_end);


void Histo1D::fill(double x, double weight) {
  pair<Histo1D::ExtraBin, size_t> index = _coordToIndex(x);
  if ( index.first == VALID )
    _bins[index.second].fill(x, weight);
  else if (index.first == UNDERFLOW)
    _underflow.fill(0.5, weight);
  else
    _overflow.fill(0.5, weight);
};


void Histo1D::fillBin(size_t index, double weight) {
//  if (index >= _nbins)
//    throw RangeError("YODA::Histo: index out of range");
  double x = _bins[index].midpoint();
  Histo1D::_bins[index].fill(x, weight);
};


// vector<Bin>& Histo1D::getBins();
// Bin& Histo1D::getBin(size_t index);
// Bin& Histo1D::getBin(ExtraBin binType);
// Bin& Histo1D::getBinByCoord(double coord);


pair<Histo1D::ExtraBin, size_t> Histo1D::_coordToIndex(double coord) const {
  if ( coord < _cachedBinEdges[0] ) return make_pair(UNDERFLOW, 0);
  if ( coord >= _cachedBinEdges[_nbins+1] ) return make_pair(OVERFLOW, 0);
  size_t i = 0;
  while (_cachedBinEdges[i+1] < coord) i++;
  return make_pair(VALID, i);
};


// double getTotalArea();
// double getMean();
// double getRMS();

