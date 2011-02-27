// -*- C++ -*-
//
// Package:    MiniTreesProducer
// Class:      MTAtom
//
// Original Author: Jordi Duarte Campderros
//         Created:  Wed Nov  24 12:53:08 CET 2010
//
// jordi.duarte.campderros@cern.ch
//
//


#include "MiniTrees/MiniTreesProducer/interface/MTAtom.h"


MTAtom::MTAtom(const std::string & CollectionType, const std::vector<std::string> & InstancesCollection)
:_InstancesCollection( InstancesCollection )
{
	// Initialize and registry what branches of elements contains the
	// MTAtom (see the concrete implementation of each atom)
	registryvalues();
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

