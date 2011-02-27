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

#include <string>
#include <vector>

#include "FWCore/Framework/interface/Event.h"

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
	  	virtual void produce(const edm::Event&, const edm::EventSetup&)=0;
	  	virtual void endJob();
	  	static void fill(const MTStorageSingleton * stdirector );
	  	virtual void Clean() = 0;
		
		
	// ----------member data ---------------------------
	protected:
	  	std::vector<std::string> _InstancesCollection;
};


#endif
