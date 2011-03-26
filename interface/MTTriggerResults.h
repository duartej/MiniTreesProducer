// -*- C++ -*-
//
// Package:    MiniTreesProducer
// Class:      MTTriggerResults
// 
/**\class MTTriggerResults MiniTrees/MiniTreesProducer/src/MTTriggerResults.cc 

 Description: Create branches related with the concrete instances of
              Trigger Physic Object

 Implementation: 
                 Implements the concrete constructor and analyze method for the 
	    	 class PhBranch.
*/
//
// Original Author: Jordi Duarte 
//         Created:  Fri Nov 03 12:21:15 CET 2010
// jordi.duarte.campderros@cern.ch
//
//

#ifndef MTTriggerResults_HH
#define MTTriggerResults_HH

// system include files
#include <set>
#include <list>

#include "TTree.h"

#include "MiniTrees/MiniTreesProducer/interface/MTAtom.h"

#include "FWCore/Common/interface/TriggerResultsByName.h"

//
// class declaration
//

class MTTriggerResults : public MTAtom
{
     	public:
	  	MTTriggerResults(const std::string&, const std::vector<std::string>&);
	  	virtual ~MTTriggerResults();
		
		virtual void endJob();
		//
	  	virtual void produce(MTEventDirector * eventdirector);
		virtual void initbranches( TTree* thetree );
	  	virtual void Clean();
     	private:
		void registryvalues();
		void storevalues( const int & Ninstance, const edm::TriggerResultsByName & hltresultsbyName );
		
	  	// ----------member data ---------------------------
		std::list<std::string> _triggerPaths;
		// Trigger paths not found
		std::set<std::string> _notFound;
};

#endif
