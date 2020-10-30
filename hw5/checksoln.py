import numpy as np
import sys  

#Checking if maze and solution files were provided
if len(sys.argv) < 3:
    print('Usage:')
    print('  python3 {} <maze file> <solution file>'.format(sys.argv[0]))
    sys.exit(0)

mazefile = sys.argv[1]
solnfile = sys.argv[2]

mazefile = open(mazefile, "r")
solnfile = open(solnfile, "r")

#Reading in just the first row to get the dimensions of the maze
mazeFirstLine = mazefile.readline().split()
rows = mazeFirstLine[0]
cols = mazeFirstLine[1]

#Initializing all entries in array to one
maze = np.ones((int(rows),int(cols)))

#Changing wall locations to zeroes, just as in the C++ array
for line in mazefile:
    line = line.split()
    rowIndex = int(line[0])
    colIndex = int(line[1])
    maze[rowIndex][colIndex] = 0

#Reading in first line of the solution file
solnFirstLine = solnfile.readline().split()
startRow = int(solnFirstLine[0])
startCol = int(solnFirstLine[1])

#If user didn't enter in the non-wall location on the first 
#row, print that the solution is invalid
if startRow != 0 or maze[startRow][startCol] == 0:
    print("Solution is invalid")
else:
    prevRow = startRow
    prevCol = startCol
    #Going through each of the rows in the solution file, determining
    #if user is making invalid moves or passing through walls
    for line in solnfile:
        line = line.split()
        nextRow = int(line[0])
        nextCol = int(line[1])
        #Exited bounds of maze
        if nextRow > int(rows)-1 or nextCol > int(cols)-1:
            print("Solution is invalid")
            break
        #Staying at the same place    
        elif nextRow==prevRow and nextCol==prevCol:
            print("Solution is invalid")
            break
        #Moving more than unit distance
        elif nextRow > prevRow+1 or nextRow < prevRow-1:
            print("Solution is invalid")
            break
        elif nextCol > prevCol+1 or nextCol < prevCol-1:
            print("Solution is invalid")
            break 
        #Making diagonal moves   
        elif nextRow != prevRow and nextCol != prevCol:
            print("Solution is invalid")
            break
        #At a wall location
        elif maze[nextRow][nextCol] == 0:
            print("Solution is invalid")
            break 
        #If current move is fine go to the next row of file
        else:
            prevRow = nextRow
            prevCol = nextCol
    #Solution is valid if we've reached the last row of maze
    if nextRow == int(rows)-1:
        print("Solution is valid!")
    else:
        print("Solution is invalid")
