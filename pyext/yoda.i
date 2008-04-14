%module yoda

%{
  #include "YODA/Histo1D.h"
  using namespace YODA;
%}

//%ignore YODA::Bin::Bin();
//%ignore std::vector<YODA::Bin>::vector(size_type);
%feature("ignore") std::vector<YODA::Bin>::vector(size_type size);
%feature("ignore") std::vector<YODA::Bin>::resize(size_type size);
%feature("ignore") std::vector<YODA::Bin>::pop();

%include "std_string.i"
%include "std_vector.i"

%include "YODA/AnalysisObject.h"
%include "YODA/Histo1D.h"

%template(Bins) std::vector<YODA::Bin>;
