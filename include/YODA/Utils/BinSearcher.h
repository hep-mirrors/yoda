#ifndef YODA_BINSEARCHER_H
#define YODA_BINSEARCHER_H

#include <cstdlib>
#include <vector>
#include <limits>
#include <iostream>
#include "YODA/Utils/fastlog.h"
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

namespace YODA {
  namespace Utils {

    const size_t SEARCH_SIZE = 16;
    const size_t BISECT_LINEAR_THRESHOLD = 32;


    /// @brief Bin estimator
    ///
    /// This class handles guessing the right bin for a given value. The better
    /// the guess, the less time spent looking.
    class Estimator {
    public:
      virtual size_t operator() (double x) const = 0;
      virtual ~Estimator() {}
    };


    /// @brief Linear bin estimator
    ///
    /// This class handles guessing a bin on a linear scale.
    class LinEstimator : public Estimator {
    public:
      LinEstimator(double xlow, double xhigh, size_t N) {
        _c = xlow;
        _max = N + 1;
        _m = (double) N / (xhigh - xlow);
      }

      size_t operator() (double x) const {
        return (size_t) _m * (x - _c);
      }

    protected:
      double _c, _m;
      size_t _max;
    };


    /// @brief Linear bin estimator
    ///
    /// This class handles guessing a bin on a logarithmic scale.
    class LogEstimator : public Estimator {
    public:
      LogEstimator(double xlow, double xhigh, size_t N) {
        _c = log2(xlow);
        _max = N + 1;
        _m = N / (log2(xhigh) - _c);
      }

      size_t operator() (double x) const {
        return static_cast<size_t>(1 + _m * (fastlog2(x) - _c));
      }

    protected:
      double _c, _m;
      size_t _max, _half;
    };


    /// @brief Bin searcher
    ///
    /// Handles low-level bin lookups using a hybrid algorithm that is
    /// considerably faster for regular (logarithmic or linear) and near-regular
    /// binnings. Comparable performance for irregular binnings.

    // The reason this works is that linear search is faster than bisection
    // search up to about 32-64 elements. So we make a guess, and we then do a
    // linear search. If that fails, then we bisect on the remainder,
    // terminating once bisection search has got the range down to about 32. So
    // we actually pay for the fanciness of predicting the bin out of speeding
    // up the bisection search by finishing it with a linear search. So in most
    // cases, we get constant-time lookups regardless of the space.

    class BinSearcher {
    public:

      BinSearcher() {
        _est.reset(new LinEstimator(0, 1, 1));
      }

      // Copy constructor
      BinSearcher(const BinSearcher& bs) {
        _est = bs._est;
        _lows = bs._lows;
        _max = bs._max;
      }

      // Explicit constructor
      BinSearcher(std::vector<double> &lows, bool log)
      {
        _updateLows(lows);
        // Use an array for c-style hackery
        // (Templates make no sense here)
        if (log) {
          _est.reset(new LogEstimator(lows.front(), lows.back(), lows.size() - 1));
        } else {
          _est.reset(new LinEstimator(lows.front(), lows.back(), lows.size() - 1));
        }
      }

      // Fully automatic constructor: give bins and it does the rest!
      BinSearcher(std::vector<double> &lows)
      {
        _updateLows(lows);

        if (!lows.size())
          _est.reset(new LinEstimator(0, 0, 0));
        else {
          LinEstimator linEst(lows.front(), lows.back(), lows.size() - 1);
          LogEstimator logEst(lows.front(), lows.back(), lows.size() - 1);

          double logsum = 0;
          for (size_t i = 0; i < lows.size(); i++) {
            logsum += fabs(logEst(lows[i]) - i);
          }

          double linsum = 0;
          for (size_t i = 0; i < lows.size(); i++) {
            linsum += fabs(linEst(lows[i]) - i);
          }

          // Use an array for c-style hackery
          // (Templates make no sense here)
          double log_avg = logsum / lows.size();
          double lin_avg = linsum / lows.size();

          // This also implicitly works for NaN returned from the log
          // There is a subtle bug here if the if statement is the other way
          // around, as (nan < linsum) -> false always.
          // But (nan > linsum) -> false also.
          if (log_avg < lin_avg) {
            _est.reset(
                       new LogEstimator(
                                        lows.front(), lows.back(), lows.size() - 1));
          } else {
            _est.reset(
                       new LinEstimator(
                                        lows.front(), lows.back(), lows.size() - 1));
          }
        }

      }

      // Lookup a bin
      __attribute__((noinline))
      size_t index(double x) const {

        size_t index = estimate(x);

        if (x >= _lows[index]) {
          size_t di = _linsearch_forward(_lows.get() + index, x, SEARCH_SIZE);
          index += di;
          if (di == SEARCH_SIZE) {
            index = _bisect(_lows.get(), x, index, _max);
          }
        } else {
          size_t di = _linsearch_backward(_lows.get() + index, x, SEARCH_SIZE);
          index -= di;
          if (di == SEARCH_SIZE) {
            index = _bisect(_lows.get(), x, 0, index + 1);
          }
        }

        return index - 1;
      }

      size_t estimate(const double x) const {
        double y = (*_est)(x);
        size_t yi = (y < 0) ? 0 : y;
        return (yi > _max) ? _max : yi;
      }

      std::pair<double, double> indexRange(size_t ix) const {
        return std::make_pair(_lows[ix], _lows[ix+1]);
      }

      bool inRange (std::pair<double, double> range, double x) const {
        return (range.first <= x) && (x < range.second);
      }

    protected:
      void _updateLows(std::vector<double> &lows) {
        _lows.reset(new double[lows.size() + 2]);

        // Lower sentinel
        _lows[0] = -std::numeric_limits<double>::infinity();

        // Copy a sorted vector
        for (size_t i = 0; i < lows.size(); i++) {
          _lows[i+1] = lows[i];
        }

        _max = lows.size() + 1;

        // Upper sentinel
        _lows[_max] = std::numeric_limits<double>::infinity();
      }

      // Linear search in the forward direction
      size_t _linsearch_forward(double *arr, double key, size_t n) const {
        for (size_t i=0; i<n; i++)
          if (arr[i] > key) return i;
        return n;
      }

      // Linear search in the backward direction
      size_t _linsearch_backward(double *arr, double key, size_t n) const {
        arr -= n;
        for (size_t i=0; i < n; i++)
          if (arr[n - 1 - i] <= key) return i;
        return n;
      }

      // Bisection search, adapted from C++ std lib implementation
      size_t _bisect(double *arr,
                     const double key, size_t min, size_t max) const {
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
      boost::shared_array<double> _lows;
      size_t _max;
    };
  }
}

#endif
