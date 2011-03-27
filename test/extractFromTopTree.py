#!/usr/bin/env python

import sys

f = open('TopTreeProducer_25Marzo.cc')

lines = f.readlines()

variables = set([])

tomatch = sys.argv[1]
for l in lines:
	if l.find( '*T_'+tomatch+'_' ) != -1 or l.find( '* T_'+tomatch+'_') != -1:
		where = l.find('T_'+tomatch+'_')
		var = l.split('<')[1].split('>')[0]
		variables.add( (var,l[where:].split(';')[0]) ) 

for v in variables:
	if v[0] == 'int':
		print '_IVALUES.push_back("'+v[1]+'");'
	elif v[0] == 'float':
		print '_FVALUES.push_back("'+v[1]+'");'
	elif v[0] == 'bool':
		print '_BVALUES.push_back("'+v[1]+'");'
	else:
		print '------'+ v[1]
		

