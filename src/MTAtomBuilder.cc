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
#include "MiniTrees/MiniTreesProducer/interface/MTTriggerResults.h"
#include "MiniTrees/MiniTreesProducer/interface/MTEventHeader.h"
#include "MiniTrees/MiniTreesProducer/interface/MTTriggerParticles.h"
#include "MiniTrees/MiniTreesProducer/interface/MTMuon.h"
#include "MiniTrees/MiniTreesProducer/interface/MTElectron.h"
#include "MiniTrees/MiniTreesProducer/interface/MTTrack.h"
/*#include "../interface/MTMET.hh"
#include "../interface/MTJet.hh"
#include "../interface/MTGenParticle.hh"*/
/*
#include "../../MTAtom/interface/MTMToton.hh"
#include "../../MTAtom/interface/MTBTag.hh"
#include "../../MTAtom/interface/MTTauId.hh"
// */


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
	else if( CollectionType == "TriggerResults" )
	{
		// extract the name of the HLT trigger Paths to evaluate
		std::vector<std::string> triggerPaths = iConfig.getParameter<std::vector<std::string> >("TriggerPaths");
		// And put it the last one
		for(std::vector<std::string>::iterator it = triggerPaths.begin(); it != triggerPaths.end(); it++)
		{
			InstancesCollection.push_back( *it );
		}

		p = new MTTriggerResults(CollectionType, InstancesCollection);
	}
	else if( CollectionType == "TriggerParticles" )
	{
		// extract the summary label parameter
		std::string triggerProcess = iConfig.getParameter<std::string>("SummaryLabel");
		// And put it the last one
		InstancesCollection.push_back( triggerProcess );

		p = new MTTriggerParticles(CollectionType, InstancesCollection);
	}
	else if( CollectionType == "EventHeader" ) //FIXME Ponerlo directamente en el MiniTreeProducer pues siempre tiene que estar
	{
		p = new MTEventHeader(CollectionType, InstancesCollection);
	}
	else if(CollectionType == "Muon" ) 
   	{
	 	p = new MTMuon(CollectionType, InstancesCollection);
   	}
	else if(CollectionType == "Electron" ) 
   	{
	 	p = new MTElectron(CollectionType, InstancesCollection);
   	}
     	else if( CollectionType == "Track" )
     	{
     		p = new MTTrack(CollectionType, InstancesCollection);
     	}
/*     	else if( CollectionType == "MET" )
   	{
	 	p = new MTMET(CollectionType, InstancesCollection);
   	}
	
     	else if( CollectionType == "Jet" )
   	{
	 	p = new MTJet(CollectionType, InstancesCollection);
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
	   else if( CollectionType == "Photon" )
     {
       p= new MTPhoton(CollectionType, InstancesCollection);
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
