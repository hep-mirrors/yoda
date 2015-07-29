// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2015 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_TRAITS_H
#define YODA_TRAITS_H

namespace YODA{
  //SFINAE struct to check for iterator concept at compile time
  template<typename T>
  struct Iterable{
    typedef char yes[1]; typedef char no[2];
    template <typename C> static yes& test(typename C::const_iterator*  c);
    template <typename> static no& test(...);
    static const bool value = sizeof(test<T>(0)) == sizeof(yes);
  };
}
#endif
