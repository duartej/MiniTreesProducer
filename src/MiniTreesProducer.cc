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
#include "MiniTrees/MiniTreesProducer/interface/MTEventDirector.h"
//#include "MiniTrees/MiniTreesProducer/interface/MTAtomBuilder.h"
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

		MTEventDirector * _eventdirector;
};

MiniTreesProducer::MiniTreesProducer(const edm::ParameterSet& iConfig)
{
	// Initialize the director for the storagement (output ntuple file and the tree)
	MTStorageSingleton * stdirector = MTStorageSingleton::instance( iConfig.getParameter<std::string>("outputFile") );
	if( stdirector == 0 )
	{
		std::cout << "PROBLEMS" << std::endl; //FIXME: cmsException
	}

	// Initialize the event director (who get the control of the storage algorithms)
	_eventdirector = new MTEventDirector( iConfig );
	
	//----> Poner un mensaje de que hay que llamar a esta funcion para inicializar los trees
	// Quizas pasar al singleton el MTEventDirector?
	MTStorageSingleton::Register( _eventdirector->getatoms() );
}


MiniTreesProducer::~MiniTreesProducer()
{
	if( _eventdirector )
	{
		delete _eventdirector;
	}

	// Saving the file
	MTStorageSingleton::close();
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
MiniTreesProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	// Registring edm-products needed 
	_eventdirector->preEvent(iEvent, iSetup);
	// Get the values to store
	_eventdirector->produceEvent();
	
	// Filling the trees
	MTStorageSingleton::fill();
	
	// Cleaning
	_eventdirector->postEvent();
}

// ------------ method called once each job just before starting event loop  ------------
void 
MiniTreesProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MiniTreesProducer::endJob() 
{
	// Maybe more convenient to do a endJob function for the MTEventDirector
	std::list<MTAtom *> mtatoms = _eventdirector->getatoms();
	for(std::list<MTAtom *>::iterator it = mtatoms.begin(); it != mtatoms.end(); ++it)
	{
		(*it)->endJob();
	}
}

//define this as a plug-in
DEFINE_FWK_MODULE(MiniTreesProducer);
