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
#ifndef MTStorageSingleton_HH
#define MTStorageSingleton_HH

// System includes
#include<cstring> 
#include<list>

//#include "MiniTrees/MiniTreesProducer/interface/MTAtom.h"

#include "TTree.h"
#include "TFile.h"

class MTAtom;

class MTStorageSingleton
{
	public:
		static MTStorageSingleton* instance(const std::string & outfilename);
		~MTStorageSingleton();
		//
		static void fill();
		static void Register(std::list<MTAtom*> mtatoms);
		static void close();

	protected:
		// Constructor encapsulated
		MTStorageSingleton(){ };

	private:
		static MTStorageSingleton* _instance;

		static TTree *_tree;
		static TFile *_fs;
};

#endif
