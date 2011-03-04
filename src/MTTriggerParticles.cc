
#include "MiniTrees/MiniTreesProducer/interface/MTTriggerParticles.h"

//FIXME: Some errors... La clase no acaba de funcionar bien.

MTTriggerParticles::MTTriggerParticles(const std::string & CollectionType, const std::vector<std::string> & InstancesCollection)
:MTAtom(CollectionType, InstancesCollection)
{
	// Extract the summaryLabel and the trigger process
	const std::string sumPlusProc  = _InstancesCollection.at( _InstancesCollection.size()-1 );
	size_t pos = sumPlusProc.find("::");
	if( pos == std::string::npos )
	{
		throw cms::Exception("Configuration") << "ERROR in config file! Missed the 'summaryLabel' for the 'TriggerParticles' Type." <<
			"\n\tInclude in 'TriggerParticles':" << "\n\t\tcms.PSet(\n\t\t\tType = cms.string('TriggerParticles')," << 
			"\n\t\t\tsummaryLabel = cms.string( 'hltsummaryLabel::triggProcess' ),\n\t\t\tInstances = cms.vstring( 'filtername1', 'filtername2', ....) "
		       	<< "\n\t\t)" << std::endl;
	}
	_summaryName = sumPlusProc.substr(0,pos);
	_processName = sumPlusProc.substr(pos+2);
	
	// Just the filters names
	_InstancesCollection.pop_back();

	// Registring the values to be stored
	registryvalues();

	initialize();
}
	

MTTriggerParticles::~MTTriggerParticles(){ }

// ------------ method called to for each event  ------------
void MTTriggerParticles::produce(const edm::Event& iEvent, const edm::EventSetup& /*iSetup*/)
{
	edm::Handle<trigger::TriggerEvent> trgEvent;
	iEvent.getByLabel( _summaryName, trgEvent );

	if( !trgEvent.isValid() )
	{
		std::cout << "TriggerSummary " << _summaryName << " NOT FOUND" << std::endl;
		//do something or not?
	}

	for(unsigned int i = 0; i < _NInstances; ++i)
	{
		// Initialization
		for(std::map<std::string,std::vector<float>* >::iterator it = _floatMethods[i].begin(); it != _floatMethods[i].end(); ++it)
		{
			it->second = new std::vector<float>;
		}
		// Storing ...
		storevalues( i, *trgEvent );
	}
}

void MTTriggerParticles::Clean()
{
	for(unsigned int i=0; i < _NInstances; ++i)
	{
		for(std::map<std::string,std::vector<float>* >::iterator it = _floatMethods[i].begin(); it != _floatMethods[i].end(); ++it)
		{
			if( it->second )
			{
				delete it->second;
			}
		}
    	}
}

void MTTriggerParticles::registryvalues()
{
	// Values to be stored for this object
	// This method is highly related with
	// storevalues method. If you add something
	// here, you must be consistent and add the
	// real storage in storevalues
	_FVALUES.push_back( "Px" );
	_FVALUES.push_back( "Py" );
	_FVALUES.push_back( "Pz" );
	_FVALUES.push_back( "Energy" );
}

void MTTriggerParticles::storevalues( const int & Ninstance, const trigger::TriggerEvent & trgEvent )
{
	std::string filterName = _InstancesCollection[Ninstance];
	
	// Getting the index corresponding to the filter
	const edm::InputTag collectionTag( filterName, "", _processName );
	size_t index = trgEvent.filterIndex( collectionTag );

	if( index < trgEvent.sizeFilters() )
	{
		const trigger::TriggerObjectCollection & toc = trgEvent.getObjects();
		//unsigned int sizetoc = toc.size();
		// Getting the map to extract the trigger object corresponding to the filter
		// within the TriggerObject vector
		const trigger::Keys & keys = trgEvent.filterKeys( index );
		for(unsigned int i = 0 ;i < keys.size(); ++i)
		{
			_floatMethods[Ninstance][ "Energy" ]->push_back( toc.at( keys[i] ).energy() );
			_floatMethods[Ninstance][ "Px" ]->push_back( toc[ keys[i] ].px() );
			_floatMethods[Ninstance][ "Py" ]->push_back( toc[ keys[i] ].py() );
			_floatMethods[Ninstance][ "Pz" ]->push_back( toc[ keys[i] ].pz() );
		}
	}
}

void MTTriggerParticles::initbranches( TTree* thetree )
{
	// Then, set the branchs
	for(unsigned int i=0; i < _InstancesCollection.size(); ++i)
	{	
		const std::string instanceCol( "T_TriggerParticles_"+_InstancesCollection[i] );
		for(std::vector<std::string>::iterator it = _FVALUES.begin(); it != _FVALUES.end(); ++it)
		{
			_floatMethods[i][ *it ] = 0;
			thetree->Branch( (instanceCol+"_"+(*it)).c_str(),"std::vector<float>", &((_floatMethods[i])[ *it ]) );
		}
	}
}

