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

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "MiniTrees/MiniTreesProducer/interface/MTStorageSingleton.h"

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
};

MiniTreesProducer::MiniTreesProducer(const edm::ParameterSet& iConfig)
{
	// Initialize the output ntuple file and the tree
	MTStorageSingleton *stdirector = MTStorageSingleton::instance( iConfig.getParameter<std::string>("outputFile") );

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
