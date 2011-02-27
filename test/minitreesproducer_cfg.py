######################################################################
##						                    ##
## Example config file for the use of AnMiniTrees analyzer.         ##
##						                    ##
##  The Physics Objects (PO) are:                                   ##
##                          Muon, MET, Jet, Electron, Photon, Btag, ## 
##                          PF, TauID, Vertex, Track, GenParticle,  ##
##                          TriggerResults                          ##
##						                    ##
##  Beware! The instances1, ..., used in the Instance definition    ##
##          must be listed in the Table.in file ( the file is found ##
##          in interface dir but it must be placed in the working   ##
##          dir).                                                   ##
##          This software is CMSSW release dependent because the    ##
##          inclusion of the PO headers.                            ##
##						                    ##  
## Jordi Duarte Campderros (IFCA), Nov-2010                         ##
######################################################################

import FWCore.ParameterSet.Config as cms

process = cms.Process("MiniTree")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
	    'file:/gpfs/gaes/cms/store/data/Run2010B/Mu/AOD/Dec22ReReco_v1/0001/30AA97EA-120E-E011-BA5F-E0CB4E29C4C2.root'
    )
)

process.minitrees = cms.EDProducer('MiniTreesProducer'
		,outputFile = cms.string("outputfile.root")
		#,Collections = 
		#cms.VPSet(
		#	cms.PSet(
		#		Type = cms.string('Vertex')
		#		, Instances = cms.vstring("offlinePrimaryVertices")
		#		),
		#	cms.PSet(
		#		Type = cms.string('TriggerResults')
		#		, Instances = cms.vstring("REDIGI36X")
		#		),
		#	cms.PSet(
		#		Type = cms.string('TriggerParticles')
		#		, SummaryLabel = cms.string('hltTriggerSummaryAOD::REDIGI36X')
		#		, Instances = cms.vstring("hltSingleMu3L3Filtered3","hltSingleMu9L3Filtered9","hltL1sDoubleLooseIsoTau15")
		#		),
		#	)     
)

process.p = cms.Path(process.minitrees)
