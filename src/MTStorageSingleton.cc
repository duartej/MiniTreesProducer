// -*- C++ -*-
//
// Package:    MiniTreesProducer
// Class:      MTStorageSingleton.h
// 
/**\class MTStorageSingleton MTStorageSingleton.hh MiniTrees/MiniTreesProducer/src/MTStorageSingleton.cc

 Description: Singleton class in charge of storage.

 Implementation:  It is called by the Builder to construct
	 	  the TBranchs associated to this object.
     
*/
//
// Original Author: Jordi Duarte Campderros  
//         Created:  Wed Nov  24 12:48:11 CET 2010
// 
// jordi.duarte.campderros@cern.ch
//
//

#include "MiniTrees/MiniTreesProducer/interface/MTStorageSingleton.h"

MTStorageSingleton* MTStorageSingleton::_instance = 0;
TFile* MTStorageSingleton::_fs = 0;
TTree* MTStorageSingleton::_tree = 0;

MTStorageSingleton* MTStorageSingleton::instance(const std::string & outfilename)
{
	if( _instance == 0)
	{
		_instance = new MTStorageSingleton();
		// Instance for the file and for the tree
		_fs = new TFile( outfilename.c_str(),"UPDATE" );
		_tree = new TTree( "Tree" , "MiniTree" );
	}

	return _instance;
}


MTStorageSingleton::~MTStorageSingleton()
{
	if( _fs )
	{
		_fs->Write();
		_fs->Close();
		_fs->Delete();

		delete _fs;
	}
/*CUIDADO... ya lo hace el file,, no?
	if( _tree )
	{
		delete _tree;
	}*/
}

void MTStorageSingleton::fill() const
{
	_tree->Fill();
}
