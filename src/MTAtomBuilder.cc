// -*- C++ -*-
//
// Package:    MiniTreesProducer
// Class:      MTAtomBuilder
// 
//
// Original Author: Jordi Duarte Campderros  
//         Created:  Wed Nov  24 12:48:11 CET 2010
// 
// jordi.duarte.campderros@cern.ch
//
//

#include <string>
#include <vector>

#include "MiniTrees/MiniTreesProducer/interface/MTAtomBuilder.h"
#include "MiniTrees/MiniTreesProducer/interface/MTVertex.h"
/*#include "MiniTrees/MiniTreesProducer/interface/EventHeader.h"
#include "MiniTrees/MiniTreesProducer/interface/MTTriggerResults.h"
#include "MiniTrees/MiniTreesProducer/interface/MTTriggerParticles.h"*/
/*#include "../interface/MTMuon.hh"
#include "../interface/MTMET.hh"
#include "../interface/MTJet.hh"
#include "../interface/MTTrack.hh"
#include "../interface/MTGenParticle.hh"*/
/*
#include "../../MTAtom/interface/MTElectron.hh"
#include "../../MTAtom/interface/MTMToton.hh"
#include "../../MTAtom/interface/MTPF.hh"
#include "../../MTAtom/interface/MTBTag.hh"
#include "../../MTAtom/interface/MTTauId.hh"
// */

// Overload to construct the event content header part
/*MTAtom * MTAtomBuilder::Build()
{
	// dummy variables: //FIXME: cambia la declarcion de EventContent para que no necesite de estos argumentos
	std::string dummy1;
	std::vector<std::string> dummy2;

	MTAtom * p = 0;
	p = new EventHeader( dummy1, dummy2 );

	return p;
}// */

MTAtom *MTAtomBuilder::Build(const edm::ParameterSet & iConfig )
{
	MTAtom *p = 0;
     	
	// Getting the MTAtoms to use 
     	std::string CollectionType = iConfig.getParameter<std::string>("Type");
	// and their instances in EDM
     	std::vector<std::string> InstancesCollection = iConfig.getParameter<std::vector<std::string> >("Instances");

	// Building the concrete MTAtom
     	if( CollectionType == "Vertex" )
     	{
     		p = new MTVertex(CollectionType, InstancesCollection);
     	}
	/*else if( CollectionType == "TriggerResults" )
	{
		p = new MTTriggerResults(CollectionType, InstancesCollection);
	}
	else if( CollectionType == "TriggerParticles" )
	{
		// Need more information
		std::string triggerProcess = iConfig.getParameter<std::string>("SummaryLabel");
		InstancesCollection.push_back( triggerProcess );

		p = new MTTriggerParticles(CollectionType, InstancesCollection);
	}*/
	/*
     	if(CollectionType == "Muon" ) 
   	{
	 	p = new MTMuon(CollectionType, InstancesCollection);
   	}
  	
     	else if( CollectionType == "MET" )
   	{
	 	p = new MTMET(CollectionType, InstancesCollection);
   	}
	
     	else if( CollectionType == "Jet" )
   	{
	 	p = new MTJet(CollectionType, InstancesCollection);
  	}
     	else if( CollectionType == "Track" )
     	{
     		p = new MTTrack(CollectionType, InstancesCollection);
     	}
     	else if( CollectionType == "GenParticle" )
     	{
     		p = new MTGenParticle(CollectionType, InstancesCollection);
     	}
	// *
	   else if( CollectionType == "Electron" )
      	   {
    	   p= new MTElectron(CollectionType, InstancesCollection);
      	   }
	   else if( CollectionType == "MToton" )
     {
       p= new MTMToton(CollectionType, InstancesCollection);
     }
   else if( CollectionType == "Btag" )
     {
       p= new MTBtag(CollectionType, InstancesCollection);
     }
   else if( CollectionType == "PF" )
     {
       p= new MTPF(CollectionType, InstancesCollection);
     }
   else if( CollectionType == "TauID" )
     {
       p= new MTTauId(CollectionType, InstancesCollection);
     }
   //*/
     
   return p;
}
