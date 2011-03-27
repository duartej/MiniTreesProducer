
#include "MiniTrees/MiniTreesProducer/interface/MTTrack.h"
#include "MiniTrees/MiniTreesProducer/interface/MTEventDirector.h"

#include "TrackingTools/IPTools/interface/IPTools.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"


MTTrack::MTTrack(const std::string & CollectionType, const std::vector<std::string> & InstancesCollection)
:MTAtom(CollectionType, InstancesCollection)
{
	// Registry what variables to store
	registryvalues();
	// And initializes, registry branches, ...
	initialize();
}


MTTrack::~MTTrack(){ }

// ------------ method called to for each event  ------------
void MTTrack::produce(MTEventDirector * eventdirector)
{
	for(unsigned int i=0; i < _NInstances; ++i)
	{
		// Initialization
		for(std::map<std::string,std::vector<float>* >::iterator it = _floatMethods[i].begin(); it != _floatMethods[i].end(); ++it)
		{
			it->second = new std::vector<float>;
		}
		for(std::map<std::string,std::vector<int>* >::iterator it = _intMethods[i].begin(); it != _intMethods[i].end(); ++it)
		{
			it->second = new std::vector<int>;
		}
		// Get the track products
		const std::vector<reco::Track> * tracks = static_cast<std::vector<reco::Track> *>(eventdirector->requestProduct( _InstancesCollection[i] ));

		// I need the vertices
		this->_vertices = static_cast<std::vector<reco::Vertex> *>(eventdirector->requestProduct( "Vertex" ));
		
		// And the event setup
		this->_setup = &(eventdirector->getsetup());

		// Storing ...
	  	for(std::vector<reco::Track>::const_iterator it = tracks->begin(); it != tracks->end(); ++it)
		{
			storevalues( i, *it );
		}
   	}
}

void MTTrack::Clean()
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
		for(std::map<std::string,std::vector<int>* >::iterator it = _intMethods[i].begin(); it != _intMethods[i].end(); ++it)
		{
			if( it->second )
			{
				delete it->second;
			}
		}
    	}
}

void MTTrack::initbranches( TTree * thetree )
{
     	for(unsigned int i=0; i < _InstancesCollection.size(); ++i)
   	{
		const std::string instanceCol("T_Track" ); // -->TODO: Preguntar si quieren el nombre de la coleccion

		for(std::vector<std::string>::iterator it = _FVALUES.begin(); it != _FVALUES.end(); ++it)
		{
			_floatMethods[i][ *it ] = 0;
			thetree->Branch( (instanceCol+"_"+(*it)).c_str(),"std::vector<float>", &((_floatMethods[i])[ *it ]) );
		}

		for(std::vector<std::string>::iterator it = _IVALUES.begin(); it != _IVALUES.end(); ++it)
		{
			_intMethods[i][ *it ] = 0;
			thetree->Branch( (instanceCol+"_"+(*it)).c_str(),"std::vector<int>", &((_intMethods[i])[ *it ]) );
		}
	}
}

void MTTrack::registryvalues()
{
        _IVALUES.push_back("NLayers");
        _IVALUES.push_back("NValidHits");
        _IVALUES.push_back("Charge");
        
	_FVALUES.push_back("Chi2");

        _FVALUES.push_back("Px");
        _FVALUES.push_back("Py");
	_FVALUES.push_back("Pz");
        _FVALUES.push_back("Pt");
	
        _FVALUES.push_back("vx");
	_FVALUES.push_back("vy");
        _FVALUES.push_back("vz");

        _FVALUES.push_back("IPz");
        _FVALUES.push_back("IPzSignedSig");
        _FVALUES.push_back("IPSig");
        _FVALUES.push_back("IPAbs");
}

void MTTrack::storevalues( const int & Ninstance, const reco::Track & track )
{
	_intMethods[Ninstance]["NLayers"]->push_back(track.hitPattern().trackerLayersWithMeasurement());
	_intMethods[Ninstance]["NValidHits"]->push_back(track.numberOfValidHits());
	_intMethods[Ninstance]["Charge"]->push_back(track.charge());

	_floatMethods[Ninstance]["Chi2"]->push_back(track.chi2());
	_floatMethods[Ninstance]["Px"]->push_back(track.px());
	_floatMethods[Ninstance]["Py"]->push_back(track.py());
	_floatMethods[Ninstance]["Pz"]->push_back(track.pz());
	_floatMethods[Ninstance]["Pt"]->push_back(track.pt());

	_floatMethods[Ninstance]["vx"]->push_back(track.vx());
	_floatMethods[Ninstance]["vy"]->push_back(track.vy());
	_floatMethods[Ninstance]["vz"]->push_back(track.vz());

	// --- Compute the IP-- FIXME: QUIZAS NO ES LO MISMO
	if( this->_vertices->size() > 0 )
	{
		std::pair<bool,Measurement1D> pair_mess1D = this->computeIP(track);
		if( pair_mess1D.first ) 
		{
			_floatMethods[Ninstance]["IPAbs"]->push_back( pair_mess1D.second.value() );
			_floatMethods[Ninstance]["IPSig"]->push_back( pair_mess1D.second.significance() );
		}
		
		std::pair<bool,Measurement1D> pair_mess1D_z = this->computeIPz( track );
		if( pair_mess1D_z.first )
		{
			_floatMethods[Ninstance]["IPz"]->push_back( pair_mess1D_z.second.value() );
			_floatMethods[Ninstance]["IPzSignedSig"]->push_back( pair_mess1D_z.second.significance() );
		}
	}
}

std::pair<bool,Measurement1D> MTTrack::computeIP(const reco::Track & tr )
{
	//get the transient track (e.g. for the muon track)
	edm::ESHandle<TransientTrackBuilder> builder;
	this->_setup->get<TransientTrackRecord>().get("TransientTrackBuilder", builder);
	reco::TransientTrack transTrack = builder->build(tr);

	// Must be checked before the size of vertices > 0

	return IPTools::absoluteImpactParameter3D( transTrack, this->_vertices->at(0) );
}

std::pair<bool,Measurement1D> MTTrack::computeIPz(const reco::Track & tr )
{
	//get the transient track (e.g. for the muon track)
	edm::ESHandle<TransientTrackBuilder> builder;
	this->_setup->get<TransientTrackRecord>().get("TransientTrackBuilder", builder);
	reco::TransientTrack transTrack = builder->build(tr);

	// Must be checked before the size of vertices > 0

	return IPTools::absoluteTransverseImpactParameter( transTrack, this->_vertices->at(0) );
}
