// -*- C++ -*-
//
// Package:    MiniTreesProducer
// Class:      MTAtomBuilder
// 
/**\class MTAtomBuilder MTAtomBuilder.h MiniTrees/MiniTreesProducer/src/MTAtomBuilder.cc

 Description: Builder to create Physics objects instances 
              (MTAtom concrete classes).

 Implementation: The Builder is called by the MiniTreesProducer which
              acts as a client. The class calls the instances of MTAtoms
   	      selected by the user: Muon, MET,... acting as a director.
	      Return a generic MTAtom.     
*/
//
// Original Author: Jordi Duarte Campderros  
//         Created:  Wed Nov  24 12:50:11 CET 2010
// 
// jordi.duarte.campderros@cern.ch
//
//

#ifndef MTAtomBuilder_HH
#define MTAtomBuilder_HH

#include "FWCore/ParameterSet/interface/ParameterSet.h"

class MTAtom;

class MTAtomBuilder {

  public:
     static MTAtom * Build(const edm::ParameterSet& );
 
};


#endif
