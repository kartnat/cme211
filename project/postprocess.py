import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
import sys

#Reading input file (e.g. input1.txt)
inputFile = sys.argv[1]
inputFile = open(inputFile, "r");
params = inputFile.readline().strip()
params = params.split()
length, width, height = float(params[0]), float(params[1]), float(params[2])
nrows = int(width/height-1)
ncols = int(length/height)

#Reading the solution file (e.g. solution010.txt)
solnFile = sys.argv[2]
solnFile = open(solnFile, "r");
solnList = []
sum=0

#Creating list of the solution values
for entry in solnFile:
    solnList.append(float(entry))
    sum = sum+float(entry)
meanTemp = sum/len(solnList)

#Populating 2d heat array
heat = np.zeros((nrows,ncols))
k=0
for j in range(0,ncols):
    for i in range (0,nrows):
        heat[i,j] = solnList[k]
        k = k+1

#Isoline
iso = []
#Calculating mean of every column in matrix
for j in range(0,ncols):
    avg = np.mean(heat[:,j])
    iso.append(avg)
x = np.linspace(0, ncols-1, ncols)

plt.figure()            
plt.imshow(heat)
plt.hold(True)
plt.plot(x,iso)
plt.colorbar();
plt.xlim(-5, ncols+5)
plt.ylim(nrows, 0)
plt.savefig('plot.png')
    
print("Input file processed: {}".format(sys.argv[1]))
print("Mean Temperature: {0:.5f}".format(meanTemp))
