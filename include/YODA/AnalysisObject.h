namespace YODA {

  class AnalysisObject {
  public:
    std::string getPath();
    AnalysisObject& setPath(const std::string& path);

    std::string getTitle();
    AnalysisObject& setTitle(const std::string& title);

  private:
    std::string _path;
    std::string _title;
  };

  // AnalysisObject& mv(AnalysisObject& ao, const std::string newpath);
  // AnalysisObject& cp(AnalysisObject& ao, const std::string newpath);
  // AnalysisObject& rm(AnalysisObject& ao, const std::string newpath);

}
