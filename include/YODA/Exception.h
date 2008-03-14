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

  class Exception : public std::exception { 
  public:
    Exception() : std::exception() {}
  };

  class RangeError : public std::out_of_range {
  public:
    RangeError(const std::string& what) : std::out_of_range(what) {} 
  };

  class LogicError : public std::logic_error {
  public:
    LogicError(const std::string& what) : std::logic_error(what) {} 
  };

}

#endif // YODA_Exception_h
