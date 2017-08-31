import sys

inFile = open(sys.argv[1] + '.rbox', 'r')
outFile = open(sys.argv[1] + '_filtered.rbox', 'w')

acceptable_rbox = {'Symmetric', 'Transitive', 'Reflexive', , 'Subproperty', 'Subproperty of Chain'}

for line in inFile:
    if line.strip().split('\t')[0] in acceptable_rbox:
        outFile.write(line)

inFile.close()
outFile.close()
