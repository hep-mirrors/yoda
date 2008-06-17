// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_Axis_h
#define YODA_Axis_h

#include "YODA/AnalysisObject.h"
#include "YODA/Exception.h"
#include "YODA/Bin.h"
#include <string>
#include <vector>
#include <cassert>
using namespace std;

namespace YODA {


  class Axis {
  public:

    Axis(const vector<double>& binedges) :
      _cachedBinEdges( binedges ),
      _nbins( binedges.size()-1 ),
      _binHash()
    {
      sort(_cachedBinEdges.begin(), _cachedBinEdges.end());
      for (size_t i = 0; i < _nbins; i++) {
        // Insert upper bound mapped to bin ID
        _binHash.insert(make_pair(_cachedBinEdges[i+1],i));
      }
    }


    Axis(size_t nbins, double lower, double upper) :
      _cachedBinEdges(),
      _nbins( nbins ),
      _binHash()
    {
      const double binwidth = (upper-lower)/static_cast<double>(_nbins);
      for (size_t i = 0; i <= _nbins; ++i) {
        const double edge = lower + binwidth*i;
        _cachedBinEdges.push_back(edge);
      }
      for (size_t i = 0; i < _nbins; ++i) {
        _binHash.insert(make_pair(_cachedBinEdges[i+1],i));
      }
    }
    

    // Axis(const Bins& bins) :
    //   _cachedBinEdges(),
    //   _nbins( bins.size() ),
    //   _binHash()
    // {
    //   for (size_t i = 0; i < _nbins; ++i) {
    //     _cachedBinEdges.push_back(_bins[i].lowEdge());
    //     _binHash.insert(make_pair(_bins[i].highEdge(),i));
    //   }
    //   _cachedBinEdges.push_back(_bins.back().highEdge());
    // }


    unsigned int numBins() {
      return _nbins;
    }


    pair<double,double> binEdges(size_t binId) {
      assert(binId < _cachedBinEdges.size());
      return make_pair(_cachedBinEdges[binId], _cachedBinEdges[binId+1]);
    }


  public:

    std::pair<Bin::BinType, size_t> findBinIndex(double coord) const {
      if ( coord < _cachedBinEdges[0] ) return make_pair(Bin::UNDERFLOWBIN, 0);
      if ( coord >= _cachedBinEdges[_nbins] ) return make_pair(Bin::OVERFLOWBIN, 0);
      size_t i = _binHash.upper_bound(coord)->second;
      return make_pair(Bin::VALIDBIN, i);
    }

  bool operator == (const Axis& other) const {
    return 
      _cachedBinEdges == other._cachedBinEdges &&
      _binHash == other._binHash;
  }

  bool operator != (const Axis& other) const {
    return ! operator==(other);
  }

  private:

    /// @name Bin data
    //@{

    /// Bin edges: lower edges, except last entry,
    /// which is the high edge of the last bin
    std::vector<double> _cachedBinEdges;

    /// The number of bins
    /// @todo Remove?
    size_t _nbins;

    /// Map for fast bin lookup
    std::map<double,size_t> _binHash;
    //@}

  };

}

#endif
