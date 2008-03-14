// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_SCATTER_H
#define YODA_SCATTER_H 1

#include "YODA/AnalysisObject.h"
#include <vector>
#include <string>
#include "Point.h"
#include "Histo1D.h"

namespace YODA {

  class Scatter : public AnalysisObject {
  public:
    Scatter(std::string name);
    Scatter(const Histo1D &); // explicit?

    Scatter(std::string name,
        const std::vector<double> & data, 
        const std::vector<double> & yerrors,
        const std::vector<double> & xerrors = std::vector<double>());

    void addErrors(const std::vector<double> & yerrors, 
           const std::vector<double> & xerrors = std::vector<double>());

    Scatter & operator+=(const Scatter &);
    Scatter & operator-=(const Scatter &);
    Scatter & operator*=(const Scatter &);
    Scatter & operator/=(const Scatter &);

    // don't use implicit Scatter -> Histo1D conversion if runtime problem
    Scatter & operator+=(const Histo1D &);
    Scatter & operator-=(const Histo1D &);
    Scatter & operator*=(const Histo1D &);
    Scatter & operator/=(const Histo1D &);

    string name() const;
  private:
    // for debugging
    bool isConsistent() const;

  private:
    std::vector<Point> _points;
    // treat as cache
    std::vector<double> _binlimits;
    double _sumweights;
    std::string _name;
  };

  Scatter operator+(const Scatter &, const Scatter &);
  Scatter operator-(const Scatter &, const Scatter &);
  Scatter operator*(const Scatter &, const Scatter &);
  Scatter operator/(const Scatter &, const Scatter &);

  Scatter operator+(const Scatter &, const Histo1D &);
  Scatter operator-(const Scatter &, const Histo1D &);
  Scatter operator*(const Scatter &, const Histo1D &);
  Scatter operator/(const Scatter &, const Histo1D &);

  Scatter operator+(const Histo1D &, const Scatter &);
  Scatter operator-(const Histo1D &, const Scatter &);
  Scatter operator*(const Histo1D &, const Scatter &);
  Scatter operator/(const Histo1D &, const Scatter &);

}

#endif
