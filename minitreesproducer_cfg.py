import FWCore.ParameterSet.Config as cms

process = cms.Process("MiniTrees")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
	    'file:/gpfs/gaes/cms/store/data/Run2010B/Mu/AOD/Dec22ReReco_v1/0001/30AA97EA-120E-E011-BA5F-E0CB4E29C4C2.root'
    )
)

process.minitrees = cms.EDProducer('MiniTreesProducer',
		outputFile = cms.string( 'myoutputfile.root' )
		)


#process.out = cms.OutputModule("PoolOutputModule",
#    fileName = cms.untracked.string('myOutputFile.root')
#)

  
process.p = cms.Path(process.minitrees)

#process.e = cms.EndPath(process.out)
