// -*- C++ -*-
//
// Package:    MiniTreesProducer
// Class:      MTEventDirector
// 
/**\class MTEventDirector MTEventDirector.hh MiniTrees/MiniTreesProducer/src/MTEventDirector.cc

 Description:     Class to encapsulate the behaviour ...
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
#ifndef MTEventDirector_HH
#define MTEventDirector_HH

#include<list>
#include<map>

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
//
#include "MiniTrees/MiniTreesProducer/interface/MTAtom.h"

class MTEventDirector
{
	public:
		MTEventDirector(const edm::ParameterSet & iConfig);
		~MTEventDirector();
		// Some getters
		inline std::list<MTAtom *> & getatoms() { return _mtatoms; };
		inline const edm::Event & getevent() { return *_Event; };
		inline const edm::EventSetup & getsetup() { return *_iSetup; };
		// Initialize the event, extract the edm-product needed, ...
		void preEvent(const edm::Event & iEvent,const edm::EventSetup & iSetup );
		void produceEvent();
		void postEvent();
		void * requestProduct(const std::string & instance);

	private:
		//------ member data ---------------------------------------
		// List of active Objects (o quizas una referencia a los objetos...
		// si han sido construidos en algun otro lugar, solo pueden haber una
		// unica copia....
		std::list<MTAtom *> _mtatoms;
		// Per event map, what edm-products has been called and are in memory
		std::map<std::string,const void *> _activehandles;

		// Event
		const edm::Event * _Event;  // A lo mejor no puedo usar referencia, usare pointer
		//Setup
		const edm::EventSetup * _iSetup;







};

#endif
