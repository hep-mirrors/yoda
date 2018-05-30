#include "YODA/Scatter1D.h"
#include "YODA/Counter.h"
#include <sstream>

namespace YODA {


  /// Make a Scatter1D representation of a Histo1D
  Scatter1D mkScatter(const Counter& c) {
    Scatter1D rtn;
    for (const std::string& a : c.annotations())
      rtn.setAnnotation(a, c.annotation(a));
    rtn.setAnnotation("Type", c.type()); // might override the copied ones
    rtn.addPoint(c.val(), c.err());
    return rtn;
  }
  
  /// Update the annotation which holds the names of the variations 
  void Scatter1D::updateVariations(Scatter1D::Points& points){
    std::vector<std::string> variations;
    YAML::Node variationsYAML; 
    for (auto &point : points){
      for (auto &it : point.errMap()){
        //if the variation is not already in the vector, add it !
        if (std::find(variations.begin(), variations.end(), it.first) == variations.end()){
          variations.push_back(it.first);
        }
      }
    }
    YAML::Emitter out; 
    out << YAML::Flow ;
    out << variations;
    this->setAnnotation("variations",(std::string) out.c_str());
  }

  /// Access the names of the variations as a vector of strings
  const std::vector<std::string> Scatter1D::variations() const {
    std::vector<std::string> vecvariations;
    if (this->hasAnnotation("variations")){
      std::string annvariations = this->annotation("variations");
      std::istringstream iss(annvariations);
      YAML::Parser parser(iss);
      YAML::Node anns;
      parser.GetNextDocument(anns);
      for (YAML::Iterator it = anns.begin(); it != anns.end(); ++it) {
        std::string val;
        *it >> val;
        vecvariations.push_back(val);
      }
    }
    return vecvariations;
  }
}
