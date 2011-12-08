#ifndef YODA_ARRAY_H
#define YODA_ARRAY_H

#include <vector>
#include <algorithm>
#include <sstream>
#include "boost/array.hpp"

namespace YODA {
  namespace Utils {


    /// @brief Fixed-size array with constructors from standard array types
    template <typename T, int N>
    class ndarray {
    public:

      /// @name Constructors
      //@{

      /// Default constructor
      ndarray() {
        clear();
      }

      /// Conversion from std::vector
      ndarray(const std::vector<T>& vec) {
        if (vec.size() != N) {
          std::stringstream msg;
          msg << "Value vector of wrong size supplied to a " << N << " dimensional array";
          throw RangeError(msg.str());
        }
        std::copy(vec, vec+N, _val);
      }


      /// Conversion from Boost array
      ndarray(const boost::array<T,N> arr) {
        _val = arr;
      }


      /// Conversion from C array
      ndarray(const T arr[5]) {
        try {
          std::copy(arr, arr+N, _val);
        } catch (...) {
          std::stringstream msg;
          msg << "Value vector of wrong size supplied to a " << N << " dimensional array";
          throw RangeError(msg.str());
        }
      }

      //@}


      /// @name Accessors and modifiers
      //@{

      /// Clear the point values and errors
      void clear() {
        for (size_t i = 0; i < N; ++i) _val[i] = T();
      }

      /// Array-like accessor
      T& operator[](size_t i) {
        return _val[i];
      }

      /// Array-like accessor (const version)
      const T& operator[](size_t i) const {
        return _val[i];
      }

      //@}


    private:

      boost::array<T,N> _val;

    };


  }
}

#endif
