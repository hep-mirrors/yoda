%module yoda

%{
  #define SWIG_FILE_WITH_INIT
  #include "YODA/Histo1D.h"
  #include "YODA/Profile1D.h"
  #include "YODA/Scatter.h"
  using namespace YODA;
%}


%feature("ignore") std::vector<YODA::HistoBin>::vector(size_type size);
%feature("ignore") std::vector<YODA::HistoBin>::resize(size_type size);
%feature("ignore") std::vector<YODA::HistoBin>::pop();

%feature("ignore") std::vector<YODA::ProfileBin>::vector(size_type size);
%feature("ignore") std::vector<YODA::ProfileBin>::resize(size_type size);
%feature("ignore") std::vector<YODA::ProfileBin>::pop();

%include "std_string.i"
%include "std_vector.i"

namespace YODA {
  enum Binning { LINEAR, LOG };
}

%include "YODA/AnalysisObject.h"
%include "YODA/Histo1D.h"
%include "YODA/Profile1D.h"
%include "YODA/Bin.h"
%include "YODA/HistoBin.h"
%include "YODA/ProfileBin.h"

%template(HistoBins) std::vector<YODA::HistoBin>;
%template(ProfileBins) std::vector<YODA::ProfileBin>;

%include "YODA/Scatter.h"
%include "YODA/Point.h"
