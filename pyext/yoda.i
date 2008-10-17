%module yoda


%{
  #define SWIG_FILE_WITH_INIT
  #include "YODA/Histo1D.h"
  #include "YODA/Profile1D.h"
  #include "YODA/Scatter.h"
  using namespace YODA;
%}


// STL vector methods to ignore (for classes with no default constructor)
%feature("ignore") std::vector<YODA::HistoBin>::vector(size_type size);
%feature("ignore") std::vector<YODA::HistoBin>::resize(size_type size);
%feature("ignore") std::vector<YODA::HistoBin>::pop();
%feature("ignore") std::vector<YODA::ProfileBin>::vector(size_type size);
%feature("ignore") std::vector<YODA::ProfileBin>::resize(size_type size);
%feature("ignore") std::vector<YODA::ProfileBin>::pop();


// STL class support
%include "std_string.i"
%include "std_vector.i"


////////////////////////////////////////////////


// YODA base object
%include "YODA/AnalysisObject.h"


// Standard histograms and profile histograms
namespace YODA {
  enum Binning { LINEAR, LOG };
}
%include "YODA/Histo1D.h"
%include "YODA/Profile1D.h"
%include "YODA/Bin.h"
%include "YODA/HistoBin.h"
%include "YODA/ProfileBin.h"
%template(HistoBins) std::vector<YODA::HistoBin>;
%template(ProfileBins) std::vector<YODA::ProfileBin>;


// Scatter plots
%ignore YODA::ErrorCombiner;
%ignore YODA::Point::error(size_t dim, ErrorCombiner& ec);
%ignore YODA::Point::symmError(size_t dim, ErrorCombiner& ec);
%ignore YODA::Point::errors(size_t dim, ErrorCombiner& ec);
%ignore YODA::Point::symmErrors(size_t dim, ErrorCombiner& ec);
%include "YODA/Scatter.h"
%include "YODA/Point.h"
