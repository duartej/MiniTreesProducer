
#include<algorithm>

#include "MiniTrees/MiniTreesProducer/interface/MTMuon.h"
#include "MiniTrees/MiniTreesProducer/interface/MTEventDirector.h"



MTMuon::MTMuon(const std::string & CollectionType, const std::vector<std::string> & InstancesCollection)
:MTAtom(CollectionType, InstancesCollection)
{
	// Registry what variables to store
	registryvalues();
	// And initializes, registry branches, ...
	initialize();
}


MTMuon::~MTMuon(){ }

// ------------ method called to for each event  ------------
void MTMuon::produce(MTEventDirector * eventdirector)
{
	for(unsigned int i=0; i < _NInstances; ++i)
	{
		// Get the muon products
		const edm::View<pat::Muon> * muons = static_cast<edm::View<pat::Muon> *>(eventdirector->requestProduct( _InstancesCollection[i] ));
		// Ordering: In pt-decreasing order -------
		std::map<float,pat::Muon> ptordermap;
		std::vector<pat::Muon> orderedmuons;
		for(unsigned int pos = 0 ; pos < muons->size(); ++pos)
		{
			ptordermap.insert(std::pair<float,pat::Muon>( muons->at(pos).pt(), muons->at(pos) ));
		}
		// map keys are sorted automatically from lower to higher.
		for(std::map<float,pat::Muon>::reverse_iterator it = ptordermap.rbegin(); it != ptordermap.rend(); ++it)
		{
			orderedmuons.push_back( it->second );
		}
		// orderedmuons ready ---------------------

		// Initialization
		for(std::map<std::string,std::vector<float>* >::iterator it = _floatMethods[i].begin(); it != _floatMethods[i].end(); ++it)
		{
			it->second = new std::vector<float>;
		}
		for(std::map<std::string,std::vector<int>* >::iterator it = _intMethods[i].begin(); it != _intMethods[i].end(); ++it)
		{
			it->second = new std::vector<int>;
		}

		// I need the beam spot
		this->_beamSpot = *(static_cast<reco::BeamSpot *>(eventdirector->requestProduct( "BeamSpot" )));

		// And the vertex...

		// Storing ...
	  	for(unsigned int imuon=0; imuon < orderedmuons.size(); ++imuon)
		{
			storevalues( i, orderedmuons[imuon] );
		}
   	}
}

void MTMuon::Clean()
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

void MTMuon::initbranches( TTree * thetree )
{
     	for(unsigned int i=0; i < _InstancesCollection.size(); ++i)
   	{
		const std::string instanceCol("T_Muon_"+_InstancesCollection[i] ); // -->TODO: Preguntar si quieren el nombre de la coleccion

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

void MTMuon::registryvalues()
{
      	_IVALUES.push_back("IsGlobalMuon");
      	_IVALUES.push_back("IsTMLSOLPT");
      	_IVALUES.push_back("IsTMLSOLPL");
        _IVALUES.push_back("IsGMPTMuons");
        _IVALUES.push_back("IsAllStandAloneMuons");     // checks isStandAloneMuon flag
        _IVALUES.push_back("IsAllTrackerMuons");        // checks isTrackerMuon flag
        _IVALUES.push_back("IsTrackerMuonArbitrated");  // resolve ambiguity of sharing segments
        _IVALUES.push_back("IsAllArbitrated");          // all muons with the tracker muon arbitrated
        _IVALUES.push_back("IsTMLastStationLoose");     // penetration depth loose selector
        _IVALUES.push_back("IsTMLastStationTight");     // penetration depth Tight selector
        _IVALUES.push_back("IsTM2DCompatibilityLoose"); // likelihood based loose selector
        _IVALUES.push_back("IsTM2DCompatibilityTight"); // likelihood based tight selector
        _IVALUES.push_back("IsTMOneStationLoose");      // require one well matched segment
        _IVALUES.push_back("IsTMOneStationTight");      // require one well matched segment
        _IVALUES.push_back("IsTMLSOPL");                // combination of TMLastStation and TMOneStation
        _IVALUES.push_back("IsGMTkChiCompatibility");   // require tk stub have good chi2 relative to glb track
        _IVALUES.push_back("IsGMStaChiCompatibility");  // require sta stub have good chi2 compatibility relative to glb track
        _IVALUES.push_back("IsGMTkKinkTight");
        _IVALUES.push_back("IsTMLastStationAngLoose");
        _IVALUES.push_back("IsTMLastStationAngTight");
        _IVALUES.push_back("IsTMOneStationAngLoose");
        _IVALUES.push_back("IsTMOneStationAngTight");

        _IVALUES.push_back("Charge");
        _IVALUES.push_back("NValidHitsInTrk");
        _IVALUES.push_back("NValidPixelHitsInTrk");
        _IVALUES.push_back("NValidHitsSATrk");
        _IVALUES.push_back("NValidHitsGTrk");
        _IVALUES.push_back("NumOfMatches");

        _FVALUES.push_back("SegmentCompatibility");
        _FVALUES.push_back("Px");
        _FVALUES.push_back("Py");
        _FVALUES.push_back("Pz");
        _FVALUES.push_back("Pt");
        _FVALUES.push_back("deltaPt");
        _FVALUES.push_back("Energy");
        
	_FVALUES.push_back("SegmentCompatibility");

	_FVALUES.push_back("NormChi2GTrk");

        _FVALUES.push_back("Chi2InTrk");
        _FVALUES.push_back("dofInTrk");
        _FVALUES.push_back("SumIsoCalo");
        _FVALUES.push_back("SumIsoTrack");
        _FVALUES.push_back("IPAbsGTrack");
        _FVALUES.push_back("IPSigGTrack");
        _FVALUES.push_back("IPAbsInTrack");
        _FVALUES.push_back("IPSigInTrack");
        _FVALUES.push_back("IPwrtBSInTrack");
        _FVALUES.push_back("IPwrtAveBSInTrack");
        _FVALUES.push_back("IPwrtBSGTrack");
        _FVALUES.push_back("vz");
        _FVALUES.push_back("vy");
        _FVALUES.push_back("vx");
        _FVALUES.push_back("OutPosx");
        _FVALUES.push_back("OutPosy");
        _FVALUES.push_back("OutPosz");
        _FVALUES.push_back("OutMomx");
        _FVALUES.push_back("OutMomy");
        _FVALUES.push_back("OutMomz");
        _FVALUES.push_back("OutPhi");
        _FVALUES.push_back("OutEta");
        _FVALUES.push_back("OutTheta");
        _FVALUES.push_back("OutRadius");
        _FVALUES.push_back("EcalVeto");
        _FVALUES.push_back("HcalVeto");

}

void MTMuon::storevalues( const int & Ninstance, const pat::Muon & muon )
{
	_intMethods[Ninstance]["isGlobalMuon"]->push_back(muon.isGlobalMuon());
      	_intMethods[Ninstance]["IsTMLSOLPT"]->push_back(muon.muonID("TMLastStationOptimizedLowPtTight"));
      	_intMethods[Ninstance]["IsTMLSOLPL"]->push_back(muon.muonID("TMLastStationOptimizedLowPtLoose"));
        _intMethods[Ninstance]["IsGMPTMuons"]->push_back(muon.muonID("GlobalMuonPromptTight"));
        _intMethods[Ninstance]["IsAllStandAloneMuons"]->push_back(muon.muonID("AllStandAloneMuons"));     
        _intMethods[Ninstance]["IsAllTrackerMuons"]->push_back(muon.muonID("AllTrackerMuons"));        
        _intMethods[Ninstance]["IsTrackerMuonArbitrated"]->push_back(muon.muonID("TrackerMuonArbitrated"));  
        _intMethods[Ninstance]["IsAllArbitrated"]->push_back(muon.muonID("AllArbitrated"));          
        _intMethods[Ninstance]["IsTMLastStationLoose"]->push_back(muon.muonID("TMLastStationLoose"));     
        _intMethods[Ninstance]["IsTMLastStationTight"]->push_back(muon.muonID("TMLastStationTight"));     
        _intMethods[Ninstance]["IsTM2DCompatibilityLoose"]->push_back(muon.muonID("TM2DCompatibilityLoose")); 
        _intMethods[Ninstance]["IsTM2DCompatibilityTight"]->push_back(muon.muonID("TM2DCompatibilityTight")); 
        _intMethods[Ninstance]["IsTMOneStationLoose"]->push_back(muon.muonID("TMOneStationLoose"));      
        _intMethods[Ninstance]["IsTMOneStationTight"]->push_back(muon.muonID("TMOneStationTight"));      
        _intMethods[Ninstance]["IsTMLSOPL"]->push_back(muon.muonID("TMLastStationOptimizedLowPtLoose"));                
        _intMethods[Ninstance]["IsGMTkChiCompatibility"]->push_back(muon.muonID("GMTkChiCompatibility"));   
        _intMethods[Ninstance]["IsGMStaChiCompatibility"]->push_back(muon.muonID("GMStaChiCompatibility"));  
        _intMethods[Ninstance]["IsGMTkKinkTight"]->push_back(muon.muonID("GMTkKinkTight"));
        _intMethods[Ninstance]["IsTMLastStationAngLoose"]->push_back(muon.muonID("TMLastStationAngLoose"));
        _intMethods[Ninstance]["IsTMLastStationAngTight"]->push_back(muon.muonID("TMLastStationAngTight"));
        _intMethods[Ninstance]["IsTMOneStationAngLoose"]->push_back(muon.muonID("TMOneStationAngLoose"));
        _intMethods[Ninstance]["IsTMOneStationAngTight"]->push_back(muon.muonID("TMOneStationAngTight"));

        _intMethods[Ninstance]["Charge"]->push_back(muon.charge());

        _intMethods[Ninstance]["NumOfMatches"]->push_back(muon.numberOfMatches());

        _floatMethods[Ninstance]["Px"]->push_back(muon.px());
        _floatMethods[Ninstance]["Py"]->push_back(muon.py());
        _floatMethods[Ninstance]["Pz"]->push_back(muon.pz());
        _floatMethods[Ninstance]["Pt"]->push_back(muon.pt());
        _floatMethods[Ninstance]["Energy"]->push_back(muon.energy());
        
	_floatMethods[Ninstance]["SegmentCompatibility"]->push_back(muon::segmentCompatibility(muon));
	
	// Usign the inner track reference
	try
	{
		_intMethods[Ninstance]["NValidHitsInTrk"]->push_back(muon.innerTrack()->hitPattern().numberOfValidTrackerHits());
		_intMethods[Ninstance]["NValidPixelHitsInTrk"]->push_back(muon.innerTrack()->hitPattern().numberOfValidPixelHits());
		_floatMethods[Ninstance]["Chi2InTrk"]->push_back(muon.innerTrack()->chi2());
		_floatMethods[Ninstance]["dofInTrk"]->push_back(muon.innerTrack()->ndof());
        	 _floatMethods[Ninstance]["deltaPt"]->push_back(muon.innerTrack()->ptError());
        	_floatMethods[Ninstance]["IPwrtBSInTrack"]->push_back(muon.innerTrack()->dxy(this->_beamSpot.position()));
	}
	catch(...)
	{
	}

	// If standalone track is not present get an null vector
	try
	{
		_intMethods[Ninstance]["NValidHitsSATrk"]->push_back(muon.standAloneMuon()->hitPattern().numberOfValidMuonHits());
	}
	catch( ... )
	{
	}

	// Global track reference
	try
	{
		_floatMethods[Ninstance]["OutPosx"]->push_back(muon.globalTrack()->outerPosition().x());
	 	_floatMethods[Ninstance]["OutPosy"]->push_back(muon.globalTrack()->outerPosition().y());
		_floatMethods[Ninstance]["OutPosz"]->push_back(muon.globalTrack()->outerPosition().z());
		_floatMethods[Ninstance]["OutMomx"]->push_back(muon.globalTrack()->outerMomentum().x());
		_floatMethods[Ninstance]["OutMomy"]->push_back(muon.globalTrack()->outerMomentum().y());
		_floatMethods[Ninstance]["OutMomz"]->push_back(muon.globalTrack()->outerMomentum().z());
		_floatMethods[Ninstance]["OutPhi"]->push_back(muon.globalTrack()->outerPhi());
		_floatMethods[Ninstance]["OutEta"]->push_back(muon.globalTrack()->outerEta());
		_floatMethods[Ninstance]["OutTheta"]->push_back(muon.globalTrack()->outerTheta());
		_floatMethods[Ninstance]["OutRadius"]->push_back(muon.globalTrack()->outerRadius());

		_floatMethods[Ninstance]["NormChi2GTrk"]->push_back(muon.globalTrack()->normalizedChi2());
		
		_floatMethods[Ninstance]["IPwrtBSGTrack"]->push_back(muon.globalTrack()->dxy(this->_beamSpot.position()));
	
		_intMethods[Ninstance]["NValidHitsGTrk"]->push_back(muon.globalTrack()->hitPattern().numberOfValidMuonHits());
	}
	catch( ... )
	{
	}
		

        _floatMethods[Ninstance]["SumIsoCalo"]->push_back(muon.caloIso());
        _floatMethods[Ninstance]["SumIsoTrack"]->push_back(muon.trackIso());
       /* _floatMethods[Ninstance]["IPAbsGTrack"]->push_back(muon.);
        _floatMethods[Ninstance]["IPSigGTrack"]->push_back(muon.);
        _floatMethods[Ninstance]["IPAbsInTrack"]->push_back(muon.);
        _floatMethods[Ninstance]["IPSigInTrack"]->push_back(muon.);
        _floatMethods[Ninstance]["IPwrtAveBSInTrack"]->push_back(muon.);*/
        _floatMethods[Ninstance]["vz"]->push_back(muon.vz());
        _floatMethods[Ninstance]["vy"]->push_back(muon.vy());
        _floatMethods[Ninstance]["vx"]->push_back(muon.vx());
        _floatMethods[Ninstance]["EcalVeto"]->push_back(muon.isolationR03().emVetoEt);
        _floatMethods[Ninstance]["HcalVeto"]->push_back(muon.isolationR03().hadVetoEt);
}
