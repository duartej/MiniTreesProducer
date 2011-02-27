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
	// MTAtom (see the concrete implementation of registry value at each atom)
	registryvalues(); // Pon un print para ver que realmente coge la de la hija
	
	_NInstances = _InstancesCollection.size();

	// the list contains as much maps as instances we have
	for(unsigned int i=0; i < _InstancesCollection.size(); ++i)
	{	
		// Initializations: Mapping the name of the instances to
		//                  a vector of floats and integers which
		//                  will be stored as ntuples
		std::map<std::string, std::vector<float>* > floatMeth;
		_floatMethods.push_back( floatMeth );
		
		std::map<std::string, std::vector<int>* > intMeth;
		_intMethods.push_back( intMeth );
	}
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

void MTAtom::registryvalues()
{
}
