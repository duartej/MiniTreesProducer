# import skeleton process
##from PhysicsTools.PatAlgos.patTemplate_cfg import *  --> problema con el pickRelValInputFiles del source
## lo cargo a mano
## ----> PhysicsTools.PatAlgos.patTemplate_cfg :
import FWCore.ParameterSet.Config as cms

process = cms.Process("PAT")

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")

## Options and Output Report
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

## Source
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import pickRelValInputFiles
process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(''
#    pickRelValInputFiles( cmsswVersion  = 'CMSSW_4_1_2'
#                       , relVal        = 'RelValTTbar'
#                      , globalTag     = 'START311_V2'
#                        , numberOfFiles = 1
#                        )
    )
)
## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.PyReleaseValidation.autoCond import autoCond
process.GlobalTag.globaltag = cms.string( autoCond[ 'startup' ] )
process.load("Configuration.StandardSequences.MagneticField_cff")

## Standard PAT Configuration File
process.load("PhysicsTools.PatAlgos.patSequences_cff")

## Output Module Configuration (expects a path 'p')
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('patTuple.root'),
                               # save only events passing the full path
                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
                               # save PAT Layer 1 output; you need a '*' to
                               # unpack the list of commands 'patEventContent'
                               outputCommands = cms.untracked.vstring('drop *', *patEventContent )
                               )

process.outpath = cms.EndPath(process.out)
#------------------- END patTemplate_cfg

process.load('HiggsAnalysis/Skimming/higgsToWW2Leptons_Sequences_cff')

from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *

# load the standard PAT config 
process.load("PhysicsTools.PatAlgos.patSequences_cff")


# Configure PAT to use PF2PAT instead of AOD sources
# this function will modify the PAT sequences. It is currently
# not possible to run PF2PAT+PAT and standart PAT at the same time
from PhysicsTools.PatAlgos.tools.pfTools import *
#process.pfIsolatedMuons.combinedIsolationCut = cms.double(0.25)
#process.load("RecoTauTag.Configuration.RecoPFTauTag_cff")  # --> Added
postfix = "PF"
usePF2PAT(process,runPF2PAT=True, jetAlgo='AK5', runOnMC=False, postfix=postfix)

# turn to false when running on data
getattr(process, "patElectrons"+postfix).embedGenMatch = False
getattr(process, "patMuons"+postfix).embedGenMatch = False

# in PAT (iterativeCone5) to ak5 (anti-kt cone = 0.5)
switchJetCollection(process,
                 cms.InputTag('ak5CaloJets'),
                 doJTA            = True,
                 doBTagging       = True,
                 jetCorrLabel     = ('AK5Calo',cms.vstring(['L1Offset','L2Relative', 'L3Absolute','L2L3Residual'])),
                 doType1MET       = True,
                 genJetCollection = cms.InputTag("ak5GenJets"),
                 doJetID          = True,
                 jetIdLabel       = "ak5"
                        )

from PhysicsTools.PatAlgos.tools.coreTools import *
removeMCMatching(process, ['All'])
removeSpecificPATObjects(process,['Taus'])#,'Photons'])

from PhysicsTools.PatAlgos.tools.metTools import *
addTcMET(process, 'TC')

# uncomment the following line to add tcMET to the event content
#from PhysicsTools.PatAlgos.tools.metTools import *
#addTcMET(process, 'TC')
#addPfMET(process, 'PF')

#### special TrackJets
process.load('CommonTools.RecoAlgos.TrackWithVertexSelectorParams_cff')
process.trackWithVertexSelectorParams.ptMin = cms.double(1.0)
process.trackWithVertexSelectorParams.ptMax =  cms.double(7000)
#process.trackWithVertexSelectorParams.dzMax = cms.double(0.2)
process.trackWithVertexSelectorParams.zetaVtx = cms.double(0.2)
#process.trackWithVertexSelectorParams.rhoVtx = cms.double(2.001)
process.trackWithVertexSelectorParams.useVtx = cms.bool(True)
process.trackWithVertexSelectorParams.nVertices = cms.uint32(1) ## how many vertices to look at before dropping the track

process.load('CommonTools.RecoAlgos.TrackWithVertexRefSelector_cfi')
process.load('RecoJets.JetProducers.TracksForJets_cff')
process.load('RecoJets.JetProducers.ak5TrackJets_cfi')
process.ak5TrackJets.src=cms.InputTag("trackRefsForJets::PAT")

    
### Jets part    
from PhysicsTools.PatAlgos.tools.jetTools import *
#switchJECSet( process, "Summer09_7TeV_ReReco332")
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

addJetCollection(process,cms.InputTag('ak5TrackJets::PAT'), 'AK5','TJ',
                        doJTA=True,doBTagging=True,jetCorrLabel=('AK5TRK',cms.vstring(['L2Relative', 'L3Absolute'])),
	doType1MET=False,doL1Cleaning = True,doL1Counters=False,
                        genJetCollection=cms.InputTag("ak5GenJets"),
			                 doJetID      = True)
#setTagInfos(process,
#                coll='patJetsAK5TJ',
#                tagInfos = cms.vstring('impactParameterTagInfos','secondaryVertexTagInfos')
#                )


# uncomment the following lines to add antikt5JPT jets to your PAT output
addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
                 'AK5','JPT',
                 doJTA        = True,
                 doBTagging   = True,
                 jetCorrLabel = ('AK5JPT',cms.vstring(['L1Offset','L1JPTOffset','L2Relative', 'L3Absolute','L2L3Residual'])),
#                 jetCorrLabel = None,
                 doType1MET   = False,
                 doL1Cleaning = True,
                 genJetCollection = cms.InputTag("ak5GenJets"),
                 doJetID      = True,
		 jetIdLabel       = "ak5"
                 )

# uncomment the following lines to add antikt5Pflow jets to your PAT output
#addJetCollection(process,cms.InputTag('pfJets::PAT'),
addJetCollection(process,cms.InputTag('ak5PFJets'),
                 'AK5','PF',
                 doJTA        = True,
                 doBTagging   = True,
                 jetCorrLabel = ('AK5PF',cms.vstring(['L1Offset','L2Relative', 'L3Absolute','L2L3Residual'])),
                 doType1MET   = False,
                 doL1Cleaning = False,
                 genJetCollection = cms.InputTag("ak5GenJets"),
##                 genJetCollection=cms.InputTag("ak5GenJetsNoNu"),
                 doJetID      = True,
		 jetIdLabel       = "ak5"
                 )

#from PhysicsTools.PatAlgos.tools.coreTools import *
#removeMCMatching(process, ['All'])
#removeSpecificPATObjects(process,['Taus'])#,'Photons'])

process.patJetCorrFactorsPF.levels =cms.vstring(['L1Offset','L2Relative', 'L3Absolute','L2L3Residual'])
process.selectedPatJetsPF.cut=cms.string("pt>10")
process.selectedPatJetsAK5PF.cut=cms.string("pt>10")
process.selectedPatJets.cut=cms.string("pt>10")
process.selectedPatJetsAK5JPT.cut=cms.string("pt>10")
process.selectedPatJetsAK5TJ.cut=cms.string("pt>10")

####################################################################
#process.demo = cms.EDAnalyzer('TopTreeProducer',
# 
#  histosFileName = cms.untracked.string("Tree.root"),
# 
##  trigTag = cms.untracked.InputTag('TriggerResults::HLT'),
#  trigTag = cms.untracked.InputTag('TriggerResults::HLT'),
#  muonTag = cms.untracked.InputTag('patMuons::PAT'),
#  pfmuonTag = cms.untracked.InputTag('patMuonsPF::PAT'),
#  jetTag=cms.untracked.InputTag('selectedPatJets::PAT'),
#  jetJPTTag=cms.untracked.InputTag('selectedPatJetsAK5JPT::PAT'),
#  jetPFTag=cms.untracked.InputTag('selectedPatJetsAK5PF::PAT'),
#  jetPF2Tag=cms.untracked.InputTag('selectedPatJetsPF::PAT'),
#  jetTJTag=cms.untracked.InputTag('selectedPatJetsAK5TJ::PAT'),
# # jetSCTag=cms.untracked.InputTag('patJetsSC5::PAT'),
# # jetSCJPTTag=cms.untracked.InputTag('patJetsSC5JPT::PAT'),
# # jetSCPFTag=cms.untracked.InputTag('patJetsSC5PF::PAT'),
# # jetSCTJTag=cms.untracked.InputTag('patJetsSC5TJ::PAT'),
#  metTag=cms.untracked.InputTag('patMETs::PAT'),
#  metTypeIITag=cms.untracked.InputTag('patMETsTypeII::PAT'),
#  metPFTag=cms.untracked.InputTag('patMETsPF::PAT'),
#  metPFTypeITag=cms.untracked.InputTag('patMETsTypeIPF::PAT'),
#  metPFNoPUTag=cms.untracked.InputTag('patMETsPFNoPU::PAT'),
#  metPFNoPUTypeITag=cms.untracked.InputTag('patMETsTypeIPFNoPU'),
#  metChargedPFNoPUTag=cms.untracked.InputTag('patChargedPFMETNoPU'),
#  metTCTag=cms.untracked.InputTag('patMETsTC::PAT'),
#  PVTag=cms.untracked.InputTag('offlinePrimaryVertices::PAT'),
#  pfelectronTag = cms.untracked.InputTag('patElectronsPF::PAT'),
#  electronTag = cms.untracked.InputTag('patElectrons::PAT'),
#  pftauTag = cms.untracked.InputTag('patTausPF::PAT'),
#  hltSummary = cms.InputTag('hltTriggerSummaryAOD::HLT'),
#  HLT_FilterLabels= cms.vstring('hltSingleMu9L3Filtered9',
# 'hltL1NonIsoHLTNonIsoSingleElectronLWEt15PixelMatchFilter',
#'hltL1NonIsoHLTNonIsoSingleElectronEt17CaloEleIdPixelMatchFilter',
#'hltL1NonIsoHLTNonIsoSingleElectronEt12EleIdDphiFilter',
# 'hltL1NonIsoHLTNonIsoSingleElectronLWEt10PixelMatchFilter',
# 'hltL1NonIsoHLTNonIsoSingleElectronEt12TighterEleIdIsolTrackIsolFilter',
# 'hltSingleMu15L3Filtered15','hltL1NonIsoHLTNonIsoSingleElectronEt10PixelMatchFilter'),
#  trigProcess=cms.string('HLT')
#
#   )
#############################################################################################
from minitreesproducer_cfg import triggerpaths

process.minitrees = cms.EDProducer('MiniTreesProducer'
		,outputFile = cms.string("outputfile.root")
		,Collections = 
		cms.VPSet(
			cms.PSet(
				Type = cms.string('EventHeader')
				, Instances = cms.vstring("")
				),
			cms.PSet(
				Type = cms.string('Vertex')
				, Instances = cms.vstring("offlinePrimaryVertices")
				),
			cms.PSet(
				Type = cms.string('Muon')
				, Instances = cms.vstring("selectedPatMuons")
				),
			cms.PSet(
				Type = cms.string('Track')
				, Instances = cms.vstring("generalTracks")
				),
			cms.PSet(
				Type = cms.string('TriggerResults')
				, TriggerPaths = triggerpaths
				, Instances = cms.vstring("HLT")
				),
			cms.PSet(
				Type = cms.string('TriggerParticles')
				, SummaryLabel = cms.string('hltTriggerSummaryAOD::HLT')
				, Instances = cms.vstring("hltSingleMu3L3Filtered3","hltSingleMu9L3Filtered9","hltL1sDoubleLooseIsoTau15")
				),
			)     
)


#process.higgsToWW2LeptonsHLTFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","REDIGI")


process.load("RecoVertex.PrimaryVertexProducer.OfflinePrimaryVerticesDA_cfi")
process.offlinePrimaryVertices = process.offlinePrimaryVerticesDA.clone()
process.offlinePrimaryVertices.useBeamConstraint = cms.bool(True)
process.offlinePrimaryVertices.TkClusParameters.TkDAClusParameters.Tmin = cms.double(4.)
process.offlinePrimaryVertices.TkClusParameters.TkDAClusParameters.vertexSize = cms.double(0.01)
#process.vertexing = cms.Path(process.offlinePrimaryVertices)

process.primaryVertexFilter = cms.EDFilter("VertexSelector",
    src = cms.InputTag("offlinePrimaryVertices::PAT"),
    cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"), # tracksSize() > 3 for the older cut
    filter = cms.bool(True),   # otherwise it won't filter the events, just produce an empty vertex collection.
)


#process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",  
#        vertexCollection = cms.InputTag('offlinePrimaryVertices'),
#         minimumNDOF = cms.uint32(4) ,
#         maxAbsZ = cms.double(24),
#         maxd0 = cms.double(2)
#)

process.noscraping = cms.EDFilter("FilterOutScraping",
applyfilter = cms.untracked.bool(True),
debugOn = cms.untracked.bool(False),
numtrack = cms.untracked.uint32(10),
thresh = cms.untracked.double(0.25)
)

#process.higgsToWW2LeptonsFilter.ElectronCollectionLabel = cms.InputTag ( "gsfElectrons::PAT" )
process.patMuons.usePV = cms.bool(False)      #to set the dB to the beamspot
process.patElectrons.usePV = cms.bool(False)  #to set the dB to the beamspot

process.selectedPatTaus.src=cms.InputTag("patTausPF","","")

#from RecoEgamma.ElectronIdentification.electronIdLikelihoodExt_cfi import *
#import RecoEgamma.ElectronIdentification.electronIdLikelihoodExt_cfi
#process.egammaIDLikelihood = RecoEgamma.ElectronIdentification.electronIdLikelihoodExt_cfi.eidLikelihoodExt.clone()
#process.eIdSequence = cms.Sequence( egammaIDLikelihood )

#HBHENoiseFilter
process.load('CommonTools.RecoAlgos.HBHENoiseFilter_cfi')

# KFactor
#process.load('HiggsAnalysis.HiggsToWW2Leptons.HWWKFactorProducer_cfi')

#For Rho lepton Isolation
process.load('RecoJets.JetProducers.kt4PFJets_cfi')
process.kt6PFJets = process.kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
process.kt6PFJets.Rho_EtaMax = cms.double(2.5)

##### Extra MET collections########
from JetMETCorrections.Type1MET.MetType1Corrections_cff import metJESCorAK5PFJet
#pfMET TypeI
process.metJESCorAK5PFTypeI = metJESCorAK5PFJet.clone(
        inputUncorJetsLabel = "patJetsPF",
        metType = "pat",
        inputUncorMetLabel = "pfMet",
        useTypeII = False,
        jetPTthreshold = cms.double(6.0)
    )
process.patMETsTypeIPF = process.patMETsPF.clone(
        metSource = cms.InputTag("metJESCorAK5PFTypeI")
    )

#PF no PU
process.pfMETPFnoPU= process.pfMETPF.clone(
    src = cms.InputTag("pfNoPileUpPF"),
    alias = cms.string('PFMETnoPU')
    )
process.patMETsPFNoPU = process.patMETsPF.clone(
        metSource = cms.InputTag("pfMETPFnoPU")
    )

#pfMET NoPU TypeI
process.metJESCorAK5PFNoPUTypeI = process.metJESCorAK5PFJet.clone(
        inputUncorJetsLabel = "patJetsPF",
        metType = "pat",
        inputUncorMetLabel = "pfMETPFnoPU",
        useTypeII = False,
        jetPTthreshold = cms.double(6.0)
    )
process.patMETsTypeIPFNoPU = process.patMETsPF.clone(
        metSource = cms.InputTag("metJESCorAK5PFNoPUTypeI")
    )

#extra pfMETs
#process.makePatMETsPF.replace(
#        process.patMETsPF,
#        process.patMETsPF+
process.ExtraPFMetSequence=cms.Sequence(
	process.metJESCorAK5PFTypeI+process.patMETsTypeIPF+
        process.pfMETPFnoPU+process.patMETsPFNoPU+process.metJESCorAK5PFNoPUTypeI+process.patMETsTypeIPFNoPU
        )

#Calo TypeII
process.metJESCorAK5CaloJetTypeII= process.metJESCorAK5CaloJet.clone(
useTypeII = cms.bool(True)
)
process.patMETsTypeII= process.patMETs.clone(
        metSource = cms.InputTag("metJESCorAK5CaloJetTypeII")
    )
#process.makePatMETs.replace(
#        process.patMETs,
#        process.patMETs+
process.CaloTypeIISequence=cms.Sequence(
        process.metJESCorAK5CaloJetTypeII+
        process.patMETsTypeII
        )

##### pfChargedMet
process.load("PhysicsTools.PFCandProducer.ParticleSelectors.pfSortByType_cff")
#Take only the charged noPU objects
process.pfNoPileUpCharged = cms.EDFilter("PdgIdPFCandidateSelector",
src = cms.InputTag("pfNoPileUpPF"),
#Pick if you want  electronsand muons as well
pdgId = cms.vint32(211,-211,321,-321,999211,2212,-2212,11,-11,13,-13)
#pdgId =cms.vint32(211,-211,321,-321,999211,2212,-2212)
                                    )
process.pfChargedPFMETnoPU= process.pfMETPF.clone(
    src = cms.InputTag("pfNoPileUpCharged"),
    alias = cms.string('chargedPFMETnoPU')
    )
process.patChargedPFMETNoPU = process.patMETsPF.clone(
        metSource = cms.InputTag("pfChargedPFMETnoPU")
    )
process.pfChargedPFMETnoPUSequence=cms.Sequence(
	process.pfNoPileUpCharged+
	process.pfChargedPFMETnoPU+
	process.patChargedPFMETNoPU)


#to avoid crashing from DA vertices due to more than one vertex to a track
#process.pfPileUpPF.Vertices=cms.InputTag("offlinePrimaryVertices","","HLT")


process.p = cms.Path(
process.offlinePrimaryVertices*
process.primaryVertexFilter*process.noscraping*
#process.KFactorProducer *

process.HBHENoiseFilter*
process.higgsToWW2LeptonsFilter*

(process.trackWithVertexRefSelector+
process.trackRefsForJets+
process.ak5TrackJets)*
#process.eIdSequence*
process.kt6PFJets*
process.patPF2PATSequencePF*
process.patDefaultSequence* 
process.ExtraPFMetSequence*
process.CaloTypeIISequence*
process.pfChargedPFMETnoPUSequence*
process.minitrees
#process.demo

)

#
process.GlobalTag.globaltag = cms.string('GR_R_311_V2::All')


process.source.fileNames = cms.untracked.vstring('/store/data/Run2011A/MuEG/AOD/PromptReco-v1/000/161/008/6A776BFE-2656-E011-942F-001D09F2A465.root')
#process.source.fileNames = cms.untracked.vstring('file:/hadoop/phedexExtension/oviedo/cms/store/mc/Winter10/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/E7TeV_ProbDist_2011Flat_BX156_START39_V8-v1/0000/FEB70FBF-6326-E011-A7E5-485B39800BD7.root')
#process.source.fileNames = cms.untracked.vstring('file:/hadoop/phedexExtension/oviedo/cms/store/mc/Winter10/GluGluToHToWWTo2L2Nu_M-160_7TeV-powheg-pythia6/AODSIM/E7TeV_ProbDist_2011Flat_BX156_START39_V8-v1/0000/120BD948-C726-E011-BDFD-00151796D6EC.root')
#process.source.fileNames = cms.untracked.vstring('file:/pool/fanae20/MuHad412.root')
#/hadoop/phedex/oviedo/cms/store/mc/Spring11/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/PU_S1_START311_V1G1-v1/0000/68E4078D-114E-E011-BB13-E0CB4EA0A904.root')
#process.source.fileNames = cms.untracked.vstring("#inputfiles#")
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('143833:517-143833:522','143833:537-143833:540','143833:545-143833:546','143833:549-143833:552','143833:557-143833:560')

#process.source.noEventSort = cms.untracked.bool(True)
#process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
process.maxEvents.input = -1             ## (e.g. -1 to run on all events)
#   process.out.outputCommands = [ ... ] ## (e.g. taken from PhysicsTools/PatAlgos/python/patEventContent_cff.py)
#process.out.fileName =  cms.untracked.string("#outputfile#")         ## (e.g. 'PAT.root')
process.options.wantSummary = False      ## (to suppress the long output at the end of the job)    
process.MessageLogger.cerr.FwkReport.reportEvery = 10000


#process.source.eventsToSkip = cms.untracked.VEventRange ( '1:899088')
process.higgsToWW2LeptonsFilter.ElectronCollectionLabel = cms.InputTag ("kk" )
process.higgsToWW2LeptonsFilter.SingleLeptonPtMin = cms.double(10.0)
process.higgsToWW2LeptonsFilter.DiLeptonPtMin = cms.double(0.0)
process.higgsToWW2LeptonsFilter.nLeptons = cms.int32(1)

#process.JPTeidTight.reducedEndcapRecHitCollection=cms.InputTag("reducedEcalRecHitsEE","")
#process.JPTeidTight.reducedBarrelRecHitCollection=cms.InputTag("reducedEcalRecHitsEB","")
process.patMETsTC.addMuonCorrections=cms.bool(False)
#process.patMETs.addMuonCorrections=cms.bool(False)
getattr(process,"pfNoTau"+postfix).enable = False


#Anyadimos algunos contenidos que necesito para el MiniTreeProducer
#process.out.fileName = cms.untracked.string('patrecoTest_1MuSkim_Run2011A_MuEG_AOD_PromptReco-v2.root')
#process.out.outputCommands.append( 'keep *_offlineBeamSpot_*_*' )
#process.out.outputCommands.append( 'keep *_KFactorProducer_*_*' )
#process.out.outputCommands.append( 'keep *_hltTriggerSummary*_*_*' )
#process.out.outputCommands.append( 'keep edmTriggerResults_*_*_*' )
#process.out.outputCommands.append( 'keep *_offlinePrimaryVertices*_*_*' )
#process.out.outputCommands.append( 'keep *_generalTracks*_*_*' )
#process.out.outputCommands.append( 'keep recoTrackExtra_*_*_*' )


#from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
#from PhysicsTools.PatAlgos.patEventContent_cff import patExtraAodEventContent
#from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent

#storeoutputs = cms.untracked.vstring('drop *', *patEventContent)
#storeoutputs += cms.untracked.vstring( *patExtraAodEventContent )
#storeoutputs += cms.untracked.vstring( *patTriggerEventContent )

#---> Tambien funciona
#process.out = cms.OutputModule("PoolOutputModule",
#		fileName = cms.untracked.string('patrecoTest_1MuSkim_Run2011A_MuEG_AOD_PromptReco-v2.root'),
		# save only events passing the full path
		#SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
		# save PAT Layer 1 output; you need a '*' to
		# unpack the list of commands 'patEventContent'
		#outputCommands = cms.untracked.vstring('drop *',
		#	'keep *_*_*_RECO',
		#	'keep *_*_*_PAT',
		#	'keep *_*_*_HLT'
		#	)
#		)

#process.outpath = cms.EndPath(process.out)
#process.out = cms.OutputModule("PoolOutputModule",
#    fileName = cms.untracked.string('patrecoTest_1MuSkim_Run2011A_MuEG_AOD_PromptReco-v1.root')
#    ,outputCommands = cms.untracked.vstring('keep *',
#	    'keep *_*_*_PAT'
# #     "keep *_generalTracks_*_*",
# #    "keep *_globalMuons_*_*",
# #      "keep *_MuonTrackPoints_*_*",
# #     "keep *_TrackTrackPoints_*_*")
#    )
#)
#
#process.e = cms.EndPath(process.out) 
