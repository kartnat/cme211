import sys
import time

ref_file = str(sys.argv[1])
reads_file = str(sys.argv[2])
align_file = str(sys.argv[3])

#Open reference, reads and alignment files
ref_file = open(ref_file, "r")
reads_file = open(reads_file, "r")
align_file = open(align_file, "w")


ref = ref_file.read()

#Stripping newline character
ref = ref.strip()

#Begin timer
startTime = time.time()

#Initializing number reads and number alignments counters, for the later printed output
nreads = 0
alignsZero = 0
alignsOnce = 0
alignsTwice = 0

#Beginning the alignment implementation
for read in reads_file:
    nreads += 1
    read = read.strip()
    readPos = ref.find(read)     #Finding the first location where the read is found (or not found)
    #If the read isn't found print the read followed by -1 in the alignment file, and increase the corresponding counter by one.  Else we just write the read
    if readPos == -1:
        alignsZero += 1
        align_file.write("{} {}".format(read, readPos))
    else:
        align_file.write("{} ".format(read))
    refIndex = 0
    counter = 0
    #Where the magic happens--writing reads and alignment positions where there is at least one alignment
    while readPos != -1:
        refIndex = refIndex + readPos
        align_file.write("{} ".format(refIndex))
        refIndex = refIndex + len(read)
        newRef = ref[refIndex:]
        readPos = newRef.find(read)
        #Determines during the initial pass through the loop if there is one or more than one alignment
        if (readPos != -1) and (counter == 0):
            alignsTwice += 1
        elif counter == 0:
            alignsOnce += 1
        counter += 1
    align_file.write("\n")

#End timer
endTime = time.time()

print('reference length: {}'.format(len(ref)))
print('number reads: {}'.format(nreads))
print('aligns 0: {}'.format(alignsZero/nreads))
print('aligns 1: {}'.format(alignsOnce/nreads))
print('aligns 2: {}'.format(alignsTwice/nreads))
print('elapsed time: {}'.format(endTime - startTime))


