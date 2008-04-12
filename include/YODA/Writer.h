// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//

#include "YODA/AnalysisObject.h"
#include "YODA/Histo1D.h"
#include <vector>
#include <string>


namespace YODA {

  /// Pure virtual base class for various output writers.
  class Writer {
  public:

    virtual ~Writer() {}

    /// @name Writing a single analysis object.
    //@{
    /// Write out object @a ao to output stream @a stream in the
    /// format specified by @a format. @a format will be treated case-insensitively.
    virtual bool write(std::ostream& stream, const AnalysisObject& ao) = 0;
    /// Write out object @a ao to file @a filename in the format specified by @a format.
    /// @a format will be treated case-insensitively.
    virtual bool write(const std::string& filename, const AnalysisObject& ao) = 0;
    //@}


    /// @name Writing multiple analysis objects by collection.
    //@{
    /// Write out the vector of objects @a objs to output stream @a stream in the
    /// format specified by @a format. @a format will be treated case-insensitively.
    virtual bool write(std::ostream& stream, const std::vector<AnalysisObject>& aos) = 0;
    /// Write out the vector of objects @a objs to file @a filename in the
    /// format specified by @a format. @a format will be treated case-insensitively.
    virtual bool write(const std::string& filename, const std::vector<AnalysisObject>& aos) = 0;
    //@}

    /// @name Writing multiple analysis objects by iterator range.
    //@{
    /// Write out the objects specified by start iterator @a begin and end
    /// iterator @a end to output stream @a stream in the format specified by 
    /// @a format.  @a format will be treated case-insensitively.
    virtual bool write(std::ostream& stream, 
                       const std::vector<AnalysisObject>::const_iterator& begin, 
                       const std::vector<AnalysisObject>::const_iterator& end) = 0;
    /// Write out the objects specified by start iterator @a begin and end
    /// iterator @a end to file @a filename in the format specified by @a format. 
    /// @a format will be treated case-insensitively.
    virtual bool write(const std::string& filename,
                       const std::vector<AnalysisObject>::const_iterator& begin, 
                       const std::vector<AnalysisObject>::const_iterator& end) = 0;
    //@}


  protected:
    /// @brief Find the file extension.
    /// The file extension is defined here as the part of the file base name
    /// from the first @c . character to the end, excluding the leading dot. 
    /// It is returned as a lower-case string for ease of comparison.
    /// This way, multi-part extensions like @c .aida.xml or @c .dat.gz can 
    /// be used without being chopped to only show the last part.
    //std::string _getFileExtension(const std::string& filename) {
    //}

  };

}
