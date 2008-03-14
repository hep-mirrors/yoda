%module yoda

%include "std_string.i"
%include "std_vector.i"

%{
  #include "YODA/AnalysisObject.h"
  #include "YODA/Histo1D.h"
  #include "YODA/Bin.h"
%}

#namespace std {
#  %template(Bin) vector<Bin>;
#}

%include "YODA/AnalysisObject.h"

%include "YODA/Histo1D.h"

%include "YODA/Bin.h"
