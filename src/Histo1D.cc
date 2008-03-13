#include "YODA/Histo1D.h"

#include <cmath>

using namespace YODA;
using std::pair;
using std::vector;
using std::string;
using std::make_pair;

Histo1D::Histo1D(string path, string title, vector<double> binedges) :
                AnalysisObject ( path, title ),
                _bins (),
                _underflow ( Bin(0,1) ),
                _overflow ( Bin(0,1) ),
                _cachedBinEdges( binedges ),
                _nbins ( binedges.size()-1 ),
                _binHash ()
{
  sort(_cachedBinEdges.begin(), _cachedBinEdges.end());
  for (size_t i = 0; i < _nbins; i++) {
    _bins.push_back( Bin(_cachedBinEdges[i], _cachedBinEdges[i+1]) );
    // insert upper bound mapped to bin id
    _binHash.insert(make_pair(_cachedBinEdges[i+1],i));
  }
}


Histo1D::Histo1D(string path, string title, size_t nbins, double lower, double upper, bool log) :
                AnalysisObject ( path, title ),
                _bins (),
                _underflow ( Bin(0,1) ),
                _overflow ( Bin(0,1) ),
                _cachedBinEdges(),
                _nbins ( nbins ),
                _binHash ()
{
  if (!log)
    for (size_t i = 0; i <= _nbins; i++)
      _cachedBinEdges.push_back(lower+(upper-lower)*i/_nbins);
  else {
    if (lower == 0.)
      throw LogicError("YODA::Histo: log binning with 0 as lower bound.");
    double c = log10(upper/lower)/nbins;
    for(size_t i = 0; i< nbins; ++i) {
      _cachedBinEdges.push_back(lower*pow(10.,c*i));
    }
    _cachedBinEdges.push_back(lower*pow(10.,c*(nbins+1)));
  }
  for (size_t i = 0; i < _nbins; i++) {
    _bins.push_back( Bin(_cachedBinEdges[i], _cachedBinEdges[i+1]) );
    _binHash.insert(make_pair(_cachedBinEdges[i+1],i));
  }
}

// Histo1D::Histo1D(string path, string title, const_iterator<double> binedges_begin,const_iterator<double> binedges_end);


void Histo1D::fill(double x, double weight) {
  pair<Histo1D::ExtraBin, size_t> index = _coordToIndex(x);
  if ( index.first == VALIDBIN )
    _bins[index.second].fill(x, weight);
  else if (index.first == UNDERFLOWBIN)
    _underflow.fill(0.5, weight);
  else
    _overflow.fill(0.5, weight);
}


void Histo1D::fillBin(size_t index, double weight) {
  if (index >= _nbins)
    throw RangeError("YODA::Histo: index out of range");
  double x = _bins[index].midpoint();
  Histo1D::_bins[index].fill(x, weight);
}


// vector<Bin>& Histo1D::getBins();

Bin& Histo1D::getBin(size_t index) {
  if (index >= _nbins)
    throw RangeError("YODA::Histo: index out of range");
  return _bins[index];  
}

Bin& Histo1D::getBin(Histo1D::ExtraBin binType) {
  if (binType == UNDERFLOWBIN) return _underflow;
  if (binType == OVERFLOWBIN) return _overflow;
  throw RangeError("YODA::Histo: index out of range");
  // just to fix a warning
  return _underflow;
}

Bin& Histo1D::getBinByCoord(double x) {
  pair<Histo1D::ExtraBin, size_t> index = _coordToIndex(x);
  if ( index.first == VALIDBIN ) return _bins[index.second];
  return getBin(index.first);
}


pair<Histo1D::ExtraBin, size_t> Histo1D::_coordToIndex(double coord) const {
  if ( coord < _cachedBinEdges[0] ) return make_pair(UNDERFLOWBIN, 0);
  if ( coord >= _cachedBinEdges[_nbins+1] ) return make_pair(OVERFLOWBIN, 0);
  // size_t i = 0;
  //  while (_cachedBinEdges[i+1] < coord) i++;
  // SP: this is faster, I think;
  // if's above ensure, that we get
  // a valid iterator back
  size_t i = _binHash.upper_bound(coord)->second;
  return make_pair(VALIDBIN, i);
}


double Histo1D::getTotalArea() {
  double area = 0;
  for (size_t i = 0; i < _nbins; i++)
    area += _bins[i].area();
  return area;
}


double Histo1D::getMean() {
  double sumwx = 0;
  double sumw  = 0;
  for (size_t i = 0; i < _nbins; i++) {
    sumwx += _bins[i].sumXWeight();
    sumw  += _bins[i].sumWeight();
  }
  return sumwx/sumw;
}


double getSigma() {
  double mean = getMean();
  double sigma2 = 0;
  for (size_t i = 0; i < _nbins; i++)
    sigma2 += pow( (_bin[i].focus()-mean), 2) * _bin[i].sumWeight();
  return std::sqrt(sigma2/getTotalArea());
}


