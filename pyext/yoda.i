%module yoda


%{
  #define SWIG_FILE_WITH_INIT
  #include "YODA/Histo1D.h"
  #include "YODA/Profile1D.h"
  #include "YODA/Scatter.h"
  using namespace YODA;
%}


// STL class support
%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"
%template(DoubleList) std::vector<double>;
%template(DoublePair) std::pair<double, double>;
%template(IntDoubleDict) std::map<size_t, double>;

////////////////////////////////////////////////


// YODA base object
%include "YODA/AnalysisObject.h"


// Standard histograms and profile histograms
namespace YODA {
  enum Binning { LINEAR, LOG };
}


// Histos
%feature("ignore") std::vector<YODA::HistoBin>::vector(size_type size);
%feature("ignore") std::vector<YODA::HistoBin>::resize(size_type size);
%feature("ignore") std::vector<YODA::HistoBin>::pop();
%include "YODA/Bin.h"
%include "YODA/HistoBin.h"
%include "YODA/Histo1D.h"
%template(HistoBins) std::vector<YODA::HistoBin>;


// Profile histos
%feature("ignore") std::vector<YODA::ProfileBin>::vector(size_type size);
%feature("ignore") std::vector<YODA::ProfileBin>::resize(size_type size);
%feature("ignore") std::vector<YODA::ProfileBin>::pop();
%include "YODA/ProfileBin.h"
%include "YODA/Profile1D.h"
%template(ProfileBins) std::vector<YODA::ProfileBin>;


// Scatter plot errors
%ignore YODA::ErrorCombiner;
%include "YODA/Error.h"
%template(PointError1D) YODA::PointError<1>;
%template(PointError2D) YODA::PointError<2>;
%template(ErrorSet1D) YODA::ErrorSet<1>; // ?
%template(ErrorSet2D) YODA::ErrorSet<2>; // ?


// Scatter plot points
%ignore YODA::Point::error(size_t dim, ErrorCombiner& ec);
%ignore YODA::Point::symmError(size_t dim, ErrorCombiner& ec);
%ignore YODA::Point::errors(size_t dim, ErrorCombiner& ec);
%ignore YODA::Point::symmErrors(size_t dim, ErrorCombiner& ec);
%include "YODA/Point.h"


// Scatter plots
%include "YODA/Scatter.h"
%template(Point1D) YODA::Point<1>;
%template(Point2D) YODA::Point<2>;
%template(Scatter1D) YODA::Scatter<1>;
%template(Scatter2D) YODA::Scatter<2>;

