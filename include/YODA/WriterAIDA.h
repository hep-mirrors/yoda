// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//

#include "YODA/AnalysisObject.h"
#include "YODA/Writer.h"

#include <vector>
#include <string>
#include <ostream>


namespace YODA {

  /// @todo Is there really any reason for this to be a class? 
  /// Are we going to use inheritance or polymorphism anywhere between output format objects?
  class WriterAIDA : public Writer {
  public:

    virtual ~WriterAIDA() { }
    
    bool write(std::ostream& stream, const Histo1D& h);
    bool write(std::ostream& stream, const AnalysisObject& ao);
    bool write(const std::string& filename, const AnalysisObject& ao);
    bool write(std::ostream& stream, const std::vector<AnalysisObject>& aos);
    bool write(const std::string& filename, const std::vector<AnalysisObject>& aos);
    bool write(std::ostream& stream, 
               const std::vector<AnalysisObject>::const_iterator& begin, 
               const std::vector<AnalysisObject>::const_iterator& end);
    bool write(const std::string& filename,
               const std::vector<AnalysisObject>::const_iterator& begin, 
               const std::vector<AnalysisObject>::const_iterator& end);

  };

}
