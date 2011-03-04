// -*- C++ -*-
//
// Package:    MiniTreesProducer
// Class:      MTAtom
// 
/**\class MTAtom MTAtom.hh MiniTrees/MiniTreesProducer/src/MTAtom.cc

 Description: Abstract class defining the processing minimal unit (pmu). 
              The pmu is a group of variables which are going to be 
	      processed and stored using the same algorithm (subclasses of
	      MTAtom), so when reprocessing is done the pmu associated to
	      the subclass which has changed or modified has been reprocessed also.
	      The pmu is associated to physic objects (Muon, electron, trigger, ...)

 Implementation:  This abstract class acts a common interface to create a
                  physic object. It is called by the Builder to construct
	 	  the TBranchs associated to this object.
     
*/
//
// Original Author: Jordi Duarte Campderros  
//         Created:  Wed Nov  24 12:48:11 CET 2010
// 
// jordi.duarte.campderros@cern.ch
//
//
#ifndef MTAtom_HH
#define MTAtom_HH

#include<map>
#include<string>
#include<vector>

#include "FWCore/Framework/interface/Event.h"

#include "TTree.h"

#include "MiniTrees/MiniTreesProducer/interface/MTStorageSingleton.h"

//
// class declaration
//
class MTAtom
{
     	public:
	  	MTAtom(const std::string&, const std::vector<std::string>&);
	  	virtual ~MTAtom();
	
	  	virtual void beginJob(); 
	  	virtual void endJob();
		// The processing method, where the storage algorithm is implemented
	  	virtual void produce(const edm::Event&, const edm::EventSetup&) = 0;
		// Initializes the branches in the file
		virtual void initbranches( TTree * thetree ) = 0;
	  	//static void fill(const MTStorageSingleton * stdirector );
		// Cleannig...
	  	virtual void Clean() = 0;
		
		
	protected:
		virtual void registryvalues() = 0 ;
		virtual void initialize();
		//virtual void storevalues( const int & Ninstance, const T & po ) = 0; --> Hacer la clase template ¿Es posible?

		// ----------member data ---------------------------
	  	std::vector<std::string> _InstancesCollection;
		unsigned int _NInstances;
		
		// Correspondence between the instance name of the Physic object and
		// a vector of <type> which will be the stored values
		std::vector<std::map<std::string,std::vector<float>* > >_floatMethods;
		std::vector<std::map<std::string,std::vector<int>* > >_intMethods;

		// Names of the values
		std::vector<std::string> _FVALUES;
		std::vector<std::string> _IVALUES;

	
};


#endif
