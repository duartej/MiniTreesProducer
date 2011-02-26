// -*- C++ -*-
//
// Package:    MViniTreesProducer
//
// Original Author:  
//         Created:  Wed Nov  24 12:53:08 CET 2010
// $Id$
//
//


#include "MiniTrees/MiniTreesProducer/interface/MTAtom.h"


MTAtom::MTAtom(const std::string & CollectionType, const std::vector<std::string> & InstancesCollection)
:_InstancesCollection( InstancesCollection )
{
}


MTAtom::~MTAtom()
{
}


//
// member functions
//
void MTAtom::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void MTAtom::endJob() 
{
}

void MTAtom::fill(const MTStorageSingleton * stdirector)
{
	// Puedo hacer esto o tener un contador de estado (un registro en el singleton de cuantos MTAtoms tengo
	// y actualizar el estado), de tal forma que cuando todos los MTAtoms se hayan utilizado hacer la 
	// llamada real TTree::Fill 
	stdirector->fill();
}

