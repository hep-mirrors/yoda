// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2015 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_TRAITS_H
#define YODA_TRAITS_H

#include <type_traits>

namespace YODA {


  namespace SFINAE {
    typedef char yes[1]; typedef char no[2];
  }

  // SFINAE struct to check for iterator concept at compile time
  template<typename T>
  struct Iterable {
    template <typename C> static SFINAE::yes& test(typename C::const_iterator*  c);
    template <typename> static SFINAE::no& test(...);
    static const bool value = sizeof(test<T>(0)) == sizeof(SFINAE::yes);
  };

  template<typename T>
  struct DerefAO {
    static const bool value = std::has_dereference<T,const YODA::AnalysisObject&>::value;
  };

  template<typename T,typename VAL>
  struct Pushable {
    template <typename SIG,SIG> struct has_sig;
    template<typename C> static SFINAE::yes& test(has_sig<void (C::*) (const VAL&),&C::push_back>*);
    template<typename C> static SFINAE::no& test(...);
    static const bool value = sizeof(test<T>(0)) == sizeof(SFINAE::yes);
  };


}

#endif
