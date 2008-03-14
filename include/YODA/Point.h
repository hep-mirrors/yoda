// -*- C++ -*-
//
// This file is part of YODA -- Yet mor Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
class Point {

// Low- and UpEdge are not just x +/- xerror. The error is independent of the
// bin edges. So we need to store the edges _and_ the errors.

// This is all very specific for 1D histogram like Scatters. Not really good if
// we want to keep the option for 2D histograms open. There something like the
// AIDA datapoint implementation makes more sense.

public:
  Point(double x, double y,
        double xerror,
        double yerror);

  Point(double x, double y,
        double xerrorminus,
        double xerrorplus,
        double yerrorminus,
        double yerrorplus);

  Point(double x, double y,
        std::vector<double> xerror,
        std::vector<double> yerror);

  Point(double x, double y,
        std::vector<double> xerrorminus,
        std::vector<double> xerrorplus,
        std::vector<double> yerrorminus,
        std::vector<double> yerrorplus);

public:
  double getX();
  double getY();
  //// This is not the Right[tm] way to do it.
  // double getXLowEdge();
  // double getXUpEdge();
  // double getYLowEdge();
  // double getYUpEdge();
  std::vector<double> getXErrorMinus();
  std::vector<double> getXErrorPlus();
  std::vector<double> getYErrorMinus();
  std::vector<double> getYErrorPlus();

public:
  // We need some kind of operations here, like adding to bins (when combining
  // Histos and Scatters). This will also require checks on bin edges, using
  // the correct type of error, etc.

private:
  double _x;
  double _y;
  std::vector<double> _xerrorminus;
  std::vector<double> _xerrorplus;
  std::vector<double> _yerrorminus;
  std::vector<double> _yerrorplus;
};

