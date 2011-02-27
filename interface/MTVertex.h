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
#include <map>

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
		
	  	virtual void produce(const edm::Event&, const edm::EventSetup&);
	  	virtual void Clean();

     	private:
		virtual void registryvalues();
		void storevalues( const int & Ninstance, const reco::Vertex & vertex ); 
		
	  	// ----------member data ---------------------------
		// Correspondence between the 
		std::vector<std::map<std::string,std::vector<float>* > >_floatMethods;
		std::vector<std::map<std::string,std::vector<int>* > >_intMethods;

		// Names of the values
		std::vector<std::string> _FVALUES;
		std::vector<std::string> _IVALUES;

		unsigned int _NInstances;
};

#endif
