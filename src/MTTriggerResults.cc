
#include "MiniTrees/MiniTreesProducer/interface/MTTriggerResults.h"


MTTriggerResults::MTTriggerResults(const std::string & CollectionType, const std::vector<std::string> & InstancesCollection)
:MTAtom(CollectionType, InstancesCollection)
{
	// Registry what variables to store
	registryvalues();
	// Initialize and registry what branches of elements contains
	initialize();
}

MTTriggerResults::~MTTriggerResults(){ }

// ------------ method called to for each event  ------------
void MTTriggerResults::produce(const edm::Event& iEvent, const edm::EventSetup& /*iSetup*/)
{
	//FIXME: Check que solo haya una NInstance
	for(unsigned int i=0; i < _NInstances; ++i)
	{
		std::string branchName( "T_HLT" ); //+_InstancesCollection[i] );

		edm::TriggerResultsByName hlttrigbyname = iEvent.triggerResultsByName( _InstancesCollection[i] );
	      
		// Initialization
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
	for(unsigned int i=0; i < _InstancesCollection.size(); ++i)
	{	
		const std::string instanceCol( "T_" ); //+_InstancesCollection[i] );

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
	_IVALUES.push_back( "HLT_Mu3" );
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
	_IVALUES.push_back( "HLT_IsoMu3" );
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
	std::cout << "Trigger paths missing: ";
	for(std::set<std::string>::iterator it = _notFound.begin(); it != _notFound.end(); ++it)
	{		
		std::cout << "\n\t" << *it;
	}
	std::cout << "" << std::endl;
}

