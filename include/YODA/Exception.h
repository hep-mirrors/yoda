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

  /// Basic unspecialised YODA exception.
  class Exception : public std::exception { 
  public:
    Exception() : std::exception() {}
  };


  /// Error for e.g. use of invalid bin ranges.
  class RangeError : public std::out_of_range, public Exception {
  public:
    RangeError(const std::string& what) : std::out_of_range(what) {} 
  };


  /// @todo Clarify where this might arise!
  class LogicError : public std::logic_error, public Exception {
  public:
    LogicError(const std::string& what) : std::logic_error(what) {} 
  };

  /// @brief Errors relating to event/bin weights
  /// Arises in computing statistical quantities because e.g. the bin
  /// weight is zero or negative.
  class WeightError : public std::runtime_error, public Exception {
  public:
    WeightError(const std::string& what) : std::runtime_error(what) {} 
  };

}

#endif
