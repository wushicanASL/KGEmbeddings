import numpy as np
import random
import time
import cPickle

read_file_start = time.time()
entity_list = []
relation_list = []


# read train set triples
train_data = open('./train.txt')
train_list = []
relation_set = set()
for contend in train_data:
    con = contend.strip('\n')
    train_list.append(con)
    con_list = con.split('\t')
    entity_list.append(con_list[0])
    entity_list.append(con_list[2])
    relation_list.append(con_list[1])
    relation_set.add(con_list[1])

train_data.close()
train_list = list(set(train_list))

print("hello\n")
print(len(list(relation_set)))

#read dev set triples
count = 0
dev_triples = []
dev_data = open('./dev.txt')
for contend in dev_data:
    count += 1
    if(count % 2 == 0):
    	dev_triples.append(contend.strip('\n'))
    con_list = contend.strip('\n').split('\t')
    entity_list.append(con_list[0])
    entity_list.append(con_list[2])
    relation_list.append(con_list[1])
dev_data.close() 
dev_list = list(set(dev_triples))

#read test set triples
count_c = 0
test_triples = []
test_data = open('./test.txt')
for contend in test_data:
    count_c += 1
    if(count_c % 2 == 0):
    	test_triples.append(contend.strip('\n'))
    con_list = contend.strip('\n').split('\t')
    entity_list.append(con_list[0])
    entity_list.append(con_list[2])
    relation_list.append(con_list[1])
test_data.close()   
test_list = list(set(test_triples))

all_list = train_list + dev_list + test_list

entity_list = list(set(entity_list))
relation_list = list(set(relation_list))


entity2id = {}
entity2id_fid = open('./entity2id.txt','w')
count = 0 
for e in entity_list:
    entity2id_fid.write(e+' '+ str(count) + '\n')
    entity2id[e] = count
    count += 1
entity2id_fid.close()

relation2id = {}
relation2id_fid = open('./relation2id.txt','w')
count = 0 
for e in relation_list:
    relation2id_fid.write(e+' '+ str(count) + '\n')
    relation2id[e] = count
    count += 1
relation2id_fid.close()

n_relation = len(relation_list)
left_set = [set() for i in xrange(n_relation)]
right_set = [set() for i in xrange(n_relation)]

for e in all_list:
    h, r, t = e.split('\t')[0:3]
    r_idx = relation2id[r]
    left_set[r_idx].add(h)
    right_set[r_idx].add(t)

l_sparse = np.zeros(n_relation)
r_sparse = np.zeros(n_relation)
for j in xrange(n_relation):
    l_sparse[j] = len(left_set[j])
    r_sparse[j] = len(right_set[j])

max_val = np.max([np.max(l_sparse ),np.max(r_sparse )])
l_sparse = 1. * l_sparse / max_val
r_sparse = 1. * r_sparse / max_val

n=20
idx_list = [i for i in xrange(n)] 

l_n_para = 0.3*l_sparse * n * n
r_n_para = 0.3*r_sparse * n * n
set_num_fid = open('./set_num_l.txt','w')
for i in xrange(n_relation):
    row_idx = []
    col_idx = []
    for j in xrange(int(l_n_para[i])):
        row_idx.append(random.choice(idx_list))
        col_idx.append(random.choice(idx_list))
    for k in xrange(n):
        k_idx_list = []
        for t in xrange(int(l_n_para[i])):
            if(row_idx[t] == k):
                k_idx_list.append(col_idx[t])
        k_idx_list.append(k)
        k_idx_list = list(set(k_idx_list))
        k_idx_array = np.sort(np.asarray(k_idx_list, 'int32'))
        k_idx_list = list(k_idx_array)
        set_num_fid.write(str(len(k_idx_list))+ ' ')
        for idx in k_idx_list[:-1]:
            set_num_fid.write(str(idx)+ ' ')
        set_num_fid.write(str(k_idx_list[-1])+'\n')
set_num_fid.close()


set_num_fid = open('./set_num_r.txt','w')
for i in xrange(n_relation):
    row_idx = []
    col_idx = []
    for j in xrange(int(r_n_para[i])):
        row_idx.append(random.choice(idx_list))
        col_idx.append(random.choice(idx_list))
    for k in xrange(n):
        k_idx_list = []
        for t in xrange(int(r_n_para[i])):
            if(row_idx[t] == k):
                k_idx_list.append(col_idx[t])
        k_idx_list.append(k)
        k_idx_list = list(set(k_idx_list))
        k_idx_array = np.sort(np.asarray(k_idx_list, 'int32'))
        k_idx_list = list(k_idx_array)
        set_num_fid.write(str(len(k_idx_list))+ ' ')
        for idx in k_idx_list[:-1]:
            set_num_fid.write(str(idx)+ ' ')
        set_num_fid.write(str(k_idx_list[-1])+'\n')
set_num_fid.close()



