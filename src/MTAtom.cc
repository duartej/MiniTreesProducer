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
:_InstancesCollection( InstancesCollection ), _objectname( CollectionType )
{
	// A daughter must have this code in her constructor
	// registryvalues();
	// initialize();
	
}

MTAtom::~MTAtom()
{
}

void MTAtom::initialize()
{
	_NInstances = _InstancesCollection.size();

	// the list contains as much maps as instances we have
	for(unsigned int i=0; i < _InstancesCollection.size(); ++i)
	{	
		// Initializations: Mapping the name of the instances to
		// a vector of floats and integers which will be stored
		//  as ntuples. Initializes with a empty vector
		std::map<std::string, std::vector<float>* > floatMeth;
		_floatMethods.push_back( floatMeth );
		
		std::map<std::string, std::vector<int>* > intMeth;
		_intMethods.push_back( intMeth );
	}
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

const std::string & MTAtom::getobjectname()
{
	return _objectname;
}

const std::vector<std::string> & MTAtom::getinstancesnames()
{
	return _InstancesCollection;
}
