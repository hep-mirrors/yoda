// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2010 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_Writer_h
#define YODA_Writer_h

#include "YODA/AnalysisObject.h"
#include "YODA/Histo1D.h"
#include "YODA/Profile1D.h"
// #include "YODA/Scatter2D.h"
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


    // /// @name Writing multiple analysis objects by collection.
    // //@{
    // /// Write out a collection of objects @a objs to output stream @a stream.
    // template <template<typename> class LIST>
    // void write(std::ostream& stream, const LIST<AnalysisObject>& aos) {
    //   write(stream, aos.begin(), aos.end());
    // }
    // /// Write out a collection of objects @a objs to file @a filename.
    // template <template<typename> class LIST>
    // void write(const std::string& filename, const LIST<AnalysisObject>& aos) {
    //   write(filename, aos.begin(), aos.end());
    // }
    // //@}


    /// @name Writing multiple analysis objects by iterator range.
    //@{

    /// Write out the objects specified by start iterator @a begin and end
    /// iterator @a end to output stream @a stream.
    template <typename ITER>
    void write(std::ostream& stream, const ITER& begin, const ITER& end) {
      writeHeader(stream);
      for (ITER ao = begin; ao != end; ++ao) {
        writeBody(stream, *ao);
        // if (!ok) {
        //   throw Exception("Error when writing to output stream");
        // }
      }
      writeFooter(stream);
    }

    /// Write out the objects specified by start iterator @a begin and end
    /// iterator @a end to file @a filename.
    template <typename ITER>
    void write(const std::string& filename,
               const ITER& begin,
               const ITER& end) {
      std::ofstream outstream;
      outstream.open(filename.c_str());
      write(outstream, begin, end);
      outstream.close();
    }

    //@}


  protected:

    /// Main writer elements
    virtual void writeHeader(std::ostream& stream) = 0;
    void writeBody(std::ostream& stream, const AnalysisObject& ao);
    virtual void writeFooter(std::ostream& stream) = 0;

    /// Specific AO type writer implementations
    virtual void writeHisto1D(std::ostream& os, const Histo1D& h, const std::string& path) = 0;
    virtual void writeProfile1D(std::ostream& os, const Profile1D& h, const std::string& path) = 0;
    // virtual void writeScatter2D(std::ostream& os, const Scatter2D& h, const std::string& path) = 0;

  };


}

#endif
