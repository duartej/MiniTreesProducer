
#include "MiniTrees/MiniTreesProducer/interface/MTVertex.h"

#include "CommonTools/Statistics/interface/ChiSquaredProbability.h"



MTVertex::MTVertex(const std::string & CollectionType, const std::vector<std::string> & InstancesCollection)
:MTAtom(CollectionType, InstancesCollection)
{
	// Registring the values to be stored
	// registryvalues(); ---> The mother class

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
	
	for(unsigned int i=0; i < _InstancesCollection.size(); ++i)
	{
		const std::string instanceCol( "T_Vertex_"+_InstancesCollection[i] );
		for(std::vector<std::string>::iterator it = _DVALUES.begin(); it != _DVALUES.end(); ++it)
		{
			_floatMethods[i][ *it ] = 0;
			_ttree->Branch( (instanceCol+"_"+(*it)).c_str(),"std::vector<float>", &((_floatMethods.back())[ *it ]) );
		}

		for(std::vector<std::string>::iterator it = _IVALUES.begin(); it != _IVALUES.end(); ++it)
		{
			_intMethods[i][ *it ] = 0;
			_ttree->Branch( (instanceCol+"_"+(*it)).c_str(),"std::vector<int>", &((_intMethods.back())[ *it ]) );
		}
   	}
}

MTVertex::~MTVertex(){ }

// ------------ method called to for each event  ------------
void MTVertex::produce(const edm::Event& iEvent, const edm::EventSetup& /*iSetup*/)
{
	for(unsigned int i=0; i < _NInstances; ++i)
	{
		edm::Handle<std::vector<reco::Vertex> > vertexs;
		iEvent.getByLabel(_InstancesCollection[i],vertexs);
		// Initialization
		for(std::map<std::string,std::vector<double>* >::iterator it = _floatMethods[i].begin(); it != _floatMethods[i].end(); ++it)
		{
			it->second = new std::vector<double>;
		}
		for(std::map<std::string,std::vector<int>* >::iterator it = _intMethods[i].begin(); it != _intMethods[i].end(); ++it)
		{
			it->second = new std::vector<int>;
		}

		// Storing ...
	  	for(unsigned int ivertex=0; ivertex < vertexs->size(); ++ivertex)
		{
			storevalues( i, (*vertexs)[ivertex] );
		}
   	}
}

void MTVertex::Clean()
{
	for(unsigned int i=0; i < _NInstances; ++i)
	{
		for(std::map<std::string,std::vector<double>* >::iterator it = _floatMethods[i].begin(); it != _floatMethods[i].end(); ++it)
		{
			if( it->second )
			{
				delete it->second;
			}
		}
		for(std::map<std::string,std::vector<int>* >::iterator it = _intMethods[i].begin(); it != _intMethods[i].end(); ++it)
		{
			if( it->second )
			{
				delete it->second;
			}
		}
    	}
}


void MTVertex::registryvalues()
{
	// Values to be stored for this object
	// This method is highly related with
	// storevalues method. If you add something
	// here, you must be consistent and add the
	// real storage in storevalues
	_DVALUES.push_back( "z" );
	_DVALUES.push_back( "y" );
	_DVALUES.push_back( "x" );
	_DVALUES.push_back( "Chi2Prob" );
	_DVALUES.push_back( "rho" );
	_DVALUES.push_back( "ndof" );

	_IVALUES.push_back( "isFake" );
}

void MTVertex::storevalues( const int & Ninstance, const reco::Vertex & vertex )
{
	_floatMethods[Ninstance][ "z" ]->push_back( vertex.z() );
	_floatMethods[Ninstance][ "y" ]->push_back( vertex.y() );
	_floatMethods[Ninstance][ "x" ]->push_back( vertex.x() );
	_floatMethods[Ninstance][ "Chi2Prob" ]->push_back( ChiSquaredProbability( vertex.chi2(), vertex.ndof() ) );
	_floatMethods[Ninstance][ "rho" ]->push_back( vertex.position().Rho() );
	_floatMethods[Ninstance][ "ndof" ]->push_back( vertex.ndof() );

	_intMethods[Ninstance][ "isFake" ]->push_back( (int)(vertex.isFake()) );
}
