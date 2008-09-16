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
#include <cmath>
#include <algorithm>

using namespace std;

namespace YODA {

  /// Enumerate the type of binning
  enum Binning { LINEAR, LOG };


  template <typename BIN>
  class Axis {
  public:

    typedef typename std::vector<BIN> Bins;


  private:


    void _mkBinHash() {
      for (size_t i = 0; i < numBins(); i++) {
        // Insert upper bound mapped to bin ID
        _binHash.insert(make_pair(_cachedBinEdges[i+1],i));
      }
    }


    void _mkAxis(const vector<double>& binedges) {
      const size_t nbins = binedges.size() - 1;

      // Make bins
      for (size_t i = 0; i < nbins; ++i) {
        _bins.push_back( BIN(binedges.at(i), binedges.at(i+1)) );
      }

      // Make cached edges
      _cachedBinEdges = binedges;
      std::sort(_cachedBinEdges.begin(), _cachedBinEdges.end());

      // Make hash
      _mkBinHash();
    }


    void _mkAxis(const Bins& bins) {
      _bins = bins;

      // Make cached edges
      for (size_t i = 0; i < bins.size(); ++i) {
        _cachedBinEdges.push_back(bins.at(i).lowEdge());
      }
      _cachedBinEdges.push_back(bins.back().highEdge());

      // Make hash
      _mkBinHash();
    }


    vector<double> _mkEdges(size_t nbins, double lower, 
                            double upper, Binning binning) {
      assert(upper > lower);
      vector<double> rtn;
      

      switch(binning) {
      case LINEAR:
        {
          const double binwidth = (upper-lower)/static_cast<double>(nbins);
          for (size_t i = 0; i <= nbins; ++i) {
            const double edge = lower + binwidth*i;
            rtn.push_back(edge);
          }
        }
        break;

      case LOG:
        {
          const double logupper = std::log10(upper);
          const double loglower = std::log10(lower);
          const double logbinwidth = (logupper-loglower)/static_cast<double>(nbins);
          for (size_t i = 0; i <= nbins; ++i) {
            const double logedge = loglower + logbinwidth*i;
            const double edge = pow(10, logedge);
            rtn.push_back(edge);
          }
        }
        break;

      default:
        throw Error("Invalid binning style... what went wrong?");
      }

      return rtn;
    }



  public:

    Axis(const vector<double>& binedges) :
      _underflow(0, 1, Bin::UNDERFLOWBIN),
      _overflow(0, 1, Bin::OVERFLOWBIN)
    {
      _mkAxis(binedges);
    }


    Axis(size_t nbins, double lower, double upper, Binning binning) :
      _underflow(0, 1, Bin::UNDERFLOWBIN),
      _overflow(0, 1, Bin::OVERFLOWBIN)
    {
      vector<double> binedges = _mkEdges(nbins, lower, upper, binning);
      _mkAxis(binedges);
    }

    
    Axis(const vector<BIN>& bins) :
      _underflow(0, 1, Bin::UNDERFLOWBIN),
      _overflow(0, 1, Bin::OVERFLOWBIN)
    {
      _mkAxis(bins);
    }


    /////////////////////


  public:

    unsigned int numBins() const {
      return _bins.size();
    }


    Bins& bins() {
      return _bins;
    }


    const Bins& bins() const {
      return _bins;
    }


    pair<double,double> binEdges(size_t binId) const {
      assert(binId < numBins());
      return make_pair(_cachedBinEdges[binId], _cachedBinEdges[binId+1]);
    }


    BIN& bin(size_t index) {
      if (index >= numBins())
        throw RangeError("YODA::Histo: index out of range");
      return _bins[index];
    }


    const BIN& bin(size_t index) const {
      if (index >= numBins())
        throw RangeError("YODA::Histo: index out of range");
      return _bins[index];
    }


    BIN& bin(Bin::BinType binType) {
      if (binType == Bin::UNDERFLOWBIN) return _underflow;
      if (binType == Bin::OVERFLOWBIN) return _overflow;
      throw RangeError("YODA::Histo: index out of range");
      return _underflow;
    }


    const BIN& bin(Bin::BinType binType) const {
      if (binType == Bin::UNDERFLOWBIN) return _underflow;
      if (binType == Bin::OVERFLOWBIN) return _overflow;
      throw RangeError("YODA::Histo: index out of range");
      return _underflow;
    }


    BIN& binByCoord(double x) {
      pair<Bin::BinType, size_t> index = findBinIndex(x);
      if ( index.first == Bin::VALIDBIN ) return _bins[index.second];
      return bin(index.first);
    }


    const BIN& binByCoord(double x) const {
      pair<Bin::BinType, size_t> index = findBinIndex(x);
      if ( index.first == Bin::VALIDBIN ) return _bins[index.second];
      return bin(index.first);
    }


    std::pair<Bin::BinType, size_t> findBinIndex(double coord) const {
      if ( coord < _cachedBinEdges[0] ) return make_pair(Bin::UNDERFLOWBIN, 0);
      if ( coord >= _cachedBinEdges[numBins()] ) return make_pair(Bin::OVERFLOWBIN, 0);
      size_t i = _binHash.upper_bound(coord)->second;
      return make_pair(Bin::VALIDBIN, i);
    }


    void reset () {
      _underflow.reset();
      _overflow.reset();
      for (typename Bins::iterator b = _bins.begin(); b != _bins.end(); ++b) {
        b->reset();
      }
    }    


  public:

    bool operator == (const Axis& other) const {
      /// @todo Need/want to compare bin hash?
      return 
        _cachedBinEdges == other._cachedBinEdges &&
        _binHash == other._binHash;
    }

    
    bool operator != (const Axis& other) const {
      return ! operator == (other);
    }


  public:

    Axis<BIN>& operator += (const Axis<BIN>& toAdd) {
      if (*this != toAdd) {
        throw LogicError("YODA::Histo1D: Cannot add axes with different binnings.");
      }
      for (size_t i = 0; i < bins().size(); ++i) {
        bins().at(i) += toAdd.bins().at(i);
      }
      bin(UNDERFLOW) += toAdd.bin(UNDERFLOW);
      bin(OVERFLOW)  += toAdd.bin(OVERFLOW);
      return *this;
    }


    Axis<BIN>& operator -= (const Axis<BIN>& toSubtract) {
      if (*this != toSubtract) {
        throw LogicError("YODA::Histo1D: Cannot subtract axes with different binnings.");
      }
      for (size_t i = 0; i < bins().size(); ++i) {
        bins().at(i) += toSubtract.bins().at(i);
      }
      bin(UNDERFLOW) += toSubtract.bin(UNDERFLOW);
      bin(OVERFLOW)  += toSubtract.bin(OVERFLOW);
    return *this;
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



  template <typename BIN>
  Axis<BIN> operator + (const Axis<BIN>& first, const Axis<BIN>& second) {
    Axis<BIN> tmp = first;
    tmp += second;
    return tmp;
  }


  template <typename BIN>
  Axis<BIN> operator - (const Axis<BIN>& first, const Axis<BIN>& second) {
    Axis<BIN> tmp = first;
    tmp -= second;
    return tmp;
  }



}

#endif
