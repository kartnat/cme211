import sys
import time
import random

if __name__ == "__main__":
    if len(sys.argv) <= 5:
        # no argumemnts, print usage message
        print("Usage:")
        print(" $ python3 generatedata.py <ref_length> <nreads> <read_len> <ref_file> <reads_file>")
        sys.exit()

ref_length = int(sys.argv[1])
nreads = int(sys.argv[2])
read_len = int(sys.argv[3])
ref_file = str(sys.argv[4])
reads_file = str(sys.argv[5])

#Begin timer
startTime = time.time()

###Creating reference file
ref_file = open(ref_file, "w")
randLength = int(.75*ref_length)
randString = ''

#Generating random part of reference
for k in range(0, randLength):
    randomNum = random.randint(0,3) 
#Each of the four random numbers represents one of the upper case letters 
    if randomNum == 0:
        randString = randString+'A'
    elif randomNum == 1:
        randString = randString+'C'
    elif randomNum == 2:
        randString = randString+'G'
    else: randString = randString+'T'

#Taking last 25% of randomly generated sequence and appending to the same
copyBlock = randString[int(.5*ref_length): randLength]
reference = randString+copyBlock

#Writing reference to disk
ref_file.write(reference)
ref_file.close()

###Creating read file
reads_file = open(reads_file, "w")
numAlignsZero = 0
numAlignsOnce = 0
numAlignsTwice = 0

#3 cases: 15% of interval [0,1) for reads that align zero times, 75% for reads that align exactly once, and 10% for the reads that align twice 
for k in range(0, nreads):
    randomRealNum = random.random()
#First case
    if 0 <= randomRealNum < 0.15:
        numAlignsZero += 1
        read = ''
#Iterating until a missing read is found
        while reference.find(read) != -1:
            read = ''
            for k in range(0, read_len):
                randomNum = random.randint(0,3)
                if randomNum == 0:
                    read = read+'A'
                elif randomNum == 1:
                    read = read+'C'
                elif randomNum == 2:
                    read = read+'G'
                else: read = read+'T'
#Second case
    elif 0.15 <= randomRealNum <0.9:
        numAlignsOnce += 1
        startingPos = random.randint(0, int(.5*ref_length-1))
        read = reference[startingPos:startingPos + read_len]
#Third and last case; note startingPos is picked read_len distance away from end of reference
    else:
        numAlignsTwice += 1
        startingPos = random.randint(int(.75*ref_length), len(reference)-read_len+1)
        read = reference[startingPos:startingPos + read_len]

    reads_file.write(read+"\n")

#Writing read file to disk
reads_file.close()
#End timer
time2 = time.time()

#Optional timing print("{}".format(time2-time1))
print("reference length: {}".format(ref_length))
print("number reads: {}".format(nreads))
print("read length: {}".format(read_len))
print("aligns 0: {}".format(numAlignsZero/nreads))
print("aligns 1: {}".format(numAlignsOnce/nreads))
print("aligns 2: {}".format(numAlignsTwice/nreads))
