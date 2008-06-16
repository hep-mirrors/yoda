// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_Exception_h
#define YODA_Exception_h

#include <string>
#include <exception>
#include <stdexcept>

namespace YODA {

  /// Generic unspecialised YODA runtime error.
  class Error : public std::runtime_error {
  public:
    Error(const std::string& what) : std::runtime_error(what) {} 
  };


  /// Also typedef Exception, so that it's there.
  typedef Error Exception;


  /// Error for e.g. use of invalid bin ranges.
  class RangeError : public Error {
  public:
    RangeError(const std::string& what) : Error(what) {} 
  };


  /// @todo Clarify where this might arise!
  class LogicError : public Error {
  public:
    LogicError(const std::string& what) : Error(what) {} 
  };

  /// @brief Errors relating to event/bin weights
  /// Arises in computing statistical quantities because e.g. the bin
  /// weight is zero or negative.
  class WeightError : public Error {
  public:
    WeightError(const std::string& what) : Error(what) {} 
  };

}

#endif
