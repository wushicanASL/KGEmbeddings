import sys
import string

fin = open(sys.argv[1], 'r')
fout = open(sys.argv[2], 'w')
tab = string.atoi(sys.argv[3])
now = 0
for line in fin:
    items = line.strip().split()
    now = now + tab
    fout.write('        ' + str(now) + '\t')
    for item in items:
        fout.write('& ' + item + '\t')
    fout.write('\\\\\n')
fin.close()
fout.close()

