// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_HistoBin_h
#define YODA_HistoBin_h

#include "YODA/Bin.h"
#include "YODA/Exception.h"

namespace YODA {

  /// @brief A Bin in a 1D histogram
  class HistoBin : public Bin {

  public:

    /// @name Constructor giving bin low and high edges.
    //@{
    HistoBin(double lowedge, double highedge);
    HistoBin(std::pair<double,double> edges);
    //@}


  public:

    /// @brief Fill this bin with weight @a weight.
    /// @todo Be careful about negative weights.
    void fill(double coord, double weight=1.0);

    /// Reset this bin
    void reset ();


  public:

    /// @name Bin content info
    //@{
    /// The area is the sum of weights in the bin, i.e. the
    /// width of the bin has no influence on this figure.
    double area() const;

    /// The height is defined as area/width.
    double height() const;
    //@}

    /// @name Error info
    //@{

    /// Error computed using binomial statistics on the sum of bin weights,
    /// i.e. err_area = sqrt{sum{weights}}
    double areaError() const;

    /// As for the height vs. area, the height error includes a scaling factor
    /// of the bin width, i.e. err_height = sqrt{sum{weights}} / width.
    double heightError() const;

    /// The x error is the standard error on the bin focus. 
    double xError() const;

    //@}


  public:

    /// Add two bins (for use by Histo1D).
    HistoBin& operator += (const HistoBin&);

    /// Subtract two bins
    HistoBin& operator -= (const HistoBin&);


  protected:

    /// Add two bins (internal, explicitly named version)
    HistoBin& add(const HistoBin&);

    /// Subtract one bin from another (internal, explicitly named version)
    HistoBin& subtract(const HistoBin&);

  };


  /// Add two bins
  Bin operator + (const Bin& a, const Bin& b);

  /// Subtract two bins
  Bin operator - (const Bin& a, const Bin& b);


}

#endif
