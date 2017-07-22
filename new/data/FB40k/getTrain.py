rawfile = open('../../../RawData/FB40k/FB40k.txt', 'r')
trainfile = open('train.txt', 'w')

for line in rawfile:
    h, t, r = line.strip().split()
    trainfile.write(h + '\t' + r + '\t' + t + '\n')

rawfile.close()
trainfile.close()