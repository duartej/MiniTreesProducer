// -*- C++ -*-
//
// Package:    MiniTreesProducer
// Class:      MTTriggerParticles
// 
/**\class MTTriggerParticles MiniTrees/MiniTreesProducer/src/PhVertex.cc 

 Description: Create branches related with the concrete instances of
              Trigger Physic Object

 Implementation: 
                 Implements the concrete constructor and produce method for the 
	    	 class MTAtom.
*/
//
// Original Author: Jordi Duarte 
//         Created:  Fri Nov 03 12:21:15 CET 2010
// jordi.duarte.campderros@cern.ch
//
//

#ifndef MTTriggerParticles_HH
#define MTTriggerParticles_HH

// system include files
#include <set>

#include "MiniTrees/MiniTreesProducer/interface/MTAtom.h"

#include "DataFormats/HLTReco/interface/TriggerEvent.h"

//
// class declaration
//

class MTTriggerParticles : public MTAtom 
{
     	public:
	  	MTTriggerParticles(const std::string&, const std::vector<std::string>&);
	  	virtual ~MTTriggerParticles();
		
	  	virtual void produce(MTEventDirector * eventdirector);
		virtual void initbranches( TTree* thetree );
	  	virtual void Clean();
     	private:
		void registryvalues();
		void storevalues( const int & Ninstance, const trigger::TriggerEvent & trigEvent );
		
	  	// ----------member data ---------------------------
		std::string _summaryName;
		std::string _processName;
		
};

#endif
