#include "YODA/Scatter.h"

namespace YODA {


  Scatter::Scatter() { }


  ////////////////////////////////////////////



  Scatter1D::Scatter1D(const std::string& path, const std::string& title) 
    : AnalysisObject(path, title)
  {  }
  

  Scatter1D::Scatter1D(const std::vector<YODA::Point1D>& points) 
    : _points(points)
  {  }
  
  
  Scatter1D::Scatter1D(const std::string& path, const std::string& title,
                       const std::vector<YODA::Point1D>& points) 
    : AnalysisObject(path, title),
      _points(points)
  {  }


  size_t Scatter1D::numPoints() const {
    return _points.size();
  }


  vector<Point1D>& Scatter1D::points() {
    return _points;
  }


  const vector<Point1D>& Scatter1D::points() const {
    return _points;
  }


  Point1D& Scatter1D::point(size_t index) {
    assert(index < numPoints();
    return _points.at(index);
  }


  const Point1D& Scatter1D::point(size_t index) const {
    return _points.at(index);
  }
  
  
  Scatter1D& Scatter1D::addPoint(const Point1D& pt) {
    _points.push_back(pt);
    return *this;
  }
  
  
  Scatter1D& Scatter1D::combineWith(const Scatter1D& other) {
    for (vector<Point1D>::const_iterator pt = other.points().begin(); 
         pt != other.points().end(); ++pt) {
      addPoint(*pt);
    }
    return *this;
  }
  
  
  Scatter1D& Scatter1D::combineWith(const std::vector<Scatter1D>& others) {
    for (vector<Scatter1D>::const_iterator s = others.begin(); 
         s != others.end(); ++s) {
      combineWith(*s);
    }
    return *this;
  }
  
  
  size_t Scatter1D::numDims() {
    return 1;
  }
  
  
  void Scatter1D::reset() {
    _points.clear();
  }


  Scatter1D combine(const Scatter1D& a, const Scatter1D& b) {
    Scatter1D rtn = a;
    rtn.combineWith(b);
    return rtn;
  }


  Scatter1D combine(const std::vector<Scatter1D>& scatters) {
    Scatter1D rtn;
    for (vector<Scatter1D>::const_iterator s = scatters.begin(); 
         s != scatters.end(); ++s) {
      rtn.combineWith(*s);
    }
    return rtn;
  }
  
  
  
  ////////////////////////////////////////////
  
  
  
  Scatter2D::Scatter2D(const std::string& path, const std::string& title)
    : AnalysisObject(path, title)
  {  }
  
  
  Scatter2D::Scatter2D(const std::vector<YODA::Point2D>& points) 
    : _points(points)
  {  }
  
  
  Scatter2D::Scatter2D(const std::string& path, const std::string& title,
                       const std::vector<YODA::Point2D>& points)
    : AnalysisObject(path, title),
      _points(points)
  {  }


  size_t Scatter2D::numPoints() const {
    return _points.size();
  }


  std::vector<Point2D>& Scatter2D::points() {
    return _points;
  }


  const vector<Point1D>& Scatter2D::points() const {
    return _points;
  }
  
  
  Point2D& Scatter2D::point(size_t index) {
    assert(index < numPoints();
    return _points.at(index);
  }
  
  
  const Point2D& Scatter2D::point(size_t index) const {
    return _points.at(index);    
  }
  
  
  Scatter2D& Scatter2D::addPoint(const Point2D&) {
    _points.push_back(pt);
    return *this;
  }
  
  
  Scatter2D& Scatter2D::combineWith(const Scatter2D& other) {
    for (vector<Point1D>::const_iterator pt = other.points().begin(); 
         pt != other.points().end(); ++pt) {
      addPoint(*pt);
    }
    return *this;    
  }
  
  
  Scatter2D& Scatter2D::combineWith(const std::vector<Scatter2D>& others) {
    for (vector<Scatter1D>::const_iterator s = others.begin(); 
         s != others.end(); ++s) {
      combineWith(*s);
    }
    return *this;  
  }
  
  
  size_t Scatter2D::numDims() {
    return 2;
  }

  
  void Scatter2D::reset() {
    _points.clear();
  }


  Scatter2D combine(const Scatter2D& a, const Scatter2D& b) {
    Scatter1D rtn = a;
    rtn.combineWith(b);
    return rtn;
  }


  Scatter2D combine(const std::vector<Scatter2D>& scatters) {
    Scatter1D rtn;
    for (vector<Scatter1D>::const_iterator s = scatters.begin(); 
         s != scatters.end(); ++s) {
      rtn.combineWith(*s);
    }
    return rtn;
  }
  

}
