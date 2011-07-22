// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2011 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_Weights_h
#define YODA_Weights_h

#include "YODA/Exceptions.h"
#include <vector>
#include <string>
#include <map>

namespace YODA {


  /// A named, vectorised generalisation of an event weight.
  /// @todo Decide on the internal storage type: is there such a thing as a sorted map?
  /// @todo Need all arithmetic operators, plus conversions to and scalings by doubles
  /// @todo Accept general Boost.Ranges as constructor args
  /// @todo Allow size changes? I think NO...
  /// @todo Throw an exception if there is an attempt to combine two weights of different sizes.
  /// @todo Autogenerate numerical names if not given
  class Weights {
  public:

    /// @name Constructors
    //@{

    // Weights() { }

    Weights(const Weights& other) { }

    /// Convenience auto-constructor from a single double, since that's the commonest use case.
    Weights(double value) { }

    /// Constructor from a vector of key/value pairs
    Weights(const vector<std::pair<std::string, double> >& keys_values) { }

    /// Constructor from vectors of keys and values
    Weights(const vector<std::string>& keys, const vector<double>& values) { }

    /// Constructor from vectors of keys and a single value, defaulting to 0.0
    Weights(const vector<std::string>& keys, double value=0.0) { }

    /// Initialise a weights collection of length @a length, with default key names.
    // Weights(size_t length, double value) { }

    //@}

  public:


    /// @name Accessors
    //@{

    /// @todo Sorted iterators of pair<std::string, double>

    /// Number of weights entries
    unsigned int size() { }

    /// List of weight values, in the order of the sorted keys
    vector<double> values() { }

    /// Sorted list of weight keys
    vector<std::string> keys() { }

    /// List of weight values, in the order of the sorted keys
    vector<double> values() { }

    //@}


    /// @name Arithmetic operators as members
    //@{

    // /// Add another weights to this
    // Weights& operator += (const Weights& toAdd) { }

    // /// Subtract another weights from this
    // Weights& operator -= (const Weights& toSubtract) { }

    // /// Multiply by another weights
    // Weights& operator *= (const Weights& toMultiplyBy) { }

    // /// Divide by another weights
    // Weights& operator /= (const Weights& toDivideBy) { }

    // /// Multiply by a double
    // Weights& operator *= (const Weights& toMultiplyBy) { }

    // /// Divide by a double
    // Weights& operator /= (const Weights& toDivideBy) { }

    // /// Negate
    // Weights& operator - () { }

    //@}


    /// @name Comparison operators
    //@{

    // /// Equals
    // bool operator == (const Weights& other) { }

    // /// not equals
    // bool operator == (const Weights& other) {
    //   return !(*this == other);
    // }

    //@}


  private:


  };


  // /// @name Combining weights: global operators
  // //@{

  // /// Add two weights
  // inline inline Weights operator + (const Weights& first, const Weights& second) {
  //   Weights tmp = first;
  //   tmp += second;
  //   return tmp;
  // }

  // /// Subtract two weights
  // inline inline Weights operator - (const Weights& first, const Weights& second) {
  //   Weights tmp = first;
  //   tmp -= second;
  //   return tmp;
  // }

  // /// Multiply two weights
  // inline Scatter2D operator / (const Weights& numer, const Weights& denom) {
  //   Weights tmp = first;
  //   tmp *= second;
  //   return tmp;
  // }


  // /// Divide two weights
  // inline Scatter2D operator / (const Weights& numer, const Weights& denom) {
  //   Weights tmp = first;
  //   tmp /= second;
  //   return tmp;
  // }

  // //@}


}

#endif
