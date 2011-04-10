
#include<algorithm>
#include<cmath>
#include<set>

#include "MiniTrees/MiniTreesProducer/interface/MTElectron.h"
#include "MiniTrees/MiniTreesProducer/interface/MTEventDirector.h"

#include "DataFormats/GeometryCommonDetAlgo/interface/Measurement1D.h"
#include "TrackingTools/IPTools/interface/ImpactParameterComputer.h"
#include "RecoEgamma/EgammaTools/interface/ConversionFinder.h"
#include "Math/VectorUtil.h"


MTElectron::MTElectron(const std::string & CollectionType, const std::vector<std::string> & InstancesCollection)
:MTAtom(CollectionType, InstancesCollection)
{
	// Set the instances to be pure PAT or PAT PF objects
	for(unsigned int i = 0; i < _InstancesCollection.size(); ++i)
	{
		const std::string instancename = _InstancesCollection.at(i);
		size_t pos = instancename.find("PF");
		if( pos == std::string::npos )
		{
			_isPF[i] = false;
		}
		else
		{
			_isPF[i] = true;
		}
	}

	// Keys that PF won't use
	_pfRejectINT.push_back("nLost");

	_pfRejectFLOAT.push_back("Beta");
	_pfRejectFLOAT.push_back("DeltaBeta");
	_pfRejectFLOAT.push_back("IPwrtAveBS");
	_pfRejectFLOAT.push_back("IPwrtBS");
	_pfRejectFLOAT.push_back("simpleEleId70cIso");
	_pfRejectFLOAT.push_back("simpleEleId80relIso");
	_pfRejectFLOAT.push_back("simpleEleId90relIso");
	_pfRejectFLOAT.push_back("simpleEleId95relIso");
	_pfRejectFLOAT.push_back("simpleEleId95cIso");
	// Keys that PAT (not particle flow) won't use
	_patRejectFLOAT.push_back("chargedHadronIso");
	_patRejectFLOAT.push_back("neutralHadronIso");
	_patRejectFLOAT.push_back("particleIso");
	_patRejectFLOAT.push_back("photonIso");


	// Registry what variables to store
	registryvalues();
	// And initializes, registry branches, ...
	initialize();
	
	// update the values, different behaviour if PF or
	// not
	updateregister();
}


MTElectron::~MTElectron(){ }

void MTElectron::updateregister()
{
	for(unsigned int i = 0; i < _NInstances; ++i)
	{
		// Reco objects
		if( not _isPF[i] )
		{
			// Auxiliar vector with indexs to be erased
			// Now de-register 
			for(std::vector<std::string>::iterator it = _patRejectINT.begin(); it != _patRejectINT.end(); ++it)
			{
				_intMethods[i].erase( *it );
			}
			for(std::vector<std::string>::iterator it = _patRejectFLOAT.begin(); it != _patRejectFLOAT.end(); ++it)
			{
				_floatMethods[i].erase( *it );
			}
		}
		else
		{
			// Auxiliar vector with indexs to be erased
			// Now de-register 
			for(std::vector<std::string>::iterator it = _pfRejectINT.begin(); it != _pfRejectINT.end(); ++it)
			{
				_intMethods[i].erase( *it );
			}
			for(std::vector<std::string>::iterator it = _pfRejectFLOAT.begin(); it != _pfRejectFLOAT.end(); ++it)
			{
				_floatMethods[i].erase( *it );
			}
		}
	}
}

// ------------ method called to for each event  ------------
void MTElectron::produce(MTEventDirector * eventdirector)
{
	for(unsigned int i=0; i < _NInstances; ++i)
	{
		// Get the electron products
		const edm::View<pat::Electron> * electrons = static_cast<edm::View<pat::Electron> *>(eventdirector->requestProduct( _InstancesCollection[i] ));
		// Ordering: In pt-decreasing order -------
		std::map<float,pat::Electron> ptordermap;
		std::vector<pat::Electron> orderedelectrons;
		for(unsigned int pos = 0 ; pos < electrons->size(); ++pos)
		{
			ptordermap.insert(std::pair<float,pat::Electron>( electrons->at(pos).pt(), electrons->at(pos) ));
		}
		// map keys are sorted automatically from lower to higher.
		for(std::map<float,pat::Electron>::reverse_iterator it = ptordermap.rbegin(); it != ptordermap.rend(); ++it)
		{
			orderedelectrons.push_back( it->second );
		}
		// orderedelectrons ready ---------------------

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
		this->_beamSpot = static_cast<reco::BeamSpot *>(eventdirector->requestProduct( "BeamSpot" ));

		// the vertices...
		this->_vertices = static_cast<std::vector<reco::Vertex> *>(eventdirector->requestProduct( "Vertex" ));

		// and the tracks ...
		this->_tracks = static_cast<std::vector<reco::Track> *>(eventdirector->requestProduct( "Track" ));
		// also need the handle of the tracks, so getting the name of the module
		// FIXME: Check the return value is not empty
		this->_tracksname = eventdirector->labelFromGenericName( "Track" );
		// and the Event to extract the handle
		this->_event = &(eventdirector->getevent());

		// And even the Setup (for IP calulations)
		this->_setup = &(eventdirector->getsetup());

		// And the magnetic field
		this->_bField = eventdirector->getMagneticField();

		// Storing ...
	  	for(unsigned int ielectron=0; ielectron < orderedelectrons.size(); ++ielectron)
		{
			storevalues( i, orderedelectrons[ielectron] );
		}
   	}
}

void MTElectron::Clean()
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

void MTElectron::initbranches( TTree * thetree )
{
     	for(unsigned int i=0; i < _InstancesCollection.size(); ++i)
   	{
		// TO be used with PF
		std::set<std::string> rejectedF( _pfRejectFLOAT.begin(), _pfRejectFLOAT.end() );
		std::set<std::string> rejectedI( _pfRejectINT.begin(), _pfRejectINT.end() );
		// TO be used with RECO
		std::set<std::string> rejectedFPat( _patRejectFLOAT.begin(), _patRejectFLOAT.end() );
		std::set<std::string> rejectedIPat( _patRejectINT.begin(), _patRejectINT.end() );

		std::string instanceCol("T_Electron"); 
		if( _isPF[i] )
		{
			instanceCol = "T_pfElectron";
		}

		for(std::vector<std::string>::iterator it = _FVALUES.begin(); it != _FVALUES.end(); ++it)
		{
			if( _isPF[i] and ( rejectedF.find( *it ) != rejectedF.end()) )
			{
				// No storing
				continue;
			}
			if( (not _isPF[i]) and ( rejectedFPat.find( *it ) != rejectedFPat.end()) )
			{
				// No storing
				continue;
			}
//			std::cout << "FLOAT: " << instanceCol+"_"+(*it) << std::endl ;
			_floatMethods[i][ *it ] = 0;
			thetree->Branch( (instanceCol+"_"+(*it)).c_str(),"std::vector<float>", &((_floatMethods[i])[ *it ]) );
		}

		for(std::vector<std::string>::iterator it = _IVALUES.begin(); it != _IVALUES.end(); ++it)
		{

			if( _isPF[i] and ( rejectedI.find( *it ) != rejectedI.end()) )
			{
				// No storing
				continue;
			}
			if( (not _isPF[i]) and ( rejectedFPat.find( *it ) != rejectedFPat.end()) )
			{
				// No storing
				continue;
			}
//			std::cout << "INT: " << instanceCol+"_"+(*it) << std::endl ;
			_intMethods[i][ *it ] = 0;
			thetree->Branch( (instanceCol+"_"+(*it)).c_str(),"std::vector<int>", &((_intMethods[i])[ *it ]) );
		}
	}
}

void MTElectron::registryvalues()
{
	_IVALUES.push_back("nLost");
	_IVALUES.push_back("Charge");
	_IVALUES.push_back("nHits");
	_IVALUES.push_back("isEcalDriven");

        _FVALUES.push_back("Px");
        _FVALUES.push_back("Py");
        _FVALUES.push_back("Pz");
        _FVALUES.push_back("Pt");
        _FVALUES.push_back("Energy");
        
        _FVALUES.push_back("SC_Eta");
        _FVALUES.push_back("SC_Et");
        
	_FVALUES.push_back("dr03HcalSumEt");
        _FVALUES.push_back("dr03EcalSumEt");
        _FVALUES.push_back("dr03TkSumPt");

        _FVALUES.push_back("SumIsoCalo");
        _FVALUES.push_back("SumIsoTrack");

	_FVALUES.push_back("vx");
        _FVALUES.push_back("vy");
        _FVALUES.push_back("vz");
        
	_FVALUES.push_back("simpleEleId80cIso");
        _FVALUES.push_back("simpleEleId90cIso");
        _FVALUES.push_back("simpleEleId80relIso");
        _FVALUES.push_back("simpleEleId90relIso");
        _FVALUES.push_back("simpleEleId70cIso");
        _FVALUES.push_back("simpleEleId95cIso");
        _FVALUES.push_back("simpleEleId95relIso");

        _FVALUES.push_back("IPwrtBS");
        _FVALUES.push_back("IPwrtAveBS");
        _FVALUES.push_back("IPAbsGTrack");
        _FVALUES.push_back("IPSigGTrack");
        
	_FVALUES.push_back("Beta");
        _FVALUES.push_back("DeltaBeta");

        _FVALUES.push_back("ConvInfoDist");
        _FVALUES.push_back("ConvInfoDCot");
	
	//PF 
	_FVALUES.push_back("chargedHadronIso");
	_FVALUES.push_back("neutralHadronIso");
	_FVALUES.push_back("particleIso");
	_FVALUES.push_back("photonIso");
}

void MTElectron::storevalues( const int & Ninstance, const pat::Electron & electron )
{
	_intMethods[Ninstance]["Charge"]->push_back(electron.charge());
	_intMethods[Ninstance]["isEcalDriven"]->push_back(electron.ecalDrivenSeed());

        _floatMethods[Ninstance]["Px"]->push_back(electron.px());
        _floatMethods[Ninstance]["Py"]->push_back(electron.py());
        _floatMethods[Ninstance]["Pz"]->push_back(electron.pz());
        _floatMethods[Ninstance]["Pt"]->push_back(electron.pt());
        _floatMethods[Ninstance]["Energy"]->push_back(electron.energy());

	_floatMethods[Ninstance]["SumIsoCalo"]->push_back(electron.caloIso());
	_floatMethods[Ninstance]["SumIsoTrack"]->push_back(electron.trackIso());
	_floatMethods[Ninstance]["dr03TkSumPt"]->push_back(electron.dr03TkSumPt());
	_floatMethods[Ninstance]["dr03EcalSumEt"]->push_back(electron.dr03EcalRecHitSumEt());
	_floatMethods[Ninstance]["dr03HcalSumEt"]->push_back(electron.dr03HcalTowerSumEt());
        
	_floatMethods[Ninstance]["SC_Et"]->push_back(electron.superCluster()->energy()/
			cosh(electron.superCluster()->eta()));
			//TMath::CosH(electron.superCluster()->eta()));
	_floatMethods[Ninstance]["SC_Eta"]->push_back(electron.superCluster()->eta());
        
	_floatMethods[Ninstance]["vz"]->push_back(electron.vz());
        _floatMethods[Ninstance]["vy"]->push_back(electron.vy());
        _floatMethods[Ninstance]["vx"]->push_back(electron.vx());

	//FIXME: No estic segur d'aixo.. Haurien d'estar?
	try{
		_floatMethods[Ninstance]["simpleEleId90cIso"]->push_back(electron.electronID("simpleEleId90cIso"));
	}
	catch(...){;}

	try
	{
		_floatMethods[Ninstance]["simpleEleId80cIso"]->push_back(electron.electronID("simpleEleId80cIso"));
	}
	catch(...){;}
        

	// Track reference
	if( not electron.gsfTrack().isNull() )
	{
		if( not _isPF[Ninstance] )
		{
			_floatMethods[Ninstance]["IPwrtBS"]->push_back(electron.gsfTrack()->dxy(this->_beamSpot->position()));
			_intMethods[Ninstance]["nLost"]->push_back(electron.gsfTrack()->trackerExpectedHitsInner().numberOfLostHits());
		}
	
		_intMethods[Ninstance]["nHits"]->push_back(electron.gsfTrack()->trackerExpectedHitsInner().numberOfHits());
		
		// IP and Beta (DBeta) 
		if( this->_vertices->size() > 0)
		{
			// Using the vxt with the highest pt sum
			IPTools::ImpactParameterComputer IPComp( this->_vertices->at(0) );
			Measurement1D mess1D = IPComp.computeIP( *(this->_setup), *(electron.gsfTrack()) );

			_floatMethods[Ninstance]["IPSigGTrack"]->push_back( mess1D.significance() );
			_floatMethods[Ninstance]["IPAbsGTrack"]->push_back( mess1D.value() );
			
			if( not _isPF[Ninstance] )
			{
				// Beta and DeltaBeta
				float beta = 0.0;
				float Dbeta = 0.0;
				for(std::vector<reco::Track>::iterator track = this->_tracks->begin(); track != this->_tracks->end(); track++)
				{
					float dR = ROOT::Math::VectorUtil::DeltaR( track->momentum(),electron.gsfTrack()->momentum() );
					if( dR < 0.01 )
					{
						continue;
					}
					if( dR < 0.3 )
					{
						if( fabs( track->vz() - this->_vertices->at(0).z() ) <0.2 ) 
						{
							beta += track->pt();
						}
						else
						{
						       	Dbeta+=track->pt();
						}
					}
				}

				float storebeta = 0.0;
				if( fabs(Dbeta) < 1e-25 )  // WATCH!! float == 0? Better less than
				{
					storebeta = 1.0;
				}
				else
				{
					storebeta = beta/(beta+Dbeta);
				}
				_floatMethods[Ninstance]["Beta"]->push_back( storebeta );
				_floatMethods[Ninstance]["DeltaBeta"]->push_back( Dbeta );
			}
		}
	}
		


	// I need the Handle of the Tracks for the getConversionInfo function  
	//edm::Handle<std::vector<reco::Track> > trackHandle( this->_tracks ,0); // NEEDS A REAL PROVENANCE...
	edm::Handle<std::vector<reco::Track> > trackHandle;	
	this->_event->getByLabel( this->_tracksname, trackHandle ); 

	ConversionFinder convFinder;
	ConversionInfo convInfo = convFinder.getConversionInfo( electron,trackHandle, this->_bField );
	_floatMethods[Ninstance]["ConvInfoDist"]->push_back(convInfo.dist());
	_floatMethods[Ninstance]["ConvInfoDCot"]->push_back(convInfo.dcot());

	// Some values to be store depending the object
	if( not _isPF[Ninstance] )
	{
		_floatMethods[Ninstance]["IPwrtAveBS"]->push_back(electron.dB());
		// FIXME: NO estic segur si volem que no estiguin els ID aquestos
		try
		{
			_floatMethods[Ninstance]["simpleEleId70cIso"]->push_back(electron.electronID("simpleEleId70cIso"));
		}
		catch(...){;}

		try
		{
			_floatMethods[Ninstance]["simpleEleId80relIso"]->push_back(electron.electronID("simpleEleId80relIso"));
		}
		catch(...){;}

		try
		{
			_floatMethods[Ninstance]["simpleEleId90relIso"]->push_back(electron.electronID("simpleEleId90relIso"));
		}
		catch(...){;}

		try
		{
			_floatMethods[Ninstance]["simpleEleId95cIso"]->push_back(electron.electronID("simpleEleId95cIso"));
		}
		catch(...){;}

		try
		{		
			_floatMethods[Ninstance]["simpleEleId95relIso"]->push_back(electron.electronID("simpleEleId95relIso"));
		}
		catch(...){;}
	}
	else 
	{
		_floatMethods[Ninstance]["chargedHadronIso"]->push_back( electron.chargedHadronIso() );
		_floatMethods[Ninstance]["neutralHadronIso"]->push_back( electron.neutralHadronIso() );
		_floatMethods[Ninstance]["particleIso"]->push_back( electron.particleIso() );
		_floatMethods[Ninstance]["photonIso"]->push_back( electron.photonIso() );
	}
}
