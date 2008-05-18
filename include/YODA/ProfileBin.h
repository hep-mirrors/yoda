// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_ProfileBin_h
#define YODA_ProfileBin_h

#include "YODA/Bin.h"
#include "YODA/Exception.h"

namespace YODA {


  /// A  one-dimensional profile histogram.
  class ProfileBin : public Bin {

    /// Profile1D is a friend to add/subtract bins
    friend class Profile1D;

  public:
    /// @name Constructors
    //@{

    /// @name Constructor giving bin low and high edges.
    //@{
    ProfileBin(double lowedge, double highedge);
    ProfileBin(std::pair<double,double> edges);
    //@}
    
  private:

    /// Fill histo by value and weight.
    void fill(double x, double y, double weight=1.0);
    
    /// Reset the bin.
    void reset();

  public:

    /// @name Bin content info
    //@{
    double meanY() const;

    double stdDevY() const;

    double varianceY() const;

    double stdErrY() const;
    //@}

  private:

    /// Add two bins (for use by Profile1D).
    ProfileBin& operator += (const ProfileBin&);

    /// Subtract two bins
    ProfileBin& operator -= (const ProfileBin&);

    /// The sum of y*weight
    double _sumYWeight;
    double sumYWeight() const;

    /// The sum of y^2 * weight
    double _sumY2Weight;
    double sumY2Weight() const;

  };


  ProfileBin& operator + (const ProfileBin& a, const ProfileBin& b);


  ProfileBin& operator - (const ProfileBin& a, const ProfileBin& b);

}

#endif
