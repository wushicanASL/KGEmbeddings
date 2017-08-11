import sys

filename = sys.argv[1] + '/' + sys.argv[2] + '.txt'

lines = []
inFile = open(filename, 'r')
for line in inFile:
    lines.append(line.strip())
inFile.close()

if (len(lines[0].split()) == 3):
    outFile = open(filename, 'w')
    for line in lines:
        outFile.write(line + '\t1\n')
    outFile.close()
