#include <string>

namespace YODA {

  class AnalysisObject {

    friend class Histo1D;

  public:
    AnalysisObject(const std::string& path, const std::string& title) :
                _path ( path ),
                _title ( title )
    {
    }
    std::string getPath();
    AnalysisObject& setPath(const std::string& path);

    std::string getTitle();
    AnalysisObject& setTitle(const std::string& title);

  protected:
    std::string _path;
    std::string _title;
  };

  // AnalysisObject& mv(AnalysisObject& ao, const std::string newpath);
  // AnalysisObject& cp(AnalysisObject& ao, const std::string newpath);
  // AnalysisObject& rm(AnalysisObject& ao, const std::string newpath);

}
