%module yoda

%{
  #include "YODA/Histo1D.h"
  #include "YODA/Profile1D.h"
  using namespace YODA;
%}

//%ignore YODA::Bin::Bin();
//%ignore std::vector<YODA::Bin>::vector(size_type);
%feature("ignore") std::vector<YODA::HistoBin>::vector(size_type size);
%feature("ignore") std::vector<YODA::HistoBin>::resize(size_type size);
%feature("ignore") std::vector<YODA::HistoBin>::pop();

%include "std_string.i"
%include "std_vector.i"

%include "YODA/AnalysisObject.h"
%include "YODA/Histo1D.h"
%include "YODA/Profile1D.h"

%template(HistoBins) std::vector<YODA::HistoBin>;
%template(ProfileBins) std::vector<YODA::ProfileBin>;
