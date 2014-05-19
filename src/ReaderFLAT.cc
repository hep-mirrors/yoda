// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2014 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/ReaderFLAT.h"
#include "YODA/Utils/StringUtils.h"
#include "YODA/Exceptions.h"

#include "YODA/Counter.h"
#include "YODA/Histo1D.h"
#include "YODA/Histo2D.h"
#include "YODA/Profile1D.h"
#include "YODA/Profile2D.h"
// #include "YODA/Scatter1D.h"
#include "YODA/Scatter2D.h"
// #include "YODA/Scatter3D.h"

#include <iostream>
using namespace std;

namespace YODA {

  qi::symbols<char, int> ReaderFLAT::bgroup;
  qi::symbols<char, int> ReaderFLAT::egroup;
  ReaderFLAT::scatter2d ReaderFLAT::_scatter2d;
  std::map<std::string, std::string> ReaderFLAT::_annotations;


  void ReaderFLAT::_readDoc(std::istream& stream, vector<AnalysisObject*>& aos) {

    // These are the context groups we know. We need
    // that map to dynamically change the parser depending
    // on what we read in.
    std::map<int, std::string> groups;
    groups[1] = "HISTOGRAM";

    // Initialize the group parser
    std::pair <int, std::string> pis;  // To make boost's foreach happy
    foreach(pis, groups) {
      bgroup.add(pis.second, pis.first);
    }

    // The grammars for content (yoda) and context (group)
    yoda_grammar<std::string::iterator, ascii::space_type> yoda_parser;
    group_grammar<std::string::iterator> group_parser;

    // Now loop over all lines of the input file
    int context = 0;
    bool contextchange = false;
    std::string s;
    while (getline(stream, s)) {
      // First check if we found a "# BEGIN ..." or "# END ..." line.
      // This marks a context change.
      int newcontext = 0;
      // if (qi::parse(s.begin(), s.end(), group_parser, newcontext)) { //< Only supported in Boost 1.47+
      std::string::iterator it1 = s.begin();
      if (qi::parse(it1, s.end(), group_parser, newcontext)) { //< End patch

        context = newcontext;
        if (context > 0) {
          // We are inside a group now, so we are looking for the corresponding
          // END and ignore all BEGINs
          bgroup.clear();
          egroup.add(groups[context], -context);
        }
        if (context < 0) {
          // We are outside a group, so we are looking for any BEGIN and ignore
          // all ENDs
          egroup.remove(groups[-context]);
          foreach(pis, groups) {
            bgroup.add(pis.second, pis.first);
          }
          contextchange = true;
        }
      }

      // Depending on the context, we either want to parse the line as data,
      // or to write out what we parsed so far (when leaving a group).
      switch (context) {
        case 1:  // we are inside HISTOGRAM
          // if (! qi::phrase_parse(s.begin(), s.end(), yoda_parser, ascii::space) ) { //< Only supported in Boost 1.47+
          { //< Why the explicit scoping? Added by supplied patch from Andrii Verbytskyi
            std::string::iterator it2 = s.begin();
            if (! qi::phrase_parse(it2, s.end(), yoda_parser, ascii::space) ) { //< End patch
              std::cerr << "failed parsing this line:\n" << s << std::endl;
            }
          } //< End patch scoping
          break;
        case -1: // we left HISTOGRAM
          if (contextchange) {
            YODA::Scatter2D* h = new YODA::Scatter2D(_scatter2d.points);
            std::pair <std::string, std::string> pss;  // to make boost's foreach happy
            foreach (pss, _annotations) {
              h->setAnnotation(pss.first, pss.second);
            }
            aos.push_back(h);
            cleanup();
            contextchange = false;
          }
          break;
      }
    }
  }


}
