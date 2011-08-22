%module yodawrap

%{
  #define SWIG_FILE_WITH_INIT
  #include "YODA/Histo1D.h"
  #include "YODA/Histo2D.h"
  #include "YODA/Profile1D.h"

  #include "YODA/Point2D.h"
  #include "YODA/Scatter2D.h"

  #include "YODA/Point3D.h"
  #include "YODA/Scatter3D.h"

  #include "YODA/WriterYODA.h"
  #include "YODA/WriterAIDA.h"

  using namespace YODA;
%}


// STL class support
%include "std_string.i"
%include "std_vector.i"
%include "std_list.i"
%include "std_set.i"
%include "std_map.i"
%template(DoubleList) std::vector<double>;
%template(DoublePair) std::pair<double, double>;
%template(IntDoubleDict) std::map<size_t, double>;


////////////////////////////////////////////////


// YODA base object
%include "YODA/AnalysisObject.h"
namespace YODA {
  %extend AnalysisObject {
    %template(setAnnotation) setAnnotation<std::string>;
    %template(addAnnotation) addAnnotation<std::string>;
  };
}


// Bin base classes and contained distributions
%include "YODA/Bin.h"
%feature("ignore") YODA::Bin1D::operator=;
%include "YODA/Bin1D.h"
%include "YODA/Bin2D.h"
%include "YODA/Dbn1D.h"
%include "YODA/Dbn2D.h"


// Histos
%template(Bin1DDbn1D) YODA::Bin1D<YODA::Dbn1D>;
%feature("ignore") YODA::HistoBin1D::operator=;
%feature("ignore") YODA::Histo1D::operator=;
%include "YODA/HistoBin1D.h"
%include "YODA/Histo1D.h"
// %feature("ignore") std::vector<YODA::HistoBin1D>::vector(size_type size);
// %feature("ignore") std::vector<YODA::HistoBin1D>::resize(size_type size);
// %feature("ignore") std::vector<YODA::HistoBin1D>::pop();
// %template(HistoBin1Ds) std::vector<YODA::HistoBin1D>;

/// @todo We currently have to hide these functions from the Python wrapper: let's fix that!
%feature("ignore") YODA::HistoBin2D::transformX;
%feature("ignore") YODA::HistoBin2D::transformY;
%feature("ignore") YODA::Histo2D::mkProfileX;
%feature("ignore") YODA::Histo2D::mkProfileY;
%feature("ignore") YODA::HistoBin2D::operator=;
%feature("ignore") YODA::Histo2D::operator=;
%include "YODA/HistoBin2D.h"
%include "YODA/Histo2D.h"
// %feature("ignore") std::vector<YODA::HistoBin2D>::vector(size_type size);
// %feature("ignore") std::vector<YODA::HistoBin2D>::resize(size_type size);
// %feature("ignore") std::vector<YODA::HistoBin2D>::pop();
// %template(HistoBin2Ds) std::vector<YODA::HistoBin2D>;

// Profile histos
%template(Bin1DDbn2D) YODA::Bin1D<YODA::Dbn2D>;
%feature("ignore") YODA::ProfileBin1D::operator=;
%feature("ignore") YODA::Profile1D::operator=;
%include "YODA/ProfileBin1D.h"
%include "YODA/Profile1D.h"
// %feature("ignore") std::vector<YODA::ProfileBin1D>::vector(size_type size);
// %feature("ignore") std::vector<YODA::ProfileBin1D>::resize(size_type size);
// %feature("ignore") std::vector<YODA::ProfileBin1D>::pop();
// %template(ProfileBin1Ds) std::vector<YODA::ProfileBin1D>;



// Scatter plots
%ignore operator ==(const YODA::Point2D&, const YODA::Point2D&);
%ignore operator !=(const YODA::Point2D&, const YODA::Point2D&);
%ignore operator <(const YODA::Point2D&, const YODA::Point2D&);
%ignore operator <=(const YODA::Point2D&, const YODA::Point2D&);
%ignore operator >(const YODA::Point2D&, const YODA::Point2D&);
%ignore operator >=(const YODA::Point2D&, const YODA::Point2D&);
%feature("ignore") YODA::Point2D::operator=;
%feature("ignore") YODA::Scatter2D::operator=;
%include "YODA/Point2D.h"
%include "YODA/Scatter2D.h"

%ignore operator ==(const YODA::Point3D&, const YODA::Point3D&);
%ignore operator !=(const YODA::Point3D&, const YODA::Point3D&);
%ignore operator <(const YODA::Point3D&, const YODA::Point3D&);
%ignore operator <=(const YODA::Point3D&, const YODA::Point3D&);
%ignore operator >(const YODA::Point3D&, const YODA::Point3D&);
%ignore operator >=(const YODA::Point3D&, const YODA::Point3D&);
/// @todo Uncomment when these methods exist
// %feature("ignore") YODA::Point3D::operator=;
// %feature("ignore") YODA::Scatter3D::operator=;
%include "YODA/Point3D.h"
%include "YODA/Scatter3D.h"


// I/O
%template(AOVector) std::vector<AnalysisObject*>;
%template(AOList) std::list<AnalysisObject*>;
%template(AOSet) std::set<AnalysisObject*>;
%include "YODA/Writer.h"
%include "YODA/WriterAIDA.h"
%include "YODA/WriterYODA.h"
// %inline %{
//   namespace YODA {
//     Writer* get_writer(const std::string& name) {
//       if (name == "AIDA") return &WriterAIDA::create();
//       if (name == "YODA") return &WriterYODA::create();
//       return 0;
//     }
//   }
// %}



// // Scatter plot errors
// %ignore YODA::ErrorCombiner;
// %include "YODA/Error.h"
// %template(PointError1D) YODA::PointError<1>;
// %template(PointError2D) YODA::PointError<2>;
// %template(PointError3D) YODA::PointError<3>;
// //%template(ErrorSet1D) YODA::ErrorSet<1>; // ?
// //%template(ErrorSet2D) YODA::ErrorSet<2>; // ?
// //%template(ErrorSet3D) YODA::ErrorSet<3>; // ?

// // Scatter plot points
// %ignore YODA::Point::error(size_t dim, ErrorCombiner& ec);
// %ignore YODA::Point::symmError(size_t dim, ErrorCombiner& ec);
// %ignore YODA::Point::errors(size_t dim, ErrorCombiner& ec);
// %ignore YODA::Point::symmErrors(size_t dim, ErrorCombiner& ec);
// %include "YODA/Point.h"

// // Scatter plots
// %include "YODA/Scatter.h"
// %template(Point1D) YODA::Point<1>;
// %template(Point2D) YODA::Point<2>;
// %template(Point3D) YODA::Point<3>;
// %template(Scatter1D) YODA::Scatter<1>;
// %template(Scatter2D) YODA::Scatter<2>;
// %template(Scatter3D) YODA::Scatter<3>;
