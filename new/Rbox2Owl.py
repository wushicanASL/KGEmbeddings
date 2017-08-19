import numpy as np
import random
import time
import cPickle

read_file_start = time.time()
h_list = []
r_list = []
t_list = []

Rbox_data = open('./FB15k.rbox')
Rbox_list = []

for contend in Rbox_data:
	con = contend.strip('\n')
	Rbox_list.append(con)
	con_list = con.split('\t')
	#print(con_list)
	#flag = 'False'
	if (con_list[0]=='Asymmetric') or (con_list[0]=='Irreflexive') or(con_list[0]=='Symmetric')\
	or(con_list[0]=='Transitive')or(con_list[0]=='Functional')or(con_list[0]=='Inverse Functional')or(con_list[0]=='Reflexive'):
	#	flag = 'True'
		h_list.append(con_list[0])
		r_list.append(con_list[1])
		t_list.append(1)
	else:
		h_list.append(con_list[0])
		r_list.append(con_list[1])
		t_list.append(con_list[2])
	#print(flag+'\n')

Rbox_data.close()

set_owl = open('./FB15k.owl','w')
#n = len(h_lsit)

set_owl.write('<?xml version="1.0"?>'+'\n')
set_owl.write('<rdf:RDF xmlns="owlapi:ontology:ont1#"'+'\n')
set_owl.write('\t'+'xml:base="owlapi:ontology:ont1"'+'\n')
set_owl.write('\t'+'xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"'+'\n')
set_owl.write('\t'+'xmlns:owl="http://www.w3.org/2002/07/owl#"'+'\n')
set_owl.write('\t'+'xmlns:xsd="http://www.w3.org/2001/XMLSchema#"'+'\n')
set_owl.write('\t'+'xmlns:rdfs="http://www.w3.org/2000/01/rdf-schema#"'+'\n')
set_owl.write('\t'+'xmlns:families="http://example.com/owlapi/families#">'+'\n')
set_owl.write('\t'+'<owl:Ontology rdf:about="owlapi:ontology:ont1"/>'+'\n'+'\n'+'\n'+'\n')

set_owl.write('\t'+'<!--'+'\n') 
set_owl.write('\t'+'///////////////////////////////////////////////////////////////////////////////////////'+'\n')
set_owl.write('\t'+'//'+'\n')
set_owl.write('\t'+'// Object Properties'+'\n')
set_owl.write('\t'+'//'+'\n')
set_owl.write('\t'+'///////////////////////////////////////////////////////////////////////////////////////'+'\n')
set_owl.write('\t'+'-->'+'\n'+'\n'+'\n'+'\n'+'\n')


flag  = 0
n = len(r_list) - 1
needWrite = 0
for e in range(n):
	if(r_list[e] != r_list[e+1] ):
		needWrite = 1
	else:
		needWrite = 0
	if t_list[e] == 1:
		if flag == 0:
			set_owl.write('\t'+'<!-- http://example.com/owlapi/families#'+r_list[e]+' -->'+'\n'+'\n')
			set_owl.write('\t'+'<owl:ObjectProperty rdf:about="http://example.com/owlapi/families#'+r_list[e]+'">'+'\n')
			set_owl.write('\t'+'\t'+'<rdf:type rdf:resource="http://www.w3.org/2002/07/owl#'+h_list[e]+'"/>'+'\n')
			flag = 1
		else:
			set_owl.write('\t'+'\t'+'<rdf:type rdf:resource="http://www.w3.org/2002/07/owl#'+h_list[e]+'"/>'+'\n')
	else:
		if (h_list[e] == 'Disjoint'):
			set_owl.write('\t'+'\t'+'<owl:property'+h_list[e]+'With rdf:resource="http://example.com/owlapi/families#'+t_list[e]+'"/>'+'\n')
		if (h_list[e] == 'Subproperty'):
			set_owl.write('\t'+'\t'+'<owl:property'+h_list[e]+'Of rdf:resource="http://example.com/owlapi/families#'+t_list[e]+'"/>'+'\n')
		if (h_list[e] == 'Functional'):
			set_owl.write('\t'+'\t'+'<rdf:type rdf:resource="http://www.w3.org/2002/07/owl#'+h_list[e]+'"/>'+'\n')
		if (h_list[e] == 'Inverse Functional'):
			set_owl.write('\t'+'\t'+'<rdf:type rdf:resource="http://www.w3.org/2002/07/owl#'+h_list[e]+'"/>'+'\n')
	if (needWrite == 1):
		set_owl.write('\t'+'</owl:ObjectProperty>'+'\n'+'\n'+'\n'+'\n')
		needWrite = 0
		flag = 0

set_owl.write('\t'+'</owl:ObjectProperty>'+'\n'+'\n'+'\n'+'\n')
set_owl.write('</rdf:RDF>')
set_owl.close()
