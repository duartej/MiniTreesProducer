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

# ---- Trigger paths to be included ----- #
triggerpaths = cms.vstring( 
		"HLT_IsoMu17_v4",
                "HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1",
                "HLT_Ele10_SW_L1R_v2",
                "HLT_Ele45_CaloIdVT_TrkIdT_v1",
                "HLT_DoubleMu3_v3",
                "HLT_Mu8_v1",
                "HLT_Mu15_v2",
                "HLT_Mu9",
                "HLT_Mu15_v1",
                "HLT_DoubleEle15_SW_L1R_v1",
                "HLT_Ele8_v1",
                
                "HLT_Mu20_v1",
                "HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v1",
                "HLT_Ele32_CaloIdL_CaloIsoVL_SC17_v1",
                "HLT_DoubleEle10_CaloIdL_TrkIdVL_Ele10_v1",
               
                "HLT_TripleEle10_CaloIdL_TrkIdVL_v1",
                "HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v1",
                "HLT_Mu17_Ele8_CaloIdL_v1",
                "HLT_Mu8_Ele17_CaloIdL_v1", 
                "HLT_Ele8_CaloIdL_CaloIsoVL_v1",
                "HLT_Ele17_SW_TightCaloEleId_SC8HE_L1R_v1",
                "HLT_Ele17_CaloIdL_CaloIsoVL_v1	",
                "HLT_Photon20_CaloIdVT_IsoT_Ele8_CaloIdL_CaloIsoVL_v1	",
                "HLT_Ele8_CaloIdL_CaloIsoVL_Jet40_v1",
                "HLT_Mu30_v1",
                "HLT_Mu24_v1",
                "HLT_Mu21_v1",
                "HLT_Mu25_v1",
                "HLT_IsoMu15_v4", 
                
                "HLT_DoubleEle17_SW_L1R_v1",
                "HLT_Mu10_Ele10_CaloIdL_v3",
                "HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v2",
                "HLT_Ele22_SW_TighterEleId_L1R_v3",
                "HLT_Ele22_SW_TighterEleId_L1R_v2",
                "HLT_Ele17_SW_TighterEleIdIsol_L1R_v3",
                "HLT_Ele17_SW_TighterEleIdIsol_L1R_v2",
               
                "HLT_DoubleMu5_v1",
                "HLT_DoubleMu6_v1",
                "HLT_DoubleMu7_v1",
                "HLT_TripleMu5_v1",
                "HLT_Mu8_Photon20_CaloIdVT_IsoT_v1",
                "HLT_Jet30_v1",
                "HLT_Jet60_v1",
                "HLT_Jet80_v1",
                "HLT_Jet110_v1",
                "HLT_DiJetAve15U_v4",
                "HLT_DiJetAve30U_v4",
                "HLT_DiJetAve70U_v4",
                "HLT_Jet15U",
                "HLT_Jet30U",
                "HLT_Jet50U",
                "HLT_Jet70U",
                "HLT_Jet100U",
                "HLT_DiJetAve15U_v3",
                "HLT_DiJetAve30U_v3",
                "HLT_DiJetAve70U_v3",
                "HLT_QuadJet15U", 
                "HLT_QuadJet20U",
                "HLT_QuadJet25U",
                "HLT_Mu8_Jet40_v3",
                "HLT_Mu5_Jet50U_v3"
		)
#---------------------------------------------------------------#


process = cms.Process("MiniTree")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
	    #'file:/gpfs/gaes/cms/store/data/Run2010B/Mu/AOD/Dec22ReReco_v1/0001/30AA97EA-120E-E011-BA5F-E0CB4E29C4C2.root'
	    'file:/gpfs/csic_projects/cms/duarte/CMSSW_64b/CMSSW_4_1_3/src/MiniTrees/MiniTreesProducer/test/patrecoTest_1MuSkim_Run2011A_MuEG_AOD_PromptReco-v2.root'
    )
)

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

process.p = cms.Path(process.minitrees)


