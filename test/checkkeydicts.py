#!/usr/bin/env python

#####
##  Utility to check if the MTAtom class
##  source file is coherently constructed,
##  i.e. the registryvalues and storevalues
##  methods have the same branches.
##
##  Use: python checkkeydicts.py <MTclass.cc>
####

import sys

fname = sys.argv[1]

f = open(fname)

lines = f.readlines()

keyputted = { 'INT': set([]), 'FLOAT': set([]) }
used = { 'INT': set([]), 'FLOAT': set([]) }

for l in lines:
	if ( l.find( '_IVALUES' ) != -1 or l.find( '_FVALUES' ) != -1 ) and l.find('.push_back(') != -1:
		if l.find( '_IVALUES' ) != -1:
			what = 'INT'
		if l.find( '_FVALUES' ) != -1:
			what = 'FLOAT'	
		var = l.split('"')[1]
		keyputted[what].add( var ) 

	if ( l.find( '_intMethods' ) != -1 or l.find( '_floatMethods' ) != -1 ) and l.find(']->push_back(') != -1:
		if l.find( '_intMethods' ) != -1:
			what = 'INT'
		if l.find( '_floatMethods' ) != -1:
			what = 'FLOAT'		
		var = l.split('"')[1]
		used[what].add(var)


if len(keyputted['INT'].difference( used['INT'] )) > 0 :
	print "There are differences between the INT keys"
	a = used['INT'].difference( keyputted['INT'] )
	print "INT Keys used (in storevalues) but not instanciated (in registryvalues):"
	for i in a:
		print i,
	print "INT Keys instanciated (in registryvalues) and do not have counterpart at storevalues:"
	b = keyputted['INT'].difference( used['INT'] )
	for i in b:
		print i,
	print 
else:
	print 'The INT keys seems to be okey'


if len(keyputted['FLOAT'].difference( used['FLOAT'] )) > 0:
	print "There are differences between the FLOAT keys"
	a = used['FLOAT'].difference( keyputted['FLOAT'] )
	print "FLOAT Keys used (in storevalues) but not instanciated (in registryvalues):"
	for i in a:
		print i,
	print "FLOAT Keys instanciated (in registryvalues) and do not have counterpart at storevalues:"
	b = keyputted['FLOAT'].difference( used['FLOAT'] )
	for i in b:
		print i,
	print 
	print
else:
	print 'The FLOAT keys seems to be okey'

		

