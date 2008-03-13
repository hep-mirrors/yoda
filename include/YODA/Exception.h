#ifndef YODA_EXCEPTION_H
#define YODA_EXCEPTION_H 1

#include <string>
#include <exception>

namespace YODA {

  class Exception : public std::exception { 
  public:
    Exception(const std::string& what) : std::exception(what) {}
  };

  class RangeError : public Exception {
  public:
    RangeError(const std::string& what) : Exception(what) {} 
  };

}

#endif
