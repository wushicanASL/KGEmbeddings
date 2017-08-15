import numpy as np
import random
import time
import cPickle

read_file_start = time.time()
entity_list = []
relation_list = []



# read FB40k
fb40k_data = open('../FB40k/FB40k.txt')
fb40k_list = []
InterSection = open('./add2FB40k.txt','w')
for contend in fb40k_data:
    con = contend.strip('\n')
    fb40k_list.append(con)
    con_list = con.split('\t')
    h =  '/'+con_list[0].replace('.','/')
    t = '/'+con_list[1].replace('.','/')
    entity_list.append(h)
    entity_list.append(t)
    relation_list.append(con_list[2])

fb40k_data.close()




entity_list = list(set(entity_list))
relation_list = list(set(relation_list))
# read train set triples
train_data = open('./train.txt')
train_list = []
for contend in train_data:
    con = contend.strip('\n')
    con_list = con.split('\t')
    h = con_list[0]
    t = con_list[2]
    r = con_list[1]
    if ((h in entity_list) or (t in entity_list)) or (r in relation_list):
        InterSection.write(h.replace("/m/","m.")+'\t'+t.replace("/m/","m.")+'\t'+r.replace("/m/","m.")+'\n')
        print("Find it")
train_data.close()
train_list = list(set(train_list))

#read dev set triples

dev_triples = []
dev_data = open('./valid.txt')
dev_relation_set = set() #change
for contend in dev_data:
    con_list = contend.strip('\n').split('\t')
    h = con_list[0]
    t = con_list[2]
    r = con_list[1]
    if ((h in entity_list) or (t in entity_list)) or (r in relation_list):
        InterSection.write(h.replace("/m/","m.")+'\t'+t.replace("/m/","m.")+'\t'+r.replace("/m/","m.")+'\n')
        print("Find it")
    dev_relation_set.add(con_list[1])
dev_data.close() 
dev_list = list(set(dev_triples))

print("dev_relation_set\n")
print(len(list(dev_relation_set)))

#read test set triples

test_triples = []
test_relation_set = set() #change
test_data = open('./test.txt')
for contend in test_data:
    con_list = contend.strip('\n').split('\t')
    h = con_list[0]
    t = con_list[2]
    r = con_list[1]
    if ((h in entity_list) or (t in entity_list)) or (r in relation_list):
        InterSection.write(h.replace("/m/","m.")+'\t'+t.replace("/m/","m.")+'\t'+r.replace("/m/","m.")+'\n')
        print("Find it")
test_relation_set.add(con_list[1])
test_data.close()   
test_list = list(set(test_triples))

print("test_relation_set\n")
print(len(list(test_relation_set)))

all_list = train_list + dev_list + test_list



print(len(entity_list))
print(len(relation_list))


InterSection.close()

