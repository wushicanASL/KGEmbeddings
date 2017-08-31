import sys

dataset = sys.argv[1]
threshold = int(sys.argv[2])

updateFile = open('data/' + dataset + '/update.txt', 'r')
eset = set()
rset = set()
for line in updateFile:
    items = line.strip().split()
    eset.add(items[0])
    rset.add(items[1])
    eset.add(items[2])
updateFile.close()

testFile = open('data/' + dataset + '/test.txt', 'r')
filteredTestFile = open('data/' + dataset + '/test_filtered.txt', 'w')

if dataset in {'FB15k', 'WN18'} :
    for line in testFile:
        items = line.strip().split()
        itemCounter = 0
        if items[0] in eset:
            itemCounter += 1
        if items[1] in rset:
            itemCounter += 1
        if items[2] in eset:
            itemCounter += 1
        if itemCounter >= threshold:
            filteredTestFile.write(line)
else:
    flag = 0
    itemCounter = 0
    lastLine = ''
    for line in testFile:
        items = line.strip().split()
        if items[0] in eset:
            itemCounter += 1
        if items[1] in rset:
            itemCounter += 1
        if items[2] in eset:
            itemCounter += 1
        if flag == 1:
            if itemCounter >= threshold:
                filteredTestFile.write(lastLine)
                filteredTestFile.write(line)
            itemCounter = 0
        else:
            lastLine = line

testFile.close()
filteredTestFile.close()

print len(rset), len(eset)
