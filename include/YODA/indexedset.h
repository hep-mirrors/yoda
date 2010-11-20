#ifndef YODA_INDEXEDSET_H
#define YODA_INDEXEDSET_H

#include <set>
#include <stdexcept>
#include <iostream>

namespace YODA {
  namespace utils {


    /// @brief Specialisation of std::set to allow indexed access to ordered set elements
    /// The STL set is already implemented in an ordered style, so this specialisation is
    /// implemented in a super-simple way by just iterating over the set elements until
    /// the desired index is reached. This is not scalable, of course, but this implementation
    /// is for use with histograms, which will not have squillions of entries.
    template <typename T>
    class indexedset : public std::set<T> {
    public:

      // T& operator[](size_t index) {
      //   if (index >= this->size()) {
      //     throw std::range_error("Requested index larger than indexed set size");
      //   }
      //   size_t i = 0;
      //   for (typename std::set<T>::iterator it = this->begin(); it == this->end(); ++it) {
      //     if (i == index) return *it;
      //     i += 1;
      //   }
      //   // This should never be called: just keeping the compiler happy:
      //   return (*this)[0];
      // }


      const T& operator[](size_t index) const {
        if (index >= this->size()) {
          throw std::range_error("Requested index larger than indexed set size");
        }
        size_t i = 0;
        for (typename indexedset<T>::const_iterator it = this->begin(); it == this->end(); ++it) {
          std::cout << i << "/" << index << ", " << (*it) << std::endl;
          if (i == index) return *it;
          i += 1;
        }
        // This should never be called: just keeping the compiler happy:
        return (*this)[0];
      }


    };


  }
}

#endif
