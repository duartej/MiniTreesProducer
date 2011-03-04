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
#include "MiniTrees/MiniTreesProducer/interface/MTAtom.h"

#include "TTree.h"

#include "DataFormats/PatCandidates/interface/Muon.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
//#include "DataFormats/MuonReco/interface/Muon.h"
//#include "DataFormats/MuonReco/interface/MuonFwd.h"

//
// class declaration
//

class MTMuon : public MTAtom {
   public:
      MTMuon(const std::string&, const std::vector<std::string>&);
      virtual ~MTMuon();

      virtual void produce(const edm::Event&, const edm::EventSetup&);
      virtual void initbranches( TTree * thetree );
      virtual void Clean();

   private:
      virtual void registryvalues();
      void storevalues( const int & Ninstance, const pat::Muon & muon);
      // ----------member data ---------------------------a
      reco::BeamSpot _beamSpot;
};

#endif
