import random

e2id = {}
r2id = {}
testset = []
validset = []
trainset = []
addset = []

enum = 0
rnum = 0

trainFile = open('train.txt', 'r')
for line in trainFile:
    items = line.strip().split()
    if not items[0] in e2id:
        e2id[items[0]] = enum
        enum = enum + 1
    if not items[1] in r2id:
        r2id[items[1]] = rnum
        rnum = rnum + 1
    if not items[2] in e2id:
        e2id[items[2]] = enum
        enum = enum + 1
    if random.randint(0, 10) > 0:
        trainset.append(items)
    else:
        addset.append(items)
trainFile.close()

testFile = open('test.txt', 'r')
for line in testFile:
    items = line.strip().split()
    if not items[0] in e2id:
        e2id[items[0]] = enum
        enum = enum + 1
    if not items[1] in r2id:
        r2id[items[1]] = rnum
        rnum = rnum + 1
    if not items[2] in e2id:
        e2id[items[2]] = enum
        enum = enum + 1
    testset.append(items)
testFile.close()

validFile = open('dev.txt', 'r')
for line in validFile:
    items = line.strip().split()
    if not items[0] in e2id:
        e2id[items[0]] = enum
        enum = enum + 1
    if not items[1] in r2id:
        r2id[items[1]] = rnum
        rnum = rnum + 1
    if not items[2] in e2id:
        e2id[items[2]] = enum
        enum = enum + 1
    validset.append(items)
validFile.close()

e2idFile = open('entity2id.txt', 'w')
e2idFile.write(str(enum) + '\n')
for (key, value) in e2id.items():
    e2idFile.write(key + '\t' + str(value) + '\n')
e2idFile.close()

r2idFile = open('relation2id.txt', 'w')
r2idFile.write(str(rnum) + '\n')
for (key, value) in r2id.items():
    r2idFile.write(key + '\t' + str(value) + '\n')
r2idFile.close()

at2idFile = open('alltriple2id.txt', 'w')
at2idFile.write(str(len(trainset) + len(addset) + len(testset) + len(validset)) + '\n')
t2idFile = open('triple2id.txt', 'w')
t2idFile.write(str(len(trainset)) + '\n')
for triple in trainset:
    t2idFile.write(str(e2id[triple[0]]) + '\t' + str(e2id[triple[2]]) + '\t' + str(r2id[triple[1]]) + '\n')
    at2idFile.write(str(e2id[triple[0]]) + '\t' + str(e2id[triple[2]]) + '\t' + str(r2id[triple[1]]) + '\n')
t2idFile.close()
add2idFile = open('add2id.txt', 'w')
add2idFile.write(str(len(addset)) + '\n')
for triple in addset:
    add2idFile.write(str(e2id[triple[0]]) + '\t' + str(e2id[triple[2]]) + '\t' + str(r2id[triple[1]]) + '\n')
    at2idFile.write(str(e2id[triple[0]]) + '\t' + str(e2id[triple[2]]) + '\t' + str(r2id[triple[1]]) + '\n')
add2idFile.close()
test2idFile = open('test2id.txt', 'w')
for triple in testset:
    test2idFile.write(str(e2id[triple[0]]) + '\t' + str(r2id[triple[1]]) + '\t' + str(e2id[triple[2]]) + '\n')
    at2idFile.write(str(e2id[triple[0]]) + '\t' + str(e2id[triple[2]]) + '\t' + str(r2id[triple[1]]) + '\n')
test2idFile.close()
for triple in validset:
    at2idFile.write(str(e2id[triple[0]]) + '\t' + str(e2id[triple[2]]) + '\t' + str(r2id[triple[1]]) + '\n')
at2idFile.close()

