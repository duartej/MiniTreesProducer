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

//		MTStorageSingleton * _stdirector;
};

MiniTreesProducer::MiniTreesProducer(const edm::ParameterSet& iConfig)
{
	// Initialize the director for the storagement (output ntuple file and the tree)
	MTStorageSingleton * stdirector = MTStorageSingleton::instance( iConfig.getParameter<std::string>("outputFile") );
	if( stdirector == 0 )
	{
		std::cout << "PROBLEMS" << std::endl;
	}


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

                mtatoms.push_back( mtatom ); 
        }
	//----> Poner un mensaje de que hay que llamar a esta funcion para inicializar los trees
	// Initializing and registring the branches
	MTStorageSingleton::Register( mtatoms );
}


MiniTreesProducer::~MiniTreesProducer()
{
	for(std::list<MTAtom *>::iterator it = mtatoms.begin(); it != mtatoms.end(); ++it)
	{
		if( *it )
		{
			delete *it;
		}
	}

	/*if( _stdirector )
	{
		delete _stdirector;
	}*/
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
MiniTreesProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 	for(std::list<MTAtom *>::iterator it = mtatoms.begin(); it != mtatoms.end(); ++it)
	{
		(*it)->produce(iEvent,iSetup);
	}
	
	MTStorageSingleton::fill();
	
	for(std::list<MTAtom *>::iterator it = mtatoms.begin(); it != mtatoms.end(); ++it)
	{
		(*it)->Clean();
	}

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
