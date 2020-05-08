import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
#mylist = FileUtils.loadListFromFile ('ttbarfiles_bdecays_gensim.txt') 
#readFiles = cms.untracked.vstring( *mylist)

process = cms.Process("Demo")

# Load the standard set of configuration modules
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

# global tag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_2016SeptRepro_v5')
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_2016_TrancheIV_v6')

# message logger
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

# set input to process
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    #fileNames = readFiles
    fileNames = cms.untracked.vstring('file:9E49EF05-55B6-EB46-9E53-5B48A528C76D.root')
#"/store/mc/RunIISummer16MiniAODv2/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/80000/FED04F50-A3BE-E611-A893-0025900E3502.root",
#'file:/eos/cms/store/cmst3/user/gvonsem/SampleProduction/TTTo2L2Nu_CustomBdecays_TuneCP5_13TeV-powheg-pythia8/TTTo2L2Nu_CustomBdecays_TuneCP5_13TeV-powheg-pythia8_cfg_RECO94X/200107_090642/0000/output_step2_1.root')
#'file:output_step2_1.root')
#'file:TTTo2L2Nu_CustomBdecays_TuneCP5_13TeV-powheg-pythia8_1.root')
)

process.demo = cms.EDAnalyzer('TopNtupler',
#genParticles = cms.InputTag("genParticles"),
pruned = cms.InputTag("prunedGenParticles"),
vertices = cms.InputTag('offlineSlimmedPrimaryVertices'),
electrons = cms.InputTag("gedGsfElectrons"),
jets = cms.InputTag("slimmedJets")
)

#Tfile service
process.TFileService = cms.Service("TFileService",
                                   #fileName = cms.string('ttbarsignal_10312019.root')
                                   fileName = cms.string('ttbar_bdecays_UL2018_test.root')
                                   )

process.p = cms.Path(process.demo)
