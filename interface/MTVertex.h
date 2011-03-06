// -*- C++ -*-
//
// Package:    MiniTreesProducer
// Class:      MTVertex
// 
/**\class MTVertex MiniTrees/MiniTreesProducer/src/MTVertex.cc 

 Description: Create branches related with the concrete instances of the
              Vertex Physic Object

 Implementation: Implements the concrete constructor, produce and regitryvalues
                 methods.
*/
//
// Original Author: Jordi Duarte 
//         Created:  Thu Nov 02 15:40:15 CET 2010
// jordi.duarte.campderros@cern.ch
//
//

#ifndef MTVertex_HH
#define MTVertex_HH

// system include files
#include "MiniTrees/MiniTreesProducer/interface/MTAtom.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

//
// class declaration
//

class MTVertex : public MTAtom {
     	public:
	  	MTVertex(const std::string&, const std::vector<std::string>&);
	  	virtual ~MTVertex();
		
	  	virtual void produce(MTEventDirector * eventdirector);
		virtual void initbranches( TTree* thetree );
	  	virtual void Clean();

     	private:
		virtual void registryvalues();
		void storevalues( const int & Ninstance, const reco::Vertex & vertex ); 
		
	  	// ----------member data ---------------------------
};

#endif
