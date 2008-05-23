// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_Dbn1D_h
#define YODA_Dbn1D_h

#include "YODA/Exception.h"

namespace YODA {


  /// @brief A 1D distribution This class is used internally by YODA to
  /// centralise the calculation of statistics of unbounded, unbinned sampled
  /// distributions. Each distribution fill contributes a weight, \f$ w \f$, and
  /// a value, \f$ x \f$. By storing the total number of fills (ignoring
  /// weights), \f$ \sum w \f$, \f$ \sum w^2 \f$, \f$ \sum wx \f$, 
  /// and \f$ \sum wx^2 \f$, the Dbn1D can calculate the mean and spread 
  /// (\f$ \sigma^2 \f$, \f$ \sigma \f$ and \f$ \hat{\sigma} \f$) of the
  /// sampled distribution. It is used to provide this information in bins
  /// and for the "hidden" \f$ y \f$ distribution in profile histogram bins.
  class Dbn1D {
  public:

    /// Constructor.
    Dbn1D() {
      reset();
    }

    /// @brief Contribute a sample at @a val with weight @a weight.
    /// @todo Be careful about negative weights.
    void fill(double val, double weight=1.0);

    /// Reset the internal counters.
    void reset();

  public:

    /// @name Distribution statistics
    //@{

    /// Number of entries (number of times @c fill was called, ignoring weights)
    double numEntries() const;

    /// Weighted mean, \f$ \bar{x} \f$, of distribution.
    double mean() const;

    /// Weighted variance, \f$ \sigma^2 \f$, of distribution.
    double variance() const;

    /// Weighted standard deviation, \f$ \sigma \f$, of distribution.
    double stdDev() const;

    /// Weighted standard error, \f$ \sim \sigma/\sqrt{N-1} \f$, of distribution.
    double stdErr() const;

    //@}

  private:

    unsigned int _numFills;
    double _sumW;
    double _sumW2;
    double _sumWX;
    double _sumWX2;

  };


}

#endif
