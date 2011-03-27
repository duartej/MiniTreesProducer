// -*- C++ -*-
//
// Package:    MiniTressProducer
// Class:      MTTrack
// 
/**\class MTTrack MTTrack.h MiniTrees/MiniTreesProducer/src/MTTrack.cc 

 Description: Create a TTree with the concrete instances of the
           Track Physic Object

 Implementation:
     Implements the concrete constructor and analyze method for the 
     class PhTTree.
*/
//
// Original Author: Jordi Duarte 
//         Created:  Mon Dec  1 17:10:08 CET 2008
// jordi.duarte.campderros@cern.ch
//
//

#ifndef MTTrack_HH
#define MTTrack_HH

// system include files
//
#include <utility>

#include "MiniTrees/MiniTreesProducer/interface/MTAtom.h"

#include "TTree.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/GeometryCommonDetAlgo/interface/Measurement1D.h"
#include "FWCore/Framework/interface/EventSetup.h"
//
// class declaration
//

class MTTrack : public MTAtom 
{
     	public:
	  	MTTrack(const std::string&, const std::vector<std::string>&);
	  	virtual ~MTTrack();
		
	  	virtual void produce(MTEventDirector * eventdirector);
	  	virtual void initbranches( TTree * thetree );
	  	virtual void Clean();
		
     	private:
	  	virtual void registryvalues();
	  	void storevalues( const int & Ninstance, const reco::Track & track);

		std::pair<bool,Measurement1D> computeIP( const reco::Track & track );
		std::pair<bool,Measurement1D> computeIPz( const reco::Track & track );
	  	// ----------member data ---------------------------
		std::vector<reco::Vertex> *_vertices;

		mutable const edm::EventSetup *_setup;
};

#endif
