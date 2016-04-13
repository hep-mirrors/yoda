// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2016 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_TRAITS_H
#define YODA_TRAITS_H

#include <type_traits>

namespace YODA {


  namespace SFINAE {
    /// @todo Replaceable by C++11 std::false/true_type?
    typedef char yes[1]; typedef char no[2];

    /// C++11 equivalent of C++17 std::void_t
    template <class...>
    using void_t = void;
}



  // SFINAE definition of has_dereference traits structs, cf. Boost
  template <class T, class = void>
  struct has_dereference : std::false_type {};
  //
  template <class T>
  struct has_dereference<T, SFINAE::void_t<decltype(*std::declval<T>())>> : std::true_type {};


  // SFINAE struct to check for iterator concept at compile time
  /// @todo Replaceable with some C++11 std stuff?
  template<typename T>
  struct Iterable {
    template <typename C> static SFINAE::yes& test(typename C::const_iterator*  c);
    template <typename> static SFINAE::no& test(...);
    static const bool value = sizeof(test<T>(0)) == sizeof(SFINAE::yes);
  };


  // SFINAE struct to check for dereferencing concept at compile time
  template<typename T>
  struct DerefAO {
    static const bool value = has_dereference<T,const YODA::AnalysisObject&>::value;
  };


  // SFINAE struct to check for pushing concept at compile time
  template<typename T,typename VAL>
  struct Pushable {
    template <typename SIG,SIG> struct has_sig;
    template<typename C> static SFINAE::yes& test(has_sig<void (C::*) (const VAL&),&C::push_back>*);
    template<typename C> static SFINAE::no& test(...);
    static const bool value = sizeof(test<T>(0)) == sizeof(SFINAE::yes);
  };


}

#endif
