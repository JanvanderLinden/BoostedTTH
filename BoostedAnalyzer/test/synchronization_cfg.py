import FWCore.ParameterSet.Config as cms

process = cms.Process("boosted")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.load('Configuration.StandardSequences.Services_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'PHYS14_25_V2::All'

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5000) )

from JetMETCorrections.Configuration.JetCorrectionServices_cff import *
#from JetMETCorrections.Configuration.JetCorrectionCondDB_cff import *

process.ak4PFCHSL1Fastjet = cms.ESProducer(
  'L1FastjetCorrectionESProducer',
  level = cms.string('L1FastJet'),
  algorithm = cms.string('AK4PFchs'),
  srcRho = cms.InputTag( 'fixedGridRhoFastjetAll' )
  )
process.ak4PFchsL2Relative = ak4CaloL2Relative.clone( algorithm = 'AK4PFchs' )
process.ak4PFchsL3Absolute = ak4CaloL3Absolute.clone( algorithm = 'AK4PFchs' )
process.ak4PFchsL1L2L3 = cms.ESProducer("JetCorrectionESChain",
  correctors = cms.vstring(
    'ak4PFCHSL1Fastjet',
    'ak4PFchsL2Relative',
    'ak4PFchsL3Absolute')
)


process.source = cms.Source(  "PoolSource",
                              fileNames = cms.untracked.vstring('file:/pnfs/desy.de/cms/tier2/store/user/hmildner/ttjets_phys14_20bx25_withfatjets_v2.root')
)

process.load("BoostedTTH.BoostedAnalyzer.BoostedAnalyzer_cfi")
process.BoostedAnalyzer.useFatJets=True
process.BoostedAnalyzer.disableObjectSelections=True
process.BoostedAnalyzer.outfileName = "Synchronization"
process.BoostedAnalyzer.selectionNames = ["SynchSelection"]
process.BoostedAnalyzer.processorNames = []


process.p = cms.Path(process.BoostedAnalyzer)