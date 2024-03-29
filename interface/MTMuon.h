// -*- C++ -*-
//
// Package:    MiniTressProducer
// Class:      MTMuon
// 
/**\class MTMuon MTMuon.h MiniTrees/MiniTreesProducer/src/MTMuon.cc 

 Description: Create a TTree with the concrete instances of the
           Muon Physic Object

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

#ifndef MTMuon_HH
#define MTMuon_HH

// system include files
//
#include<vector>
#include<string>
#include<map>

#include "MiniTrees/MiniTreesProducer/interface/MTAtom.h"

#include "TTree.h"

#include "DataFormats/PatCandidates/interface/Muon.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TrackReco/interface/Track.h"
//#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

//
// class declaration
//

class MTMuon : public MTAtom 
{
     	public:
	  	MTMuon(const std::string&, const std::vector<std::string>&);
	  	virtual ~MTMuon();
		
	  	virtual void produce(MTEventDirector * eventdirector);
	  	virtual void initbranches( TTree * thetree );
	  	virtual void Clean();
		
     	private:
	  	virtual void registryvalues();
		void updateregister();
	  	void storevalues( const int & Ninstance, const pat::Muon & muon);
	  	// ----------member data ---------------------------
		reco::BeamSpot *_beamSpot;
		std::vector<reco::Vertex> *_vertices;
		std::vector<reco::Track> *_tracks;

		// We want a copy of the EventSetup, but it is needed in every event
		mutable const edm::EventSetup *_setup;
	
		// Used to change the behaviour if PF or not
		std::vector<std::string> _pfRejectINT;
		std::vector<std::string> _pfRejectFLOAT;
		std::map<int,bool> _isPF;
};

#endif
