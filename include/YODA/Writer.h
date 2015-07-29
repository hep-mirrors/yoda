// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2015 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_Writer_h
#define YODA_Writer_h

#include "YODA/AnalysisObject.h"
#include "YODA/Counter.h"
#include "YODA/Histo1D.h"
#include "YODA/Histo2D.h"
#include "YODA/Profile1D.h"
#include "YODA/Profile2D.h"
#include "YODA/Scatter1D.h"
#include "YODA/Scatter2D.h"
#include "YODA/Scatter3D.h"
#include "YODA/Utils/Traits.h"

#include "boost/range.hpp"
#include "boost/utility/enable_if.hpp"

#include <string>
#include <fstream>

namespace YODA {


  /// Pure virtual base class for various output writers.
  class Writer {
  public:

    /// Virtual destructor
    virtual ~Writer() {}


    /// @name Writing a single analysis object.
    //@{

    /// Write out object @a ao to output stream @a stream.
    void write(std::ostream& stream, const AnalysisObject& ao);

    /// Write out object @a ao to file @a filename.
    void write(const std::string& filename, const AnalysisObject& ao);

    //@}


    /// @name Writing multiple analysis objects by collection.
    //@{

    /// Write out a collection of objects @a objs to output stream @a stream.
    /// Note: the enable_if call checks whether RANGE is const_iterable, if yes the return
    ///       type is void. If not, this template will not be a candidate in the lookup
    template <typename RANGE>
    typename boost::enable_if_c<Iterable<RANGE>::value>::type
    write(std::ostream& stream, const RANGE& aos) {
      //typedef typename boost::range_iterator<const RANGE>::type const_iterator;
      write(stream, boost::begin(aos), boost::end(aos));
    }

    /// Write out a collection of objects @a objs to file @a filename.
    template <typename RANGE>
    typename boost::enable_if_c<Iterable<RANGE>::value>::type
    write(const std::string& filename, const RANGE& aos) {
      //typedef typename boost::range_iterator<const RANGE>::type const_iterator;
      write(filename, boost::begin(aos), boost::end(aos));
    }

    //@}


    /// @name Writing multiple analysis objects by iterator range.
    //@{

    /// Write out the objects specified by start iterator @a begin and end
    /// iterator @a end to output stream @a stream.
    template <typename AOITER>
    void write(std::ostream& stream, const AOITER& begin, const AOITER& end) {
      writeHeader(stream);
      for (AOITER ipao = begin; ipao != end; ++ipao) {
        writeBody(stream, *ipao);
      }
      writeFooter(stream);
    }

    /// Write out the objects specified by start iterator @a begin and end
    /// iterator @a end to file @a filename.
    template <typename AOITER>
    void write(const std::string& filename,
               const AOITER& begin,
               const AOITER& end) {
      std::ofstream outstream;
      outstream.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
      try{
        outstream.open(filename.c_str());
        write(outstream, begin, end);
        outstream.close();
      } catch(std::ifstream::failure e) {
        throw WriteError("writing to filename " + filename + " failed: " + e.what());
      }
    }

    //@}


    /// Set precision of numerical quantities in this writer's output.
    void setPrecision(int precision) {
      _precision = precision;
    }


  protected:

    /// Main writer elements
    virtual void writeHeader(std::ostream& stream) = 0;
    virtual void writeBody(std::ostream& stream, const AnalysisObject* ao);
    virtual void writeBody(std::ostream& stream, const AnalysisObject& ao);
    virtual void writeFooter(std::ostream& stream) = 0;

    /// Specific AO type writer implementations
    virtual void writeCounter(std::ostream& stream, const Counter& c) = 0;
    virtual void writeHisto1D(std::ostream& os, const Histo1D& h) = 0;
    virtual void writeHisto2D(std::ostream& os, const Histo2D& h) = 0;
    virtual void writeProfile1D(std::ostream& os, const Profile1D& p) = 0;
    virtual void writeProfile2D(std::ostream& os, const Profile2D& p) = 0;
    virtual void writeScatter1D(std::ostream& os, const Scatter1D& s) = 0;
    virtual void writeScatter2D(std::ostream& os, const Scatter2D& s) = 0;
    virtual void writeScatter3D(std::ostream& os, const Scatter3D& s) = 0;

    /// Output precision
    int _precision;

  };


  /// Factory function to make a writer object by format name or a filename
  Writer& mkWriter(const std::string& format_name);


}

#endif
