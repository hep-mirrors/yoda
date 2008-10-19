// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_POINT_H
#define YODA_POINT_H

#include <vector>
#include <string>
#include <map>

namespace YODA {


  /// Enum for specifying different error classes.
  enum ErrorClass { ERR_STAT, ERR_SYS };

  /// Enum for specifying how different error classes are to be combined.
  enum ErrorCombScheme { QUAD_COMB, LIN_COMB, HYBRID_COMB };



  class Error1D {
  public:
    Error1D();
    Error1D(double symm_err);
    Error1D(double minus_err, double plus_err);

  public:
    double minusErr() const;
    double plusErr() const;
    double symmErr() const;
    std::pair<double,double> errs() const;

    Error1D& setMinusErr(double minus_err);
    Error1D& setPlusErr(double plus_err);
    Error1D& setErrs(double symm_err);
    Error1D& setErrs(std::pair<double,double> pm_errs);
    Error1D& setErrs(double minus_err, double plus_err);
    
  private:
    double _minus, _plus;
  }



  /// PointError is a collection of related {@link Error1D}s with some metadata.
  class PointError {
  public:
    PointError(ErrorType type=ERR_STAT, const std::string& ann="");

    PointError(std::vector<size_t, Error1D> err1Ds, ErrorType type=ERR_STAT, const std::string& ann="");

    PointError(size_t dim, Error1D, ErrorType type=ERR_STAT, const std::string& ann="");

    PointError(size_t dim, double symm_err, ErrorType type=ERR_STAT, const std::string& ann="");

    PointError(size_t dim, std::pair<double,double> pm_errs, ErrorType type=ERR_STAT, const std::string& ann="");

    PointError(size_t dim, double minus_err, double plus_err, ErrorType type=ERR_STAT, const std::string& ann="");


  public:
    Error1D error(size_t dim) const;
    double plusErr(size_t dim) const;
    double minusErr(size_t dim) const;
    double symmErr(size_t dim) const;
    ErrorType type() const;
    std::string annotation() const;

  public:
    PointError& setError(size_t dim, const Error1D& err1D);
    PointError& setError(size_t dim, double symm_err);
    PointError& setError(size_t dim, std::pair<double,double> pm_errs);
    PointError& setError(size_t dim, double minus_err, double plus_err);

    PointError& setErrorClass(ErrorType type);
    PointError& setAnnotation(const std::string& ann);

  private:
    std::vector<size_t, Error1D> _errors;
    ErrorType _type;
    std::string _annotation;
  };


  /// A collection of point errors
  typedef std::vector<PointError> ErrorSet;


  /// The ErrorCombiner interface
  struct ErrorCombiner { 
    virtual std::pair<double,double> 
    combine_errs(const ErrorSet::const_iterator& begin,
                 const ErrorSet::const_iterator& end) = 0;
  };


#endif
