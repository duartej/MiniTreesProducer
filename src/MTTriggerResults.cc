
#include "MiniTrees/MiniTreesProducer/interface/MTTriggerResults.h"

#include "MiniTrees/MiniTreesProducer/interface/MTEventDirector.h"


MTTriggerResults::MTTriggerResults(const std::string & CollectionType, const std::vector<std::string> & InstancesCollection)
:MTAtom(CollectionType, InstancesCollection)
{
	// Extract the trigger paths (all but the first one)
	while( _InstancesCollection.size() > 1 )
	{
		_triggerPaths.push_back( _InstancesCollection.back() );
		_InstancesCollection.pop_back();
	}
	/*for(unsigned int i= 1; i < _InstancesCollection.size(); i++)
	{
		_triggerPaths = _InstancesCollection.at(i);
	}
	for(unsigned int i= 1; i < _InstancesCollection.size(); i++)
	{
		_InstancesCollection.pop_back();
	}*/

	// Redo the work done by the base class constructor
	// Now _InstancesCollection only contains the name of the trigger process (HLT likely)
	
	// Registry what variables to store
	registryvalues();
	// Initialize and registry what branches of elements contains
	initialize();
}

MTTriggerResults::~MTTriggerResults(){ }

// ------------ method called to for each event  ------------
void MTTriggerResults::produce(MTEventDirector * eventdirector)
{

	for(unsigned int i = 0; i < _NInstances; ++i)
	{
		//std::string branchName( "T_HLT" ); //+_InstancesCollection[i] );
		//	//
		edm::TriggerResultsByName hlttrigbyname = eventdirector->getevent().triggerResultsByName( _InstancesCollection[i] );
		// Initialization (Note that we are hardcoding the number of trigger process we can evaluate to 0 
 		for(std::map<std::string,std::vector<int>* >::iterator it = _intMethods[i].begin(); it != _intMethods[i].end(); ++it)
		{
			it->second = new std::vector<int>;
		}
		// Storing ...
		storevalues( i, hlttrigbyname );
	}
}

void MTTriggerResults::Clean()
{
	for(unsigned int i=0; i < _NInstances; ++i)
	{
		for(std::map<std::string,std::vector<int>* >::iterator it = _intMethods[i].begin(); it != _intMethods[i].end(); ++it)
		{
			if( it->second )
			{
				delete it->second;
			}
		}
    	}
}

void MTTriggerResults::initbranches( TTree* thetree )
{
	// FIXME: Controlling only 1 instances because the branches are not ready
	// to include more ( to do it just include the string with the name of the
	// instance in the instanceCol string
	for(unsigned int i=0; i < _NInstances; ++i)
	{
		const std::string instanceCol( "T_" ); 
		
		for(std::vector<std::string>::iterator it = _IVALUES.begin(); it != _IVALUES.end(); ++it)
		{
			_intMethods[i][ *it ] = 0;
			thetree->Branch( (instanceCol+(*it)).c_str(),"std::vector<int>", &((_intMethods.back())[ *it ]) );
		}
	}
}

void MTTriggerResults::registryvalues()
{
	// Values to be stored for this object
	// This method is highly related with
	// storevalues method. If you add something
	// here, you must be consistent and add the
	// real storage in storevalues
	//
	// Extracted from the _InstancesCollection list 
	// (is not needed to copy the content of _InstancesCollection to _IVALUES, but
	// by coherence done: _InstancesCollection is the initializer container, _IVALUES
	// is the run and storage container)
	for(std::list<std::string>::iterator it = _triggerPaths.begin(); it != _triggerPaths.end(); ++it)
	{
		_IVALUES.push_back( *it );
	}
	/*_IVALUES.push_back( "HLT_Mu3" );
	_IVALUES.push_back( "HLT_Ele10_LW_L1R" );
	_IVALUES.push_back( "HLT_Ele10_SW_L1R" );
	_IVALUES.push_back( "HLT_Ele15_SW_L1R" );
	_IVALUES.push_back( "HLT_DoubleMu3" );
	_IVALUES.push_back( "HLT_DoubleEle5_SW_L1R" );
	_IVALUES.push_back( "HLT_DoubleEle15_SW_L1R_v1" );
	_IVALUES.push_back( "HLT_DoubleEle10_SW_L1R" );
	_IVALUES.push_back( "HLT_Mu9" );
	_IVALUES.push_back( "HLT_Ele10_LW_EleId_L1R" );
	_IVALUES.push_back( "HLT_Ele10_SW_EleId_L1R" );
	_IVALUES.push_back( "HLT_Ele12_SW_TightEleIdIsol_NoDEtaInEE_L1R" );
	_IVALUES.push_back( "HLT_Ele12_SW_TightEleIdIsol_L1R" );
	_IVALUES.push_back( "HLT_Ele12_SW_TightEleId_L1R" );
	_IVALUES.push_back( "HLT_Ele17_SW_CaloEleId_L1R" );
	_IVALUES.push_back( "HLT_Ele17_SW_EleId_L1R" );
	_IVALUES.push_back( "HLT_Ele17_SW_TightCaloEleId_SC8HE_L1R_v1" );
	_IVALUES.push_back( "HLT_Ele17_SW_LooseEleId_L1R" );
	_IVALUES.push_back( "HLT_Ele17_SW_TightEleId_L1R" );
	_IVALUES.push_back( "HLT_Ele17_SW_TightEleIdIsol_L1R" );
	_IVALUES.push_back( "HLT_Mu11" );
	_IVALUES.push_back( "HLT_Mu15_v1" );
	_IVALUES.push_back( "HLT_Mu15" );
	_IVALUES.push_back( "HLT_IsoMu9" );
	_IVALUES.push_back( "HLT_DoubleEle17_SW_L1R_v1" );
	_IVALUES.push_back( "HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v1" );
	_IVALUES.push_back( "HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v2" );
	_IVALUES.push_back( "HLT_Ele22_SW_TighterEleId_L1R_v3" );
	_IVALUES.push_back( "HLT_Ele22_SW_TighterEleId_L1R_v2" );
	_IVALUES.push_back( "HLT_Ele17_SW_TighterEleIdIsol_L1R_v3" );
	_IVALUES.push_back( "HLT_Ele17_SW_TighterEleIdIsol_L1R_v2" );
	_IVALUES.push_back( "HLT_Photon10_L1R" );
	_IVALUES.push_back( "HLT_Photon15_L1R" );
	_IVALUES.push_back( "HLT_Photon10_Cleaned_L1R" );
	_IVALUES.push_back( "HLT_Photon15_Cleaned_L1R" );
	_IVALUES.push_back( "HLT_Jet10U" );
	_IVALUES.push_back( "HLT_Jet15U" );
	_IVALUES.push_back( "HLT_Jet30U" );
	_IVALUES.push_back( "HLT_Jet50U" );
	_IVALUES.push_back( "HLT_Jet70U" );
	_IVALUES.push_back( "HLT_Jet100U" );
	_IVALUES.push_back( "HLT_QuadJet15U" );
	_IVALUES.push_back( "HLT_QuadJet20U" );
	_IVALUES.push_back( "HLT_QuadJet25U" );
	_IVALUES.push_back( "HLT_L1Jet6U" );
	_IVALUES.push_back( "HLT_Ele15_LW_L1R" );
	_IVALUES.push_back( "HLT_Ele15_SW_CaloEleId_L1R" );
	_IVALUES.push_back( "HLT_Ele20_SW_L1R" );
	_IVALUES.push_back( "HLT_IsoMu3" );*/
}

void MTTriggerResults::storevalues( const int & Ninstance, const edm::TriggerResultsByName & hltResName )
{
	const unsigned int sizeNames = hltResName.size();

	for(std::vector<std::string>::iterator it = _IVALUES.begin(); it != _IVALUES.end(); ++it)
	{
		const unsigned int index = hltResName.triggerIndex( *it );
		
		if( index == sizeNames )
		{
			_notFound.insert( *it );
			continue;
		}

		if( hltResName.accept( index ) )
		{
			_intMethods[Ninstance][ *it ]->push_back( 1 );
		}
		else
		{
			_intMethods[Ninstance][ *it ]->push_back( 0 );
		}
	}
}

void MTTriggerResults::endJob()
{
	// TODO: podria hacer un autoremove de las ramas que no estaban
	std::cout << "Trigger paths missing: " << _notFound.size() << " (from a " << _IVALUES.size() << " total)";
	for(std::set<std::string>::iterator it = _notFound.begin(); it != _notFound.end(); ++it)
	{		
		std::cout << "\n\t" << *it;
	}
	std::cout << "" << std::endl;
}

