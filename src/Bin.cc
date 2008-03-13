#include "YODA/Bin.h"
#include <cassert>

using namespace YODA;
using std::pair;

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
  return ( _edges.second - _edges.first ) / 2;
}

double area() const
{

}
