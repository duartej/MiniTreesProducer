
#include <iostream>

#include "MiniTrees/MiniTreesProducer/interface/MTEventHeader.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

MTEventHeader::MTEventHeader( const std::string & colType, const std::vector<std::string> & InstColl )
:MTAtom( colType, InstColl )
{
	registryvalues();
	initialize();
}

void MTEventHeader::initbranches( TTree* thetree )
{
	// Don't need _intMethods nor _floatMethods
	// instead use std::map<string,int/float>
	
	const std::string instanceCol("T_Event_");

	for(std::vector<std::string>::iterator it = _IVALUES.begin(); it != _IVALUES.end(); ++it)
	{
		_intSimpleMap[ *it ] = -1;
		thetree->Branch( (instanceCol+(*it)).c_str(), &(_intSimpleMap[*it]) , std::string(instanceCol+(*it).c_str()+"/I").c_str() );
	}
	for(std::vector<std::string>::iterator it = _FVALUES.begin(); it != _FVALUES.end(); ++it)
	{
		_floatSimpleMap[ *it ] = 0;  //Or -1??
		thetree->Branch( (instanceCol+(*it)).c_str(), &(_intSimpleMap[*it]), std::string(instanceCol+(*it).c_str()+"/F").c_str() );
	}
}

void MTEventHeader::registryvalues()
{
      	//set parameter defaults 
	_Debug=false;
	//
	_IVALUES.push_back("RunNumber");
	_IVALUES.push_back("EventNumber");
	_IVALUES.push_back("LuminosityBlock");
	_IVALUES.push_back("Bx");
	_IVALUES.push_back("Orbit");

	_FVALUES.push_back("KFactorHNLO");
	_FVALUES.push_back("PtHat");
}


MTEventHeader::~MTEventHeader() 
{

}

// Analyze the event
void MTEventHeader::produce(edm::Event const& iEvent, const edm::EventSetup & /*iSetup*/)
{
      	_intSimpleMap["RunNumber"]       = iEvent.id().run();
      	_intSimpleMap["EventNumber"]     = iEvent.id().event();
      	_intSimpleMap["LuminosityBlock"] = iEvent.luminosityBlock();
      	_intSimpleMap["Bx"]              = iEvent.bunchCrossing();
      	_intSimpleMap["Orbit"]           = iEvent.orbitNumber();
      
	//FIXME: Que es el KFactor?? Sólo si hay MC? 
	_floatSimpleMap["KFactorHNLO"] = 1;
	edm::Handle<double> weightHandle;
	try 
	{
		iEvent.getByLabel("KFactorProducer","",weightHandle);
		_floatSimpleMap["KFactorHNLO"] = _floatSimpleMap["KFactorHNLO"]*(*weightHandle);
	} 
	catch (...) {}

	if( not iEvent.isRealData() )   // FIXME: Comprobar si esta funcion esta disponible!!
	{
		edm::Handle<GenEventInfoProduct> genEvtInfo;
		iEvent.getByLabel("generator", genEvtInfo);
		if( genEvtInfo->hasBinningValues() )
		{
			_floatSimpleMap["PtHat"] =  (genEvtInfo->binningValues())[0]; //FIXME: Seguro que hay algun valor?? uso at??
		}
		else
		{
			_floatSimpleMap["PtHat"] =  0.0;
		}
	}
      	
      	if (_Debug) 
	{
    		std::cout << "EventHeader -- run   = "          << _intSimpleMap["RunNumber"]       << std::endl;
	    	std::cout << "EventHeader -- event = "          << _intSimpleMap["EventNumber"]     << std::endl;
	    	std::cout << "EventHeader -- lumisection = "    << _intSimpleMap["LuminosityBlock"] << std::endl; 
	    	std::cout << "EventHeader -- bunch crossing = " << _intSimpleMap["Bx"]        << std::endl; 
	    	std::cout << "EventHeader -- orbit number = "   << _intSimpleMap["Orbit"]     << std::endl; 
		//TODO: PRint para los que faltan
	}
}

void MTEventHeader::Clean()
{
	// Nothing to clean
}
