// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2014 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_BINSEARCHER_H
#define YODA_BINSEARCHER_H

#include <cstdlib>
#include <cmath>
#include <vector>
#include <limits>
#include "YODA/Utils/fastlog.h"
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

namespace YODA {
  namespace Utils {


    const size_t SEARCH_SIZE = 16;
    const size_t BISECT_LINEAR_THRESHOLD = 32;


    /// @brief Bin estimator
    ///
    /// Base class for guessing the right bin index for a given value. The
    /// better the guess, the less time spent looking.
    class Estimator {
    public:
      virtual size_t operator() (double x) const = 0;
      virtual ~Estimator() {}
    };


    /// @brief Linear bin estimator
    ///
    /// This class handles guessing a index bin with a hypothesis of uniformly
    /// spaced bins on a linear scale.
    class LinEstimator : public Estimator {
    public:

      /// Constructor
      LinEstimator(double xlow, double xhigh, size_t N) {
        _c = xlow;
        _m = (double) N / (xhigh - xlow);
      }

      /// Copy constructor
      LinEstimator(const LinEstimator& other)
        : _c(other._c), _m(other._m)
      {  }

      /// Call operator returns estimated bin index
      size_t operator() (double x) const {
        return (size_t)(int) floor(_m * (x - _c));
      }

    protected:

      double _c, _m;
    };


    /// @brief Logarithmic bin estimator
    ///
    /// This class handles guessing a bin index with a hypothesis of uniformly
    /// spaced bins on a logarithmic scale.
    ///
    /// @todo Make a generalised version of this with a transform function
    class LogEstimator : public Estimator {
    public:

      /// Constructor
      LogEstimator(double xlow, double xhigh, size_t N) {
        _c = log2(xlow);
        _m = N / (log2(xhigh) - _c);
      }

      /// Copy constructor
      LogEstimator(const LogEstimator& other)
        : _c(other._c), _m(other._m)
      {  }

      /// Call operator returns estimated bin index
      size_t operator() (double x) const {
        return static_cast<size_t>(1 + _m * (fastlog2(x) - _c));
      }

    protected:

      double _c, _m;
    };


    /// @brief Bin searcher
    ///
    /// @author David Mallows
    ///
    /// Handles low-level bin lookups using a hybrid algorithm that is
    /// considerably faster for regular (logarithmic or linear) and near-regular
    /// binnings. Comparable performance for irregular binnings.
    ///
    /// The reason this works is that linear search is faster than bisection
    /// search up to about 32-64 elements. So we make a guess, and we then do a
    /// linear search. If that fails, then we bisect on the remainder,
    /// terminating once bisection search has got the range down to about 32. So
    /// we actually pay for the fanciness of predicting the bin out of speeding
    /// up the bisection search by finishing it with a linear search. So in most
    /// cases, we get constant-time lookups regardless of the space.
    ///
    class BinSearcher {
    public:

      /// Default constructor
      /// @todo What's the point?
      BinSearcher() {
        _est.reset(new LinEstimator(0, 1, 1));
      }

      /// Copy constructor
      BinSearcher(const BinSearcher& bs) {
        _est = bs._est;
        _edges = bs._edges;
        _max = bs._max;
      }

      /// Explicit constructor, specifying the edges and estimation strategy
      BinSearcher(const std::vector<double>& edges, bool log) {
        _updateEdges(edges);
        // Internally use a log or linear estimator as requested
        if (log) {
          _est.reset(new LogEstimator(edges.front(), edges.back(), edges.size()-1));
        } else {
          _est.reset(new LinEstimator(edges.front(), edges.back(), edges.size()-1));
        }
      }

      /// Fully automatic constructor: give bin edges and it does the rest!
      BinSearcher(const std::vector<double>& edges) {
        _updateEdges(edges);

        if (edges.empty()) {
          _est.reset(new LinEstimator(0, 0, 0));
        } else if (edges.front() <= 0.0) {
          _est.reset(new LinEstimator(edges.front(), edges.back(), edges.size()-1));
        } else {
          LinEstimator linEst(edges.front(), edges.back(), edges.size()-1);
          LogEstimator logEst(edges.front(), edges.back(), edges.size()-1);

          // Calculate mean index estimate deviations from the correct answers (for bin edges)
          double logsum = 0, linsum = 0;
          for (size_t i = 0; i < edges.size(); i++) {
            logsum += fabs(logEst(edges[i]) - i);
            linsum += fabs(linEst(edges[i]) - i);
          }
          const double log_avg = logsum / edges.size();
          const double lin_avg = linsum / edges.size();

          // This also implicitly works for NaN returned from the log There is a
          // subtle bug here if the if statement is the other way around, as
          // (nan < linsum) -> false always.  But (nan > linsum) -> false also.
          if (log_avg < lin_avg) { //< Use log estimator if its avg performance is better than lin
            _est.reset(new LogEstimator(logEst));
          } else { // Else use linear estimation
            _est.reset(new LinEstimator(linEst));
          }
        }
      }


      /// Look up a bin index
      /// @todo What about x out of range? Need to return ssize_t = -1?
      __attribute__((noinline))
      size_t index(double x) const {
        size_t index = _estimate(x);

        if (x >= _edges[index]) {
          const size_t di = _linsearch_forward(_edges.get() + index, x, SEARCH_SIZE);
          index += di;
          if (di == SEARCH_SIZE) index = _bisect(_edges.get(), x, index, _max);
        } else {
          const size_t di = _linsearch_backward(_edges.get() + index, x, SEARCH_SIZE);
          index -= di;
          if (di == SEARCH_SIZE) index = _bisect(_edges.get(), x, 0, index+1);
        }

        return index - 1;
      }


    protected:

      /// Make an estimate of the bin index containing x
      size_t _estimate(const double x) const {
        double y = (*_est)(x); //< @todo This is where the functor syntax doesn't help...
        if ( y > double(_max) ) return _max;
        size_t yi = (size_t)(int) floor((y < 0) ? 0 : y);
        return (yi > _max) ? _max : yi;
      }


      // std::pair<double, double> indexRange(size_t ix) const {
      //   return std::make_pair(_edges[ix], _edges[ix+1]);
      // }

      // bool inRange (const std::pair<double,double>& range, double x) const {
      //   return (range.first <= x) && (x < range.second);
      // }


      void _updateEdges(const std::vector<double>& edges) {
        _edges.reset(new double[edges.size() + 2]);

        // Lower sentinel
        _edges[0] = -std::numeric_limits<double>::infinity();

        // Copy a sorted vector
        for (size_t i = 0; i < edges.size(); i++) {
          _edges[i+1] = edges[i];
        }

        _max = edges.size() + 1;

        // Upper sentinel
        _edges[_max] = std::numeric_limits<double>::infinity();
      }


      // Linear search in the forward direction
      size_t _linsearch_forward(const double* arr, double key, size_t n) const {
        for (size_t i = 0; i < n; i++) if (arr[i] > key) return i;
        return n;
      }


      // Linear search in the backward direction
      size_t _linsearch_backward(const double* arr, double key, size_t n) const {
        arr -= n;
        for (size_t i = 0; i < n; i++) if (arr[n - 1 - i] <= key) return i;
        return n;
      }


      // Bisection search, adapted from C++ std lib implementation
      size_t _bisect(const double* arr, const double key, size_t min, size_t max) const {
        size_t len = max - min;
        size_t middle;
        while (len > BISECT_LINEAR_THRESHOLD) {
          size_t half = len >> 1;
          middle = min + half;
          if (arr[middle] < key) {
            min = middle + 1;
            len = len - half - 1;
          } else
            len = half;
        }
        return min + _linsearch_forward(arr + min, key, BISECT_LINEAR_THRESHOLD);
      }


    protected:

      boost::shared_ptr<Estimator> _est;
      boost::shared_array<double> _edges;
      size_t _max;

    };


  }
}

#endif
