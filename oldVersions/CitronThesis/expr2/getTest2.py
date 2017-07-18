eset = set()
rset = set()

addFile = open('add2id.txt', 'r')
for line in addFile:
    items = line.strip().split()
    if len(items) == 3:
        eset.add(items[0])
        eset.add(items[1])
        rset.add(items[2])
addFile.close()

testFile = open('test2id.txt', 'r')
test2File = open('test2.txt', 'w')
for line in testFile:
    items = line.strip().split()
    score = 0;
    if items[0] in eset:
        score = score + 1
    if items[1] in rset:
        score = score + 1
    if items[2] in eset:
        score = score + 1
    if score > 2:
        test2File.write(line)
testFile.close()
test2File.close()
