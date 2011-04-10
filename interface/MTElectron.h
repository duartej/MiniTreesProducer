// -*- c++ -*-
//
// Package:    MiniTressProducer
// Class:      MTElectron
// 
/**\class MTElectron MTElectron.h MiniTrees/MiniTreesProducer/src/MTElectron.cc 

 Description: Create a TTree with the concrete instances of the
           Electron Physic Object

 Implementation:
     Implements the concrete constructor and produce method for the 
     class PhTTree.
*/
//
// Original Author: Jordi Duarte 
//         Created:  Mon Dec  1 17:10:08 CET 2008
// jordi.duarte.campderros@cern.ch
//
//

#ifndef MTElectron_HH
#define MTElectron_HH

// system include files
//
#include<vector>
#include<string>
#include<map>

#include "MiniTrees/MiniTreesProducer/interface/MTAtom.h"

#include "TTree.h"

#include "DataFormats/PatCandidates/interface/Electron.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TrackReco/interface/Track.h"
//#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

//
// class declaration
//

class MTElectron : public MTAtom 
{
     	public:
	  	MTElectron(const std::string&, const std::vector<std::string>&);
	  	virtual ~MTElectron();
		
	  	virtual void produce(MTEventDirector * eventdirector);
	  	virtual void initbranches( TTree * thetree );
	  	virtual void Clean();
		
     	private:
	  	virtual void registryvalues();
		void updateregister();
	  	void storevalues( const int & Ninstance, const pat::Electron & electron);
	  	// ----------member data ---------------------------
		reco::BeamSpot *_beamSpot;
		std::vector<reco::Vertex> *_vertices;
		std::vector<reco::Track> *_tracks;
		std::string _tracksname;

		// We want a copy of the EventSetup, but it is needed in every event
		mutable const edm::EventSetup *_setup;
		mutable const edm::Event *_event;

		float _bField;
	
		// Used to change the behaviour if PF or not (pure PAT)
		std::vector<std::string> _pfRejectINT;
		std::vector<std::string> _pfRejectFLOAT;
		std::vector<std::string> _patRejectINT;
		std::vector<std::string> _patRejectFLOAT;
		std::map<int,bool> _isPF;
};

#endif
