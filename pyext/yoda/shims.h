/// This file is purely for backward compatibility with Cython 1.3 and 1.4.

#include "YODA/AnalysisObject.h"
#include "YODA/Histo1D.h"
#include "YODA/Scatter2D.h"
#include "YODA/HistoBin1D.h"
#include "YODA/Histo2D.h"
#include "YODA/Scatter3D.h"
#include "YODA/WriterAIDA.h"
#include <string>
#include <vector>

using namespace YODA;

/// Histo1D operators

inline Histo1D add_Histo1D(const Histo1D& a, const Histo1D& b) {
  return a + b;
}

inline Histo1D subtract_Histo1D(const Histo1D& a, const Histo1D& b) {
  return a - b;
}

inline Scatter2D divide_Histo1D(const Histo1D& a, const Histo1D& b) {
  return a / b;
}

/// HistoBin1D Operators
inline HistoBin1D add_HistoBin1D(const HistoBin1D& a, const HistoBin1D& b) {
  return a + b;
}

inline HistoBin1D subtract_HistoBin1D(const HistoBin1D& a, const HistoBin1D& b) {
  return a - b;
}

/// Histo2D operators

inline Histo2D add_Histo2D(const Histo2D& a, const Histo2D& b) {
  return a + b;
}

inline Histo2D subtract_Histo2D(const Histo2D& a, const Histo2D& b) {
  return a - b;
}

inline Scatter3D divide_Histo2D(const Histo2D& a, const Histo2D& b) {
  return a / b;
}

void WriterAIDA_write (const std::string& filename, const std::vector<AnalysisObject*>& aos)
{
  return WriterAIDA::write(filename, aos);
}

Scatter2D Scatter2D_mkScatter(const Histo1D& h)
{
  return YODA::mkScatter(h);
}
