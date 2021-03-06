import FWCore.ParameterSet.Config as cms

import os

from tthAnalysis.HiggsToTauTau.makePlots_cfi import process

process.makePlots.processesBackground = cms.vstring(
    ##"TT",
    "TTW",
    "TTZ",
    "EWK",
    "Rares",
    "fakes_data",
    "flips_data"
)
process.makePlots.processSignal = cms.string("signal")

process.makePlots.categories = cms.VPSet(
    cms.PSet(
        name = cms.string("2lss_1tau_SS_Tight"),
        label = cms.string("2lss_1tau")
    )
)

process.makePlots.distributions.extend([
    cms.PSet(
        histogramName = cms.string("sel/evt/$PROCESS/mvaDiscr_2lss"),
        xAxisTitle = cms.string("MVA Discriminant"),
        yAxisTitle = cms.string("N")
    ),
    cms.PSet(
        histogramName = cms.string("sel/evt/$PROCESS/mvaDiscr_2lss_1tau"),
        xAxisTitle = cms.string("MVA Discriminant"),
        yAxisTitle = cms.string("N")
    ),
    cms.PSet(
        histogramName = cms.string("sel/evt/$PROCESS/mTauTauVis"),
        xAxisTitle = cms.string("m_{#tau#tau}^{vis} [GeV]"),
        yAxisTitle = cms.string("dN/dm_{#tau#tau}^{vis} [1/GeV]")
    )
])

process.makePlots.nuisanceParameters.normalization = cms.PSet(
    signal = cms.string("1.0 +/- 0.20"),
    TT = cms.string("1.0 +/- 0.20"),
    TTW = cms.string("1.0 +/- 0.20"),
    TTWW = cms.string("1.0 +/- 0.20"),
    TTZ = cms.string("1.0 +/- 0.20"),
    EWK = cms.string("1.0 +/- 0.20"),
    Rares = cms.string("1.0 +/- 0.20"),
    fakes_data = cms.string("1.0 +/- 0.20"),
    flips_data = cms.string("1.0 +/- 0.20")
)


