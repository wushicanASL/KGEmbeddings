import sys

inFile = open(sys.argv[1] + '.rbox', 'r')
outFile = open(sys.argv[1] + '_filtered.rbox', 'w')

acceptable_rbox = {'symm', 'tran', 'refl', 'asym', 'irre', 'subr', 'subc'}

for line in inFile:
    if line.strip().split()[0] in acceptable_rbox:
        outFile.write(line)

inFile.close()
outFile.close()
