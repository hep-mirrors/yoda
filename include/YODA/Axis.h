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


  template <BIN>
  class Axis {
  public:

    /// Enumerate the type of binning
    enum Binning { LINEAR, LOG };

    typedef std::vector<BIN> Bins;


  private:

    void Axis::_mkExtraBins() {
      _underflow = BIN(0, 1, Bin::UNDERFLOWBIN);
      _overflow =  BIN(0, 1, Bin::OVERFLOWBIN);
    }


    void Axis::_mkBinHash() {
      for (size_t i = 0; i < numBins(); i++) {
        // Insert upper bound mapped to bin ID
        _binHash.insert(make_pair(_cachedBinEdges[i+1],i));
      }
    }


    void Axis::_mkAxis(const vector<double>& binedges) {
      const size_t nbins = binedges.size() - 1;

      // Make bins
      for (size_t i = 0; i < nbins; ++i) {
        _bins.push_back( BIN(binedges.at(i), binedges.at(i+1)) );
      }
      _mkExtraBins();

      // Make cached edges
      _cachedBinEdges = binedges;
      sort(_cachedBinEdges.begin(), _cachedBinEdges.end());

      // Make hash
      _mkBinHash();
    }


    void Axis::_mkAxis(const Bins& bins) {
      _bins = bins;
      _mkExtraBins();

      // Make cached edges
      for (size_t i = 0; i < bins.size(); ++i) {
        _cachedBinEdges.push_back(bins.at(i).lowEdge());
      }
      _cachedBinEdges.push_back(bins.back().highEdge());

      // Make hash
      _mkBinHash();
    }


    vector<double> Axis::_mkEdges(size_t nbins, double lower, 
                                  double upper, Binning binning) {
      assert(upper > lower);
      vector<double> rtn;
      
      switch(binning) {
      case LINEAR:
        const double binwidth = (upper-lower)/static_cast<double>(nbins);
        for (size_t i = 0; i <= nbins; ++i) {
          const double edge = lower + binwidth*i;
          rtn.push_back(edge);
        }
        break;

      case LOG:
        const double logupper = std::log10(upper);
        const double loglower = std::log10(lower);
        const double logbinwidth = (logupper-loglower)/static_cast<double>(nbins);
        for (size_t i = 0; i <= nbins; ++i) {
          const double logedge = loglower + logbinwidth*i;
          const double edge = pow(10, logedge);
          rtn.push_back(edge);
        }
        break;

      default:
        throw Error("Invalid binning style... what went wrong?");
      }

      return rtn;
    }



  public:

    Axis(const vector<double>& binedges) {
      _mkAxis(binedges);
    }


    Axis(size_t nbins, double lower, double upper, Binning binning)
    {
      vector<double> binedges = _mkEdges(nbins, lower, upper, binning);
      _mkAxis(binedges);
    }

    
    Axis(const Bins& bins) {
      _mkAxis(bins);
    }


    /////////////////////


  public:

    unsigned int numBins() {
      return _bins.size();
    }


    pair<double,double> binEdges(size_t binId) {
      assert(binId < numBins());
      return make_pair(_cachedBinEdges[binId], _cachedBinEdges[binId+1]);
    }


  public:

    std::pair<Bin::BinType, size_t> findBinIndex(double coord) const {
      if ( coord < _cachedBinEdges[0] ) return make_pair(Bin::UNDERFLOWBIN, 0);
      if ( coord >= _cachedBinEdges[numBins()] ) return make_pair(Bin::OVERFLOWBIN, 0);
      size_t i = _binHash.upper_bound(coord)->second;
      return make_pair(Bin::VALIDBIN, i);
    }
    
    bool operator == (const Axis& other) const {
      return 
        _cachedBinEdges == other._cachedBinEdges &&
        _binHash == other._binHash;
    }
    
    bool operator != (const Axis& other) const {
      return ! operator == (other);
    }

    
  private:

    /// @name Bin data
    //@{

    /// The bins contained in this histogram
    Bins _bins;

    /// The underflow bin
    BIN _underflow;

    /// The overflow bin
    BIN _overflow;

    /// Bin edges: lower edges, except last entry,
    /// which is the high edge of the last bin
    std::vector<double> _cachedBinEdges;

    /// Map for fast bin lookup
    std::map<double,size_t> _binHash;
    //@}

  };

}

#endif
