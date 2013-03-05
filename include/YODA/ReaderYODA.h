// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2013 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_READERYODA_H
#define YODA_READERYODA_H

#include "YODA/AnalysisObject.h"
#include "YODA/Reader.h"
#include <YODA/Histo1D.h>
#include <YODA/Profile1D.h>
#include <YODA/Scatter2D.h>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace YODA {

  using namespace boost::spirit;
  using namespace boost::phoenix;


  /// Persistency reader from YODA flat text data format.
  class ReaderYODA : public Reader {
  public:

    /// Singleton creation function
    static Reader& create() {
      static ReaderYODA _instance;
      return _instance;
    }

    void read(std::istream& stream, std::vector<AnalysisObject*>& aos) {
      _readDoc(stream, aos);
    }


    // Hide from Doxygen until endcond
    /// @cond


  protected:

    void _readDoc(std::istream& stream, std::vector<AnalysisObject*>& aos);

  private:

    void cleanup() {
      _histo1d.bins.clear();
      _histo1d.dbn_tot.reset();
      _histo1d.dbn_uflow.reset();
      _histo1d.dbn_oflow.reset();

      _profile1d.bins.clear();
      _profile1d.dbn_tot.reset();
      _profile1d.dbn_uflow.reset();
      _profile1d.dbn_oflow.reset();

      _scatter2d.points.clear();

      _annotations.clear();
    }

  public:

    /// Private constructor, since it's a singleton.
    /// @todo This is definitely not private!
    ReaderYODA() { }


    // Here comes everything we need for the parser

    /// Structs for the Histo1D parser
    /// The data for Dbn1D
    struct histo1ddbn {
      double sumW;
      double sumW2;
      double sumWX;
      double sumWX2;
      unsigned long numFills;
    };

    /// A Histo1D bin
    struct histo1dbin {
      double lowedge;
      double highedge;
      histo1ddbn dbn;
    };

    /// Structs for the Profile1D parser
    /// The data for Dbn2D (except sumWXY)
    struct profile1ddbn {
      double sumW;
      double sumW2;
      double sumWX;
      double sumWX2;
      double sumWY;
      double sumWY2;
      unsigned long numFills;
    };

    /// A Profile1D bin
    struct profile1dbin {
      double lowedge;
      double highedge;
      profile1ddbn dbn;
    };

    /// Structs for the Scatter2D parser
    /// The data for ScatterPoint2D
    struct scatterpoint2d {
      double x;
      double exminus;
      double explus;
      double y;
      double eyminus;
      double eyplus;
    };

    /// Structs for the key-value pair parser (annotations)
    struct keyval {
      std::string key;
      std::string val;
    };


    /// Annotations (common for all data types)
    static std::map<std::string, std::string> _annotations;


    /// All information for creating a Histo1D
    struct histo1d {
      std::vector<YODA::HistoBin1D> bins;
      YODA::Dbn1D dbn_tot;
      YODA::Dbn1D dbn_uflow;
      YODA::Dbn1D dbn_oflow;
    };
    static histo1d _histo1d;


    /// All information for creating a Profile1D
    struct profile1d {
      std::vector<YODA::ProfileBin1D> bins;
      YODA::Dbn2D dbn_tot;
      YODA::Dbn2D dbn_uflow;
      YODA::Dbn2D dbn_oflow;
    };
    static profile1d _profile1d;


    /// All information for creating a Scatter2D
    struct scatter2d {
      std::vector<YODA::Point2D> points;
    };
    static scatter2d _scatter2d;


    /// Functions to call from the parser
    /// Filling the dbn_tot
    struct filltotaldbn {
      void operator()(const histo1ddbn dbn, qi::unused_type, qi::unused_type) const {
        _histo1d.dbn_tot = YODA::Dbn1D(dbn.numFills, dbn.sumW, dbn.sumW2, dbn.sumWX, dbn.sumWX2);
      }
      void operator()(const profile1ddbn dbn, qi::unused_type, qi::unused_type) const {
        _profile1d.dbn_tot = YODA::Dbn2D(dbn.numFills, dbn.sumW, dbn.sumW2, dbn.sumWX, dbn.sumWX2, dbn.sumWY, dbn.sumWY2, 0.0);
      }
    };


    /// Filling the underflow
    struct filluflowdbn {
      void operator()(const histo1ddbn dbn, qi::unused_type, qi::unused_type) const {
        _histo1d.dbn_uflow = YODA::Dbn1D(dbn.numFills, dbn.sumW, dbn.sumW2, dbn.sumWX, dbn.sumWX2);
      }
      void operator()(const profile1ddbn dbn, qi::unused_type, qi::unused_type) const {
        _profile1d.dbn_uflow = YODA::Dbn2D(dbn.numFills, dbn.sumW, dbn.sumW2, dbn.sumWX, dbn.sumWX2, dbn.sumWY, dbn.sumWY2, 0.0);
      }
    };


    /// Filling the overflow
    struct filloflowdbn {
      void operator()(const histo1ddbn dbn, qi::unused_type, qi::unused_type) const {
        _histo1d.dbn_oflow = YODA::Dbn1D(dbn.numFills, dbn.sumW, dbn.sumW2, dbn.sumWX, dbn.sumWX2);
      }
      void operator()(const profile1ddbn dbn, qi::unused_type, qi::unused_type) const {
        _profile1d.dbn_oflow = YODA::Dbn2D(dbn.numFills, dbn.sumW, dbn.sumW2, dbn.sumWX, dbn.sumWX2, dbn.sumWY, dbn.sumWY2, 0.0);
      }
    };


    /// Filling a bin
    struct fillbin {
      void operator()(const histo1dbin b, qi::unused_type, qi::unused_type) const {
        YODA::HistoBin1D bin(std::make_pair(b.lowedge, b.highedge), YODA::Dbn1D(b.dbn.numFills, b.dbn.sumW, b.dbn.sumW2, b.dbn.sumWX, b.dbn.sumWX2));
        _histo1d.bins.push_back(bin);
      }
      void operator()(const profile1dbin b, qi::unused_type, qi::unused_type) const {
        YODA::ProfileBin1D bin(std::make_pair(b.lowedge, b.highedge), YODA::Dbn2D(b.dbn.numFills, b.dbn.sumW, b.dbn.sumW2, b.dbn.sumWX, b.dbn.sumWX2, b.dbn.sumWY, b.dbn.sumWY2, 0.0));
        _profile1d.bins.push_back(bin);
      }
    };


    /// Filling a point
    struct fillpoint {
      void operator()(const scatterpoint2d p, qi::unused_type, qi::unused_type) const {
        YODA::Point2D point(p.x, p.y, p.exminus, p.explus, p.eyminus, p.eyplus);
        _scatter2d.points.push_back(point);
      }
    };


    /// Filling the annotations map
    struct fillkeyval {
      void operator()(const keyval m, qi::unused_type, qi::unused_type) const {
        _annotations[m.key] = m.val;
      }
    };


    /// A helper grammar for determining in which context we are.
    /// bgroup and egroup are dynamic parsers. We add and remove
    /// the "# BEGIN SOMETHING" and "# END SOMETHING" strings
    /// dynamically.
    static qi::symbols<char, int> bgroup;
    static qi::symbols<char, int> egroup;
    template <typename Iterator>
    struct group_grammar : qi::grammar<Iterator, int()>
    {
      group_grammar() : group_grammar::base_type(start) {
        start = begin | end;
        begin = qi::eps [_val = 0]  >>
                qi::lit("# BEGIN ") >>
                bgroup  [_val += _1];
        end   = qi::eps [_val = 0]  >>
                qi::lit("# END ")   >>
                egroup  [_val += _1];
      }
      qi::rule<Iterator, int()> start, begin, end;
    };


    /// The actual grammar for parsing the lines of a flat data file.
    template <typename Iterator, typename Skipper>
    struct yoda_grammar : qi::grammar<Iterator, Skipper>
    {

      yoda_grammar() : yoda_grammar::base_type(line) {

        /// A line can be anything. Note that we need
        /// to specify the long lines first, because the
        /// first match wins.
        /// In brackets we specify the functions that are
        /// called in case the rule matches.
        line = Profile1Dbin[fillbin()]         |
               Profile1Dtotal[filltotaldbn()]  |
               Profile1Duflow[filluflowdbn()]  |
               Profile1Doflow[filloflowdbn()]  |
               Histo1Dbin[fillbin()]           |
               Histo1Dtotal[filltotaldbn()]    |
               Histo1Duflow[filluflowdbn()]    |
               Histo1Doflow[filloflowdbn()]    |
               ScatterPoint2D[fillpoint()]     |
               keyvaluepair[fillkeyval()]      |
               comment;

        /// Match the strings "Underflow", "Overflow" and "Total"
        underflow = qi::lit("Underflow");
        overflow  = qi::lit("Overflow");
        total     = qi::lit("Total");

        /// Histo1D
        /// Regular bins, total statistics, underflow or overflow.
        Histo1Dbin   %= double_   >> double_   >> Histo1Ddbn;
        Histo1Dtotal %= total     >> total     >> Histo1Ddbn;
        Histo1Duflow %= underflow >> underflow >> Histo1Ddbn;
        Histo1Doflow %= overflow  >> overflow  >> Histo1Ddbn;
        Histo1Ddbn = double_ >> double_ >> double_ >> double_ >> ulong_;

        // Histo2D

        // Profile1D
        /// Regular bins, total statistics, underflow or overflow.
        Profile1Dbin   %= double_   >> double_   >> Profile1Ddbn;
        Profile1Dtotal %= total     >> total     >> Profile1Ddbn;
        Profile1Duflow %= underflow >> underflow >> Profile1Ddbn;
        Profile1Doflow %= overflow  >> overflow  >> Profile1Ddbn;
        Profile1Ddbn = double_ >> double_ >> double_ >> double_ >> double_ >> double_ >> ulong_;

        // Profile2D

        // Scatter1D

        // Scatter2D
        ScatterPoint2D %= double_ >> double_ >> double_ >> double_ >> double_ >> double_;

        // Scatter3D


        /// Annotations.
        /// The key is anyting up to the first equal sign, but
        /// keys can't start with a number or a minus sign. The
        /// value is anything after the equal sign.
        key = !qi::char_("0-9-") >> *~qi::char_("=");
        value = *~qi::char_("\n");
        keyvaluepair %= key >> "=" >> value;

        /// Lines starting with a "#" are comments.
        comment = qi::lit("#") >> *~qi::char_("\n");
      }

      /// In the rules, the first template argument is the Iterator for the string,
      /// the second one is the return type, and the last one is a "Skipper" for
      /// ignoring whitespace. Note that the key/value pair doesn't ignore whitespace.
      /// Most of the return values match our structs (like keyval, histo1dbin, etc.).
      /// Those are used to directly fill the corresponding structs.
      qi::rule<Iterator, Skipper> line, total, underflow, overflow, comment;
      qi::rule<Iterator, std::string()> key, value;
      qi::rule<Iterator, keyval(), Skipper> keyvaluepair;

      qi::rule<Iterator, histo1dbin(), Skipper> Histo1Dbin;
      qi::rule<Iterator, histo1ddbn(), Skipper> Histo1Ddbn, Histo1Dtotal, Histo1Duflow, Histo1Doflow;

      qi::rule<Iterator, profile1dbin(), Skipper> Profile1Dbin;
      qi::rule<Iterator, profile1ddbn(), Skipper> Profile1Ddbn, Profile1Dtotal, Profile1Duflow, Profile1Doflow;

      qi::rule<Iterator, scatterpoint2d(), Skipper> ScatterPoint2D;
    };


    /// @endcond

  };

} // end of YODA namespace



// Now we need to make boost aware of the structs we want to
// fill directly from the parser. Boost wants this magic in
// the global scope, that's why we have it outside the namespace.

/// @cond PRIVATE

BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderYODA::histo1ddbn,
  (double, sumW)
  (double, sumW2)
  (double, sumWX)
  (double, sumWX2)
  (unsigned long, numFills)
)

BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderYODA::histo1dbin,
  (double, lowedge)
  (double, highedge)
  (YODA::ReaderYODA::histo1ddbn, dbn)
)

BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderYODA::profile1ddbn,
  (double, sumW)
  (double, sumW2)
  (double, sumWX)
  (double, sumWX2)
  (double, sumWY)
  (double, sumWY2)
  (unsigned long, numFills)
)

BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderYODA::profile1dbin,
  (double, lowedge)
  (double, highedge)
  (YODA::ReaderYODA::profile1ddbn, dbn)
)

BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderYODA::scatterpoint2d,
  (double, x)
  (double, exminus)
  (double, explus)
  (double, y)
  (double, eyminus)
  (double, eyplus)
)

BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderYODA::keyval,
  (std::string, key)
  (std::string, val)
)

/// @endcond

#endif
