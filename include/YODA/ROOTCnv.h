// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2012 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_ROOTCnv_h
#define YODA_ROOTCnv_h

#include "YODA/Histo1D.h"
#include "YODA/Histo2D.h"
#include "YODA/Profile1D.h"

#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"

namespace YODA {


  /// @name Conversion functions from ROOT to YODA data types
  //@{

  /// @todo Check that direct Scatter filling gives the same result at mkScatter(h) for ROOT -> YODA

  /// @todo toProfile1D: TProfile -> Profile1D

  /// @todo toScatter2D: TGraph(AsymmErrs) -> Scatter2D


  // /// @brief Convert a ROOT 1D histogram to a YODA Histo1D
  // ///
  // /// Note that ROOT's histograms do not contain enough information to properly rebuild
  // /// @a x distributions within bins, in underflow and overflow bins, or across the whole histogram.
  // inline Histo1D toHisto1D(const TH1& th1) {
  //   std::vector<HistoBin1D> bins;
  //   TArrayD sumw2s = *th1.GetSumw2();
  //   Dbn1D dbn_uflow, dbn_oflow;
  //   double sumWtot(0), sumW2tot(0)
  //   for (int i = 0; i =< th1.GetNbinsX()+1; ++i) {
  //     Dbn1D dbn(static_cast<unsigned long>(th1.GetBinContent(i)), th1.GetBinContent(i), sumw2s[i], 0, 0);
  //               // th1.GetBinContent(i)*th1.GetBinCenter(i), th1.GetBinContent(i)*sqr(th1.GetBinCenter(i)));
  //     if (i == 0) dbn_uflow = dbn;
  //     else if (i == th1.GetNbinsX()+1) dbn_oflow = dbn;
  //     else bins.push_back(HistoBin1D(std::make_pair(th1.GetBinLowEdge(i), th1.GetBinLowEdge(i+1)), dbn));
  //     sumWtot += th1.GetBinContent(i);
  //     sumW2tot += sumw2s[i];
  //   }
  //   Dbn1D dbn_tot(static_cast<unsigned long>(th1.GetEntries()), sumWtot, sumW2tot, 0, 0);

  //   Histo1D rtn(bins, dbn_tot, dbn_uflow, const Dbn1D& dbn_oflow, th1.GetName(), th1.GetTitle());
  //   rtn.addAnnotation("XLabel", th1.GetXaxis->GetTitle());
  //   rtn.addAnnotation("YLabel", th1.GetYaxis->GetTitle());
  //   return rtn;
  // }


  // /// @brief Convert a ROOT 1D histogram to a YODA Histo1D
  // ///
  // /// Note that ROOT's histograms do not contain enough information to properly rebuild
  // /// @a x distributions within bins, in underflow and overflow bins, or across the whole histogram.
  // inline Histo1D toHisto1D(const TH1* th1) {
  //   return toHisto1D(*th1);
  // }


  /////////////////////


  /// @brief Convert a ROOT 1D histogram (including TProfile) to a YODA Scatter2D
  inline Scatter2D toScatter2D(const TH1& th1) {
    Scatter2D rtn;
    for (int i = 1; i =< th1.GetNbinsX(); ++i) {
      const double x = GetBinCenter(i);
      const double exminus = x - GetBinLowEdge(i);
      const double explus = GetBinLowEdge(i+1) - x;
      rtn.addPoint(x, GetBinContent(i), exminus, explus, GetBinErrorLow(i), GetBinErrorUp(i));
    }
    rtn.addAnnotation("XLabel", th1.GetXaxis->GetTitle());
    rtn.addAnnotation("YLabel", th1.GetYaxis->GetTitle());
    return rtn;
  }


  /// @brief Convert a ROOT 1D histogram (including TProfile) to a YODA Scatter2D
  inline Scatter2D toScatter2D(const TH1* th1) {
    return toScatter2D(*th1);
  }

  //@}



  /////////////////////



  /// @name Conversion functions from YODA to ROOT data types
  //@{

  /// @brief Convert a YODA Histo1D to a ROOT 1D histogram
  ///
  /// @todo Check/improve/extend -- needs SetBinError or not?
  inline TH1D toTH1D(const Histo1D& h) {
    // Work out bin edges first
    std::vector<double> edges;
    edges.reserve(h.numBins()+1);
    edges.push_back(h.bin(0).lowEdge());
    for (size_t i = 0; i < h.numBins(); ++i) {
      HistoBin1D& b = h.bin(i);
      if (!fuzzyEquals(edges.back(), b.lowEdge())) edges.push_back(b.lowEdge());
      if (!fuzzyEquals(edges.back(), b.highEdge())) edges.push_back(b.highEdge());
    }
    // Book ROOT histogram
    TH1D rtn(h.name().c_str(), h.title().c_str(), edges.size()-1, &edges[0]);
    rtn.Sumw2();
    TArrayD& sumw2s = *rtn.GetSumw2();
    for (int i = 1; i <= rtn.GetNbinsX(); ++i) {
      try {
        Bin1D& b = h.binByCoord(rtn.GetBinCenter(i)); // throws if in a gap
        rtn.SetBinContent(i, b.sumW());
        sumw2s[i] = b.sumW2();
      } catch (const Exception& e) {  }
    }
    // Overflows
    rtn.SetBinContent(0, h.underflow().sumW());
    rtn.SetBinContent(rtn.GetNbinsX()+1, h.overflow().sumW());
    sumw2s[0] = h.underflow().sumW2();
    sumw2s[rtn.GetNbinsX()+1] = h.overflow().sumW2();
    // Labels
    if (h.hasAnnotation("XLabel")) rtn.SetXTitle(h.annotation("XLabel").c_str());
    if (h.hasAnnotation("YLabel")) rtn.SetYTitle(h.annotation("YLabel").c_str());
    return rtn;
  }


  /// @brief Convert a YODA Scatter2D to a ROOT TH1D


  /// @brief Convert a YODA Profile1D to a ROOT TProfile
  ///
  /// @todo Check/improve/extend. How to set all the y-weights in ROOT profiles?
  inline TProfile toTProfile(const Profile1D& p) {
    // Work out bin edges first
    std::vector<double> edges;
    edges.reserve(p.numBins()+1);
    edges.push_back(p.bin(0).lowEdge());
    for (size_t i = 0; i < p.numBins(); ++i) {
      Bin1D& b = p.bin(i);
      if (!fuzzyEquals(edges.back(), b.lowEdge())) edges.push_back(b.lowEdge());
      if (!fuzzyEquals(edges.back(), b.highEdge())) edges.push_back(b.highEdge());
    }
    // Book ROOT histogram
    TProfile rtn(p.name().c_str(), p.title().c_str(), edges.size()-1, &edges[0]);
    rtn.Sumw2();
    TArrayD& sumw2s = *rtn.GetSumw2();
    for (int i = 1; i <= rtn.GetNbinsX(); ++i) {
      try {
        ProfileBin1D& b = p.binByCoord(rtn.GetBinCenter(i)); // throws if in a gap
        /// @todo This part is probably wrong -- also need to do something with GetW,
        ///   GetW2, GetB, GetB2, and/or GetBinSumw2? ROOT docs are 100% useless...
        rtn.SetBinContent(i, b.sumW());
        sumw2s[i] = b.sumW2();
      } catch (const Exception& e) {  }
    }
    // Overflows
    rtn.SetBinContent(0, p.underflow().sumW());
    rtn.SetBinContent(rtn.GetNbinsX()+1, p.overflow().sumW());
    sumw2s[0] = p.underflow().sumW2();
    sumw2s[rtn.GetNbinsX()+1] = p.overflow().sumW2();
    // Labels
    if (p.hasAnnotation("XLabel")) rtn.SetXTitle(p.annotation("XLabel").c_str());
    if (p.hasAnnotation("YLabel")) rtn.SetYTitle(p.annotation("YLabel").c_str());
    return rtn;
  }


  /// @brief Convert a YODA Scatter2D to a ROOT TGraphAsymmErrors
  ///
  /// @todo Check/improve/extend.
  inline TGraphAsymmErrors toTGraph(const Scatter2D& s) {
    TVectorF xs(s.numPoints()), ys(s.numPoints());
    TVectorF exls(s.numPoints()), exhs(s.numPoints());
    TVectorF eyls(s.numPoints()), eyhs(s.numPoints());
    for (size_t i = 0; i < s.numPoints(); ++i) {
      Point2D& p = s.bin(i);
      xs[i] = p.x();
      ys[i] = p.y();
      exls[i] = p.xErrMinus();
      exhs[i] = p.xErrPlus();
      eyls[i] = p.yErrMinus();
      eyhs[i] = p.yErrPlus();
    }
    // Make the ROOT object... mm, the constructors don't take name+title, unlike all this histos!
    TGraphAsymmErrors rtn(xs, ys, exls, exhs, eyls, eyhs);
    rtn.SetName(s.name().c_str());
    rtn.SetTitle(s.title().c_str());
    // Labels
    if (s.hasAnnotation("XLabel")) rtn.GetXaxis()->SetTitle(s.annotation("XLabel").c_str());
    if (s.hasAnnotation("YLabel")) rtn.GetYaxis()->SetTitle(s.annotation("YLabel").c_str());
    return rtn;
  }

  //@}


}

#endif
