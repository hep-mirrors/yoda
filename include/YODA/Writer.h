// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
namespace YODA {

  /// Pure virtual base class for various output writers.
  class Writer {

    virtual ~Writer() = 0;

    /// @name Writing a single analysis object to file.
    //@{
    /// Write out object @a ao to file @a filename in the format specified by @a format.
    /// @a format will be treated case-insensitively.
    bool write(std::string filename, std::string format, const AnalysisObject& ao) = 0;
    /// Write out object @a ao to file @a filename with the format automatically
    /// determined from the filename suffix.
    bool write(std::string filename, const AnalysisObject& ao) = 0;
    //@}

    /// @name Writing multiple single analysis objects to file.
    //@{
    /// Write out the vector of objects @a objs to file @a filename in the
    /// format specified by @a format. @a format will be treated
    /// case-insensitively.
    bool write(std::string filename, std::string format, const vector<AnalysisObject>& ao) = 0;
    /// Write out vector of objects @a objs to file @a filename with the format automatically
    /// determined from the filename suffix.
    bool write(std::string filename, const vector<AnalysisObject>& ao) = 0;
    /// Write out the objects specified by start iterator @a begin and end
    /// iterator @a end to file @a filename in the format specified by @a format. 
    /// @a format will be treated case-insensitively.
    bool write(std::string filename, std::string format, 
               const iterator<AnalysisObject>& begin, const iterator<AnalysisObject>& end) = 0;
    /// Write out the objects specified by start iterator @a begin and end
    /// iterator @a end to file @a filename with the format automatically
    /// determined from the filename suffix.
    bool write(std::string filename, 
               const iterator<AnalysisObject>& begin, const iterator<AnalysisObject>& end) = 0;
    //@}

  };

}
