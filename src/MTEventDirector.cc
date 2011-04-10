// -*- C++ -*-
//
// Package:    MiniTreesProducer
// Class:      MTAtomBuilder
// 
//
// Original Author: Jordi Duarte Campderros  
//         Created:  Wed Nov  24 12:48:11 CET 2010
// 
// jordi.duarte.campderros@cern.ch
//
//

#include "MiniTrees/MiniTreesProducer/interface/MTEventDirector.h"
#include "MiniTrees/MiniTreesProducer/interface/MTAtomBuilder.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "FWCore/Framework/interface/ESHandle.h"


MTEventDirector::MTEventDirector(const edm::ParameterSet & iConfig)
	:_Event(0)

{
	// Getting the user collections (MTAtoms) to be processed
	// and what EDM instances to use
	std::vector<edm::ParameterSet> Collection = iConfig.getParameter<std::vector<edm::ParameterSet> >("Collections");

	for(unsigned int i=0; i < Collection.size(); ++i)
	{
                MTAtom *mtatom = MTAtomBuilder::Build( Collection.at(i) );

                if( !mtatom )
                {
			//FIXME: El error lo deberia lanzar el Builder
                        throw cms::Exception("UnimplementedFeature") <<  "ERROR in config file! The object '"
                                << Collection.at(i).getParameter<std::string>("Type") << "' is not implemented to be stored.\n" <<
                                "'I need some of this in your config file Parameter 'Type':\n " <<
                                "\tMuon\n\tMET\n\tElectron\n\tPhoton\n\tJet\n\tBTag\n\tPF\n\tTau-ID\n\t" <<
                                "aTrack\n\tVertex\n\tGenParticle\n\tTriggerResults" << std::endl;
                }
                this->_mtatoms.push_back(mtatom); 
        }
}

MTEventDirector::~MTEventDirector()
{
	
	for(std::list<MTAtom *>::iterator it = _mtatoms.begin(); it != _mtatoms.end(); ++it)
	{
		if( *it )
		{
			delete *it;
		}
	}
}

void MTEventDirector::preEvent(const edm::Event & iEvent, const edm::EventSetup & iSetup)
{
	this->_Event = &iEvent;
	this->_iSetup = &iSetup;

	// Get common EDM handles
	edm::Handle<reco::BeamSpot> beamSpot;
	iEvent.getByLabel("offlineBeamSpot", beamSpot);
	_activehandles["BeamSpot"]=beamSpot.product();
	
	// Get all the EDM handles dependent of the objects. 
	// In this method must be added any new handle type
	for(std::list<MTAtom *>::iterator it = _mtatoms.begin(); it != _mtatoms.end(); ++it)
	{
		const std::string objectname = (*it)->getobjectname();
		const std::vector<std::string> instancenames = (*it)->getinstancesnames();

		for(std::vector<std::string>::const_iterator name = instancenames.begin(); name !=  instancenames.end(); ++name)
		{
			if( objectname == "Vertex" )
			{
				edm::Handle<std::vector<reco::Vertex> > handle; 
				iEvent.getByLabel( (*name), handle);
				// Product in memory, registring
				_activehandles[*name] = handle.product();
			}
			else if( objectname == "Muon" )
			{
				edm::Handle<edm::View<pat::Muon> > handle; 
				iEvent.getByLabel( (*name), handle);
				// Product in memory, registring
				_activehandles[*name] = handle.product();
			}
			else if( objectname == "Electron" )
			{
				edm::Handle<edm::View<pat::Electron> > handle; 
				iEvent.getByLabel( (*name), handle);
				// Product in memory, registring
				_activehandles[*name] = handle.product();
			}
			else if( objectname == "Track" )
			{
				edm::Handle<std::vector<reco::Track> > handle; 
				iEvent.getByLabel( (*name), handle);
				// Product in memory, registring
				_activehandles[*name] = handle.product();
			}
		}
	}

	// Magnetic Field ---
	// If is real data then derive bfield using the
	// magnet current from DcsStatus
	// otherwise take it from the IdealMagneticFieldRecord
	this->_bField = 0.0;
	if( iEvent.eventAuxiliary().isRealData() )
	{
		// scale factor = 3.801/18166.0 which are
		// average values taken over a stable two
		// week period
		edm::Handle<std::vector<DcsStatus> > dcsHandle;
		iEvent.getByLabel("scalersRawToDigi", dcsHandle);
		
		if( dcsHandle.isValid() ) 
		{
			if( (*dcsHandle).size()==0 )
			{
				edm::LogError("DetStatus") << "Error! dcsStatus has size 0, bField set to magneticField->inTesla(GlobalPoint(0.,0.,0.)).z()" ;
				edm::ESHandle<MagneticField> magneticField;
				iSetup.get<IdealMagneticFieldRecord>().get(magneticField);
				this->_bField = magneticField->inTesla(GlobalPoint(0.,0.,0.)).z();
			}
			else 
			{
				float currentToBFieldScaleFactor = 2.09237036221512717e-04;
				float current = (*dcsHandle)[0].magnetCurrent();
				this->_bField = current*currentToBFieldScaleFactor;
			}
		}
	}
	else 
	{
		edm::ESHandle<MagneticField> magneticField;
		iSetup.get<IdealMagneticFieldRecord>().get(magneticField);
		
		this->_bField = magneticField->inTesla(GlobalPoint(0.,0.,0.)).z();
	}
}

// To be called after the filling of the event
void MTEventDirector::postEvent()
{
	// 
	for(std::list<MTAtom *>::iterator it = this->_mtatoms.begin(); it != this->_mtatoms.end(); ++it)
	{
		(*it)->Clean();
	}
	// Deregistring the handles
	_activehandles.clear();
}

void MTEventDirector::produceEvent()
{
	for(std::list<MTAtom *>::iterator it = this->_mtatoms.begin(); it != this->_mtatoms.end(); ++it)
	{
		(*it)->produce(this);
	}
}

void * MTEventDirector::requestProduct(const std::string & instance)
{
	std::string realinstancename = instance;
	// If this method is called with the generic name of an object, it will take the first
	// instance of that object. Checking that possibility 
	if( _activehandles.find( instance ) == _activehandles.end() )
	{
		// Not found the string 'instance' as key of the dict, so assuming that 'instance'
		// is a generic object name (Track, Muon, ...)
		realinstancename = labelFromGenericName( instance );

		/*for(std::list<MTAtom *>::iterator it = _mtatoms.begin(); it != _mtatoms.end(); ++it)
		{
			const std::string objectname = (*it)->getobjectname();
			if( instance == objectname )
			{
				const std::vector<std::string> instancenames = (*it)->getinstancesnames();
				// Just the first one
				realinstancename = instancenames.at(0);
				break;
			}
		}*/
	}

	// Now, got a real name of an instance
	for(std::map<std::string,const void *>::iterator it = _activehandles.begin(); it != _activehandles.end(); ++it)
	{
		if( realinstancename == it->first )
		{
			// In order to treat with edm::Handle (overloaded * operator)
			return const_cast<void *>(it->second);
		}
	}
	//Lanzar exception de cms o devolver 0?
	throw cms::Exception("UnimplementedFeature") <<  "MTEventDirector::requestProduct: the type " << instance <<
			" can not be retrieved because is not implemented!\n" << 
			"You can do it in the MTEventDirector::preEvent method" << std::endl;
	// Creo que es mejor que devuelva 0 (vector nulo) cuando no encuentra lo que busca, de esta
	// forma el usuario puede decidir que hacer pero siempre debe comprobar que el vector devuelto no 
	// es nulo
	// return 0;
}

std::string MTEventDirector::labelFromGenericName( const std::string & genericName )
{
	for(std::list<MTAtom *>::iterator it = _mtatoms.begin(); it != _mtatoms.end(); ++it)
	{
		const std::string objectname = (*it)->getobjectname();
		if( genericName == objectname )
		{
			const std::vector<std::string> instancenames = (*it)->getinstancesnames();
			// Just the first one
			return instancenames.at(0);
		}
	}

	return std::string();
}
