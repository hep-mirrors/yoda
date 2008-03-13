#include "YODA/Bin.h"
#include <cassert>
#include <cmath>

using namespace YODA;
using std::pair;
using std::make_pair;
using std::sqrt;

Bin::Bin(double low, double high) : _edges( make_pair(low,high) ),
				    _numEntries(),
				    _sumWeight(),
				    _sumWeight2(),
				    _sumXWeight(),
				    _sumX2Weight()
{
  assert( _edges.second >= _edges.first );
}


Bin::Bin(pair<double, double> edges) : _edges( edges ),
				       _numEntries(),
				       _sumWeight(),
				       _sumWeight2(),
				       _sumXWeight(),
				       _sumX2Weight()
{
  assert( _edges.second >= _edges.first );
}


void Bin::fill(double x, double w)
{
  assert( _edges.first != _edges.second 
	  && x >= _edges.first 
	  && x < _edges.second 
	  || x == _edges.first );
  
  ++_numEntries;

  _sumWeight += w;
  _sumWeight2 += w * w;
  
  _sumXWeight += x * w;
  _sumX2Weight += x * x * w;
}


double Bin::lowEdge() const 
{
  return _edges.first;
}


double Bin::highEdge() const 
{
  return _edges.second;
}

pair<double,double> Bin::edges() const
{
  return _edges;
}

double Bin::width() const
{
  return _edges.second - _edges.first;
}

double Bin::focus() const
{
  return _sumXWeight / _sumWeight;
}

double Bin::midpoint() const
{
  return ( _edges.second + _edges.first ) / 2;
}

double Bin::area() const
{
  return _sumWeight;
}

double Bin::height() const
{
  return area() / width();
}

double Bin::areaError() const
{
  return sqrt( _sumWeight );
}

double Bin::heightError() const
{
  return areaError() / width();
}

double Bin::xError() const
{
  return sqrt( ( _sumX2Weight * _sumWeight - _sumXWeight * _sumXWeight) 
	       / ( _sumWeight * _sumWeight - _sumWeight2 ) * _sumWeight );
}

double Bin::sumWeight() const
{
  return _sumWeight;
}

double Bin::sumWeight2() const
{
  return _sumWeight2;
}

double Bin::sumXWeight() const
{
  return _sumXWeight;
}

double Bin::sumX2Weight() const
{
  return _sumX2Weight;
}

Bin& Bin::operator += (const Bin& toAdd) {
  assert(_edges == toAdd._edges);
  _numEntries += toAdd._numEntries;
  _sumWeight += toAdd._sumWeight;
  _sumWeight2 += toAdd._sumWeight2;
  _sumXWeight += toAdd._sumXWeight;
  _sumX2Weight += toAdd._sumX2Weight;
  return *this;
}

Bin& Bin::operator -= (const Bin& toSubtract) {
  assert(_edges == toSubtract._edges);
  _numEntries -= toSubtract._numEntries;
  _sumWeight -= toSubtract._sumWeight;
  _sumWeight2 -= toSubtract._sumWeight2;
  _sumXWeight -= toSubtract._sumXWeight;
  _sumX2Weight -= toSubtract._sumX2Weight;
  return *this;
}
