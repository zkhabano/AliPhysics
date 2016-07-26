#ifndef AliDhCorrelationExtraction_H
#define AliDhCorrelationExtraction_H

/**************************************************************************
 * Copyright(c) 1998-2016, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

//
//  Base class to extract D-h correlation distribution from analysis task
//
//-----------------------------------------------------------------------
//  Author F.Colamaria
//  INFN Bari
//  fabio.colamaria@cern.ch
//-----------------------------------------------------------------------

#include <iostream>
#include "TObject.h"
#include "TMath.h"
#include "TFile.h"
#include "TDirectoryFile.h"
#include "TList.h"
#include "TCanvas.h"
#include "TPaveText.h"
#include "TLegend.h"
#include "TSystem.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TF1.h"
#include "THnSparse.h"
#include "AliHFMassFitter.h"

class AliDhCorrelationExtraction : public TObject
{

public:
    
    enum DMesonSpecies {kD0toKpi, kDplusKpipi, kDStarD0pi};
    enum SandBextraction {kSandBFromFit, kSfromBinCount, kBfromBinCount}; //default for paper: kBfromBinCount
    enum SBscaling {kFitScaling, kBinCountScaling}; //default for paper: kBfromBinCount
    enum selectAnType {kSE, kME};
    enum selectRegion {kSign, kSideb};  

    AliDhCorrelationExtraction(); // default constructor
    AliDhCorrelationExtraction(const AliDhCorrelationExtraction &source);
    virtual ~AliDhCorrelationExtraction();

    Bool_t SetDmesonSpecie(DMesonSpecies k);
    void SetSandBextraction(SandBextraction k) {fSandBextraction=k;}
    void SetSBscaling(SBscaling k) {fSBscaling=k;}
    void SetInputFilename(TString filename) {fFileName=filename;}
    void SetListMassName(TString listMassName) {fListMassName=listMassName;}
    void SetDirNameSE(TString dirNameSE) {fDirNameSE=dirNameSE;}
    void SetListNameSE(TString listNameSE) {fListNameSE=listNameSE;}
    void SetDirNameME(TString dirNameME) {fDirNameME=dirNameME;}
    void SetListNameME(TString listNameME) {fListNameME=listNameME;}
    void SetMassHistoName(TString massHistoName) {fMassHistoName=massHistoName;}
    void SetSECorrelHistoName(TString correlName) {fSECorrelHistoName=correlName;}
    void SetSECorrelHistoName_DstarBkg(TString correlName_DstarBkg) {fSECorrelHistoName_DstarBkg=correlName_DstarBkg;}
    void SetMECorrelHistoNameSuffix(TString suffix) {fMEsuffix=suffix;}
    void IntegratePtBins(Bool_t intPt=kFALSE) {fIntegratePtBins=intPt;}

    void SetReadMassesOnly(Bool_t readMassOnly) {fReadMassesOnly=readMassOnly;}
    void SetRebinMassPlots(Int_t rebinMassPlots) {fRebinMassPlots=rebinMassPlots;}
    void SetNpTbins(Int_t npt) {fNpTbins=npt;}
    void SetFirstpTbin(Int_t ptFirst) {fFirstpTbin=ptFirst;}
    void SetLastpTbin(Int_t ptLast) {fLastpTbin=ptLast;}
    void SetNumberOfSigmasFitter(Double_t nsigma) {fNumberOfSigmasFitter=nsigma;}
    void SetCorrectPoolsSeparately(Bool_t usePools) {fCorrectPoolsSeparately=usePools;}
    void SetNpools(Int_t npools) {fNpools=npools;}
    void SetDeltaEtaRange(Double_t etaLow=-1., Double_t etaHigh=1) {fDeltaEtaMin=etaLow; fDeltaEtaMax=etaHigh;}

    void SetFitRanges(Double_t left, Double_t right) {fLeftFitRange=left; fRightFitRange=right;}
    void SetBkgFitFunction(Int_t func=0) {fBkgFitFunction=func;}
    void SetSignalSigmas(Double_t nsigma=2) {fSignalSigmas=nsigma;}
    void SetAutoSBRange(Bool_t autoSB=kFALSE, Double_t inSigma=0., Double_t outSigma=0.) {fAutoSBRange=autoSB; fSBOuterSigmas=outSigma; fSBInnerSigmas=inSigma;}
    void SetSBSingleBin(Bool_t singleSBbin=kFALSE) {fSBSingleBin=singleSBbin;}
    void SetSBRanges(Double_t* rangesSB1L=0x0, Double_t* rangesSB1R=0x0, Double_t* rangesSB2L=0x0, Double_t* rangesSB2R=0x0);
    void PrintRanges();
    void PrintSandBForNormal();
    void GetSignalAndBackgroundForNorm(Int_t i, TH1F* &histo);  //evaluates signal and background in 'fSignalSigmas', for trigger normalization and SB correlation rescaling
    void GetSBScalingFactor(Int_t i, TH1F* &histo); //estract sideband scaling factor
    TH2D* GetCorrelHisto(Int_t SEorME, Int_t SorSB, Int_t pool, Int_t pTbin, Double_t thrMin, Double_t thrMax);
    TH2D* GetCorrelHistoDzero(Int_t SEorME, Int_t SorSB, Int_t pool, Int_t pTbin, Double_t thrMin, Double_t thrMax);
    TH2D* GetCorrelHistoDplus(Int_t SEorME, Int_t SorSB, Int_t pool, Int_t pTbin, Double_t thrMin, Double_t thrMax);
    TH2D* GetCorrelHistoDstar(Int_t SEorME, Int_t SorSB, Int_t pool, Int_t pTbin, Double_t thrMin, Double_t thrMax);
    void NormalizeMEplot(TH2D* &histoME); //normalize ME plots to the average value of the 4 'central' bins
    void RescaleSidebandsInMassBinEdges(Int_t i); //readjust SB scaling factor if single bin is used & ranges passed from outside & ranges don't match bin edges
    void MergeMassPlotsVsPt(); //merge spectra from mass-pT bins in larger correlation-pT bins (as if you have a single pT bin)
    void MergeCorrelPlotsVsPt(THnSparse* &hsparse, Int_t SEorME, Int_t SorSB=0, Int_t pool=0); //merge THnSparse from mass-pT bins in larger correlation-pT bins (as if you have a single pT bin)

    void ClearObjects();

    void SetDebugLevel(Int_t debug) {fDebug=debug;}
    void SetHistoStyle(TH1F* &histo, Int_t colour);
    void SetHistoCorrStyle(TH1D* &histo);
    void DefinePaveText(TPaveText* &paveText, Double_t xmin, Double_t ymin, Double_t xmax, Double_t ymax, TString name);

    Bool_t ReadInputs(); //reads input files and loads lists of plots
    Bool_t FitInvariantMass(); //method to perform invariant mass fit via AliHFMassFitter
    Bool_t ExtractCorrelations(Double_t thrMin, Double_t thrMax); //method to retrieve the bkg subtracted and ME corrected correlation distributions

private:
    
    TFile *fFile; //file containing the analysis output
    
    TDirectoryFile *fDirSE; // TDirectory for SE info
    TDirectoryFile *fDirME; // TDirectory for ME info
    
    TList *fMassList; // TList with D mass
    TList *fTracksList; // TList with track properties
    TList *fSECorrelationList; // TList with Correlations from SE
    TList *fMECorrelationList; // TList with Correlations from ME

    DMesonSpecies fDmesonSpecies;
    SandBextraction fSandBextraction;
    SBscaling fSBscaling;
    TString fDmesonLabel;
    TString fFileName; 
    TString fDirNameSE; 
    TString fListNameSE; 
    TString fDirNameME; 
    TString fListNameME;
    TString fListMassName;
    TString fMassHistoName;
    TString fSECorrelHistoName;
    TString fSECorrelHistoName_DstarBkg;
    TString fMEsuffix;

    Bool_t fReadMassesOnly;
    Int_t fRebinMassPlots;    
    Int_t fNpTbins;
    Int_t fFirstpTbin;
    Int_t fLastpTbin;
    Double_t fNumberOfSigmasFitter;
    Bool_t fCorrectPoolsSeparately;
    Int_t fNpools;

    Double_t *fDmesonFitterSignal;
    Double_t *fDmesonFitterSignalError;
    Double_t *fDmesonFitterBackground;
    Double_t *fDmesonFitterBackgroundError;
    Double_t *fDMesonFitterSBCand;
    Double_t *fDMesonFitterSBCandErr;
    Double_t *fDmesonFitterMean;
    Double_t *fDmesonFitterMeanError;
    Double_t *fDmesonFitterSigma;
    Double_t *fDmesonFitterSigmaError;
    Double_t *fDmesonFitterSignificance;
    Double_t *fDmesonFitterSignificanceError;
    Double_t *fDmesonFitterSOverB;

    Double_t fLeftFitRange;
    Double_t fRightFitRange;
    Int_t fBkgFitFunction;
    Double_t fSignalSigmas;
    Bool_t fAutoSBRange;
    Double_t fSBOuterSigmas;
    Double_t fSBInnerSigmas;
    Bool_t fSBSingleBin;
    Double_t fDeltaEtaMin;
    Double_t fDeltaEtaMax;

    Double_t *fSignalCorrel;
    Double_t *fBackgrCorrel;
    Double_t *fRangesSignL;
    Double_t *fRangesSignR;
    Double_t *fRangesSB1L;
    Double_t *fRangesSB1R;
    Double_t *fRangesSB2L;
    Double_t *fRangesSB2R;
    Double_t *fScaleFactor;

    Bool_t fIntegratePtBins;

    Int_t fDebug;

    TF1 **fMassFit;
    TF1 **fBkgFit;

    TH1F **fMassHisto;

    ClassDef(AliDhCorrelationExtraction,1); // class for plotting HF correlations

};

#endif

