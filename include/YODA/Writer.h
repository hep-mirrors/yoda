// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
namespace YODA {

  /// Pure virtual base class for various output writers.
  class Writer {

    virtual ~Writer() = 0;

    /// @name Writing a single analysis object.
    //@{
    /// Write out object @a ao to output stream @a stream in the
    /// format specified by @a format. @a format will be treated case-insensitively.
    static bool write(std::ostream& stream, const std::string& format, const vector<AnalysisObject>& ao) = 0;
    /// Write out object @a ao to file @a filename in the format specified by @a format.
    /// @a format will be treated case-insensitively.
    static bool write(const std::string& filename, const std::string& format, const AnalysisObject& ao) = 0;
    /// Write out object @a ao to file @a filename with the format automatically
    /// determined from the filename suffix.
    static bool write(const std::string& filename, const AnalysisObject& ao) = 0;
    //@}


    /// @name Writing multiple analysis objects by collection.
    //@{
    /// Write out the vector of objects @a objs to output stream @a stream in the
    /// format specified by @a format. @a format will be treated case-insensitively.
    static bool write(std::ostream& stream, const std::string& format, const vector<AnalysisObject>& ao) = 0;
    /// Write out the vector of objects @a objs to file @a filename in the
    /// format specified by @a format. @a format will be treated case-insensitively.
    static bool write(const std::string& filename, const std::string& format, const vector<AnalysisObject>& ao) = 0;
    /// Write out vector of objects @a objs to file @a filename with the format automatically
    /// determined from the filename suffix.
    static bool write(const std::string& filename, const vector<AnalysisObject>& ao) = 0;
    //@}

    /// @name Writing multiple analysis objects by iterator range.
    //@{
    /// Write out the objects specified by start iterator @a begin and end
    /// iterator @a end to output stream @a stream in the format specified by 
    /// @a format.  @a format will be treated case-insensitively.
    static bool write(std::ostream& stream, const std::string& format, 
                      const iterator<AnalysisObject>& begin, const iterator<AnalysisObject>& end) = 0;
    /// Write out the objects specified by start iterator @a begin and end
    /// iterator @a end to file @a filename in the format specified by @a format. 
    /// @a format will be treated case-insensitively.
    static bool write(const std::string& filename, const std::string& format, 
                      const iterator<AnalysisObject>& begin, const iterator<AnalysisObject>& end) = 0;
    /// Write out the objects specified by start iterator @a begin and end
    /// iterator @a end to file @a filename with the format automatically
    /// determined from the filename suffix.
    static bool write(const std::string& filename, 
                      const iterator<AnalysisObject>& begin, const iterator<AnalysisObject>& end) = 0;
    //@}


  protected:
    /// @brief Find the file extension.
    /// The file extension is defined here as the part of the file base name
    /// from the first @c . character to the end, excluding the leading dot. 
    /// It is returned as a lower-case string for ease of comparison.
    /// This way, multi-part extensions like @c .aida.xml or @c .dat.gz can 
    /// be used without being chopped to only show the last part.
    std::string _getFileExtension(const std::string& filename) {
      
    }

  };

}
