// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_ERROR_H
#define YODA_ERROR_H

#include <vector>
#include <string>
#include <map>

namespace YODA {


  /// Enum for specifying different error classes.
  enum ErrorType { ERR_STAT, ERR_SYS };

  /// Enum for specifying how different error classes are to be combined.
  enum ErrorCombScheme { QUAD_COMB, LIN_COMB, HYBRID_COMB };



  class Error1D {
  public:
    Error1D();
    Error1D(double symm_err);
    Error1D(double minus_err, double plus_err);
    Error1D(std::pair<double,double> pm_errs);

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
  };



  /// PointError is a collection of related {@link Error1D}s with some metadata.
  template <size_t N>
  class PointError {
  public:
    PointError(ErrorType type=ERR_STAT, 
               const std::string& ann="")
      : _type(type), _annotation(ann)
    {  }


    PointError(std::map<size_t, Error1D> err1Ds, 
               ErrorType type=ERR_STAT, const std::string& ann="")
      : _errors(err1Ds), _type(type), _annotation(ann)
    {  }


    PointError(size_t dim, Error1D err, 
               ErrorType type=ERR_STAT, const std::string& ann="")
    : _type(type), _annotation(ann)
    {  
      setError(dim, err);
    }
  

    PointError(size_t dim, double symm_err, 
               ErrorType type=ERR_STAT, const std::string& ann="")
      : _type(type), _annotation(ann)
    {  
      setError(dim, symm_err);
    }
  

    PointError(size_t dim, std::pair<double,double> pm_errs, 
               ErrorType type=ERR_STAT, const std::string& ann="")
      : _type(type), _annotation(ann)
    {  
      setError(dim, pm_errs);    
    }
  

    PointError(size_t dim, double minus_err, double plus_err, 
               ErrorType type=ERR_STAT, const std::string& ann="")
      : _type(type), _annotation(ann)
    {  
      Error1D err(minus_err, plus_err);
      setError(dim, err);
    }


  public:
    Error1D error(size_t dim) const { 
      map<size_t, Error1D>::const_iterator ie =_errors.find(dim);
      if (ie == _errors.end()) {
        /// @todo Check num dimensions
        //if (dim >= numDims())
        //throw Exception("Requesting");
        return Error1D();
      }
      return ie->second;
    }


    double plusErr(size_t dim) const {  
      return error(dim).plusErr();
    }


    double minusErr(size_t dim) const {  
      return error(dim).minusErr();
    }
  

    double symmErr(size_t dim) const {  
      return error(dim).symmErr();
    }
  
  
    ErrorType type() const {  
      return _type;
    }
  
  
    std::string annotation() const {  
      return _annotation;
    }
  

  public:
    PointError& setError(size_t dim, const Error1D& err1D) {  
      /// @todo Check num dimensions
      _errors[dim] = err1D;
      return *this;
    }
  
  
    PointError& setError(size_t dim, double symm_err) {  
      _errors[dim] = Error1D(symm_err);
      return *this;
    }
  
  
    PointError& setError(size_t dim, std::pair<double,double> pm_errs) {  
      _errors[dim] = Error1D(pm_errs);
      return *this;
    }
  
  
    PointError& setError(size_t dim, double minus_err, double plus_err) {  
      _errors[dim] = Error1D(minus_err, plus_err);
      return *this;
    }
  

    PointError& setType(ErrorType type) {  
      _type = type;
      return *this;
    }
  
  
    PointError& setAnnotation(const std::string& ann) {  
      _annotation = ann;
      return *this;
    }


  private:
    YODA::Error1D _errors[N];
    YODA::ErrorType _type;
    std::string _annotation;
  };



  /// A collection of point errors
  /// @todo Make this a templated typedef with "typename"?
  template <size_t N>
  typedef typename std::vector<YODA::PointError<N> > ErrorSet;



  /// The ErrorCombiner interface
  struct ErrorCombiner { 
    virtual std::vector<YODA::Error1D>
    combine_errs(const ErrorSet::const_iterator& begin,
                 const ErrorSet::const_iterator& end) = 0;
  };


  struct QuadErrComb : public ErrorCombiner {
    virtual std::vector<YODA::Error1D>
    combine_errs(const ErrorSet::const_iterator& begin,
                 const ErrorSet::const_iterator& end);
  };


  struct LinErrComb : public ErrorCombiner {
    virtual std::vector<YODA::Error1D>
    combine_errs(const ErrorSet::const_iterator& begin,
                 const ErrorSet::const_iterator& end);
  };


}

#endif
