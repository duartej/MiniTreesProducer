// -*- C++ -*-
//
// Package:    MiniTreesProducer
// Class:      MiniTreesProducer
// 
/**\class MiniTreesProducer MiniTreesProducer.cc MiniTrees/MiniTreesProducer/plugins/MiniTreesProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Jordi Duarte-Campderros
//         Created:  Sat Feb 26 20:23:58 CET 2011
// $Id$
//
//


// system include files
#include<memory>
#include<cstring>
#include<list>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "MiniTrees/MiniTreesProducer/interface/MTStorageSingleton.h"
#include "MiniTrees/MiniTreesProducer/interface/MTAtomBuilder.h"
#include "MiniTrees/MiniTreesProducer/interface/MTAtom.h"

//
// class declaration
//
class MiniTreesProducer : public edm::EDProducer 
{
     	public:
	  	explicit MiniTreesProducer(const edm::ParameterSet&);
	  	~MiniTreesProducer();
		
     	private:
	  	virtual void beginJob() ;
	  	virtual void produce(edm::Event&, const edm::EventSetup&);
	  	virtual void endJob() ;

	  	// ----------member data ---------------------------
		std::list<MTAtom *> mtatoms;
};

MiniTreesProducer::MiniTreesProducer(const edm::ParameterSet& iConfig)
{
	// Initialize the output ntuple file and the tree
	MTStorageSingleton *stdirector = MTStorageSingleton::instance( iConfig.getParameter<std::string>("outputFile") );

	// Getting the user collections (MTAtoms) to be processed
	std::vector<edm::ParameterSet> Collection = iConfig.getParameter<std::vector<edm::ParameterSet> >("Collections");

	for(unsigned int i=0; i < Collection.size(); ++i)
	{
                MTAtom *mtatom = MTAtomBuilder::Build( Collection.at(i) );

                if( !mtatom )
                {
			//FIXME: El error lo deberia lanzar el Builder
                        throw cms::Exception("UnimplementedFeature") <<  "ERROR in config file! The object '"
                                << Collection.at(i).getParameter<std::string>("Type") << "' is not implemented to be stored.\n" <<
                                "'I need some of this in your config file Parameter 'Type':\n " <<
                                "\tMuon\n\tMET\n\tElectron\n\tPhoton\n\tJet\n\tBTag\n\tPF\n\tTau-ID\n\t" <<
                                "aTrack\n\tVertex\n\tGenParticle\n\tTriggerResults" << std::endl;
                }

                //mtatoms.push_back( mtatoms ); QUE PROBLEMA HAY??
        }

}


MiniTreesProducer::~MiniTreesProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
MiniTreesProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
/* This is an event example
   //Read 'ExampleData' from the Event
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);

   //Use the ExampleData to create an ExampleData2 which 
   // is put into the Event
   std::auto_ptr<ExampleData2> pOut(new ExampleData2(*pIn));
   iEvent.put(pOut);
*/

/* this is an EventSetup example
   //Read SetupData from the SetupRecord in the EventSetup
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
*/
 
}

// ------------ method called once each job just before starting event loop  ------------
void 
MiniTreesProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MiniTreesProducer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(MiniTreesProducer);
