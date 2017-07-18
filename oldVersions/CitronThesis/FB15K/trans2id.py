import sys

inFileName = sys.argv[1] + '.txt'
outFileName = sys.argv[1] + '2id.txt'

e2idFile = open('entity2id.txt', 'r')
e2id = {}
firstLine = True
for line in e2idFile:
    if firstLine:
        firstLine = False
    else:
        items = line.strip().split()
        e2id[items[0]] = items[1]
e2idFile.close()

r2idFile = open('relation2id.txt', 'r')
r2id = {}
firstLine = True
for line in r2idFile:
    if firstLine:
        firstLine = False
    else:
        items = line.strip().split()
        r2id[items[0]] = items[1]
r2idFile.close()

inFile = open(inFileName, 'r')
outFile = open(outFileName, 'w')
for line in inFile:
    items = line.strip().split()
    if len(items) == 3:
        outFile.write(e2id[items[0]] + '\t' + e2id[items[1]] + '\t' + r2id[items[2]] + '\n')
    else:
        print 'error!'
inFile.close()
outFile.close()
