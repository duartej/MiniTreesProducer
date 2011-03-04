// -*- C++ -*-
//
// Package:    MiniTreesProducer
// Class:      MTMTEventHeader
// 
/**\class MTMTEventHeader MTEventHeader.cc MiniTreesProducer/src/MTEventHeader.cc
 *
 Description: Class to deal with the header contents (event, Lumi, ...)

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  
//         Created:  Wed Nov  24 12:53:08 CET 2010
// $Id$
//
//
#ifndef MTEVTHDR_H
#define MTEVTHDR_H

#include "MiniTrees/MiniTreesProducer/interface/MTAtom.h"

class MTEventHeader : public MTAtom 
{
	public:
	      	MTEventHeader(const std::string & name, const std::vector<std::string> & vecNames ); 
		~MTEventHeader();
		
	      	virtual void produce(edm::Event const& iEvent, const edm::EventSetup & iSetup);
		virtual void initbranches( TTree* thetree );
		void Clean();

	private:
		virtual void registryvalues();

		// Not using _intMethods neither _floatMethods from
		// base class, instead use a simple maps in order to
		// store ints and floats
		std::map<std::string,int> _intSimpleMap;
		std::map<std::string,float> _floatSimpleMap;
	       	// Tree variables
	      	
		bool _Debug;
};

#endif
