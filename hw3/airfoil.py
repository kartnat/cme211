import math
import os 
import sys

""" Class Airfoil: This class computes the lift coffiecients and stagnation points for a given airfoil geometry.  At a high-level it consumes files describing the pressure distribution and geometry of an airfoil and follows a formulaic process to return those values."""

    def __init__(self, inputdir):
        """Initialization method, initializes instance of Airfoil class with attribute inputdir.  Inputdir is a pathname for the directory where all the files are housed. """
        #Raises runtime errors if the directory is invalid or any of the data files don't exist
        if os.path.exists(inputdir) == False:
            raise RuntimeError("Directory does not exist")
        if os.path.exists(inputdir+"xy.dat") == False:
            raise RuntimeError("Panel coordinate file does not exist")
        if os.path.exists(inputdir+"alpha-3.0.dat") == False:
            raise RuntimeError("-3 degree angle of attack file does not exist")
        if os.path.exists(inputdir+"alpha0.0.dat") == False:
            raise RuntimeError("0 degree angle of attack file does not exist")
        if os.path.exists(inputdir+"alpha+3.0.dat") == False:
            raise RuntimeError("+3 degree angle of attack file does not exist")
        if os.path.exists(inputdir+"alpha+6.0.dat") == False:
            raise RuntimeError("+6 degree angle of attack file does not exist")
        if os.path.exists(inputdir+"alpha+9.0.dat") == False:
            raise RuntimeError("+9 degree angle of attack file does not exist")
        else:
            self.inputdir = str(inputdir)

    def read_files(self, coordFile, angleFile):
        """This function takes the names of the coordinate and specific angle of attack files, and returns a dictionary combining both the data files in a way that may be consumed by the later methods (key is a line number, values is a list [x coord, ycoord, pressure coefficient] for panel starting with (x coord, y coord))"""
        dataDict = {}
        coordFile = open(self.inputdir+coordFile, "r")
        angleFile = open(self.inputdir+angleFile, "r")
        lineNuma = 1    #lineNuma counts the number of lines read, for later error handling
        next(coordFile)     #Skip header
        for line in coordFile:
            line = line.split()
            if not line[0] or not line[1]:
                #Runtime error if one (or both) of the coordinates is missing, indicating malformed data
                raise RuntimeError("Line has missing x or y coordinate")
            else:
                dataDict[lineNuma] = [float(line[0]), float(line[1])]
                lineNuma += 1
        #Now going through the angle file
        lineNumb = 1   
        next(angleFile)
        for line in angleFile:
            line = line.split()
            if not line:
                #Runtime error if there is a missing pressure measurement (or there is an unnecessary blank line in the file)
                raise RuntimeError("Line has missing pressure coefficient")
            else:
                #Values in dictionary are lists; appending the pressure coefficient to the list
                dataDict[lineNumb].append(float(line[0]))
                lineNumb += 1
        if lineNuma != (lineNumb + 1):     #Runtime error if every panel doesn't have a pressure coefficent, in the case that there are no missing lines in either file
            raise RuntimeError("Missing pressure coefficient data for one more panels")
        return dataDict

    def chord_length(self, dataDict):
        """This function, as the name implies, returns the chord length of the airfoil.  Just looking at the xy.dat file, this value is approximately one, but the function computes it exactly.  The input is the data dictionary generated in the previous method."""
        chordDict = {} 
        #Initialize dictionary with keys x coordinates, values y coordinates
        for key in dataDict:
            chordDict[dataDict[key][0]] = dataDict[key][1]
        trailCoordx = max(chordDict.keys())
        trailCoordy = abs(chordDict[trailCoordx])     #Positive coordinates by convention
        leadCoordx = min(chordDict.keys())
        leadCoordy = abs(chordDict[leadCoordx])
        lengthx = (trailCoordx - leadCoordx)**2
        lengthy = (trailCoordy - leadCoordy)**2
        length = (lengthx + lengthy)**.5
        return length

    def lift_coeff(self, dataDict, alpha, chordLength):
        """Takes the data dictionary, angle of attack, and chord length to product the corresponding lift coefficient"""
        for key in sorted(dataDict.keys()):
            if key != max(sorted(dataDict.keys())):     #Avoid last line as it doesn't have a "next key"
                currentx = dataDict[key][0]
                nextx = dataDict[key+1][0]
                currenty = dataDict[key][1]
                nexty = dataDict[key+1][1]
                #Append the deltax and deltay for each panel to the list
                dataDict[key].append(nextx - currentx)
                dataDict[key].append(nexty - currenty)
        c_x = 0
        c_y = 0
        #Sums up the x and y components over every panel
        for key in sorted(dataDict.keys()):
            if key != max(sorted(dataDict.keys())):
                x_contrib = -dataDict[key][2]*dataDict[key][4]/chordLength
                y_contrib = dataDict[key][2]*dataDict[key][3]/chordLength
                c_x += x_contrib
                c_y += y_contrib
        alpha = math.radians(alpha)     #Convert degrees to radians
        #Calculate and return the lift coefficient
        lift_coeff = c_y*math.cos(alpha) - c_x*math.sin(alpha)
        return(lift_coeff)
        
    def stagnation_points(self, dataDict):
        """This function computes the stagation points, and returns a tuple consisting of (i) coordinates of the points, (x,y), and (ii) the corresponding pressure"""
        stagDict = {}     
        #Creates dictionary with keys being the pressure coefficients and values being the average coordinates of the corresponding panel
        for key in dataDict.keys():
            if key != max(sorted(dataDict.keys())):
                x_avg = (dataDict[key][0] + dataDict[key+1][0])/2
                y_avg = (dataDict[key][1] + dataDict[key+1][1])/2
                stagDict[dataDict[key][2]] = x_avg, y_avg
        maxPressure = max(stagDict.keys())     #maximum pressure
        return stagDict[maxPressure], maxPressure 

    def __str__(self):
        """This function prints the angles of attack, lift coefficients, and stagnation points/pressure measurements in the desired output format"""
        str = ''
        k=0
        fileList = ["alpha-3.0.dat", "alpha0.0.dat", "alpha+3.0.dat", "alpha+6.0.dat", "alpha+9.0.dat"]
        angleList = ["-3.00", "0.00", "+3.00", "+6.00", "+9.00"]
        #Calls the functions in the required order, for each angle of attack, and appends the printed output to a string which is finally returned after the loop terminates
        for file in fileList:
            dataDict = self.read_files("xy.dat", file)
            chordLength = self.chord_length(dataDict)
            lift_coeff = self.lift_coeff(dataDict, float(angleList[k]), chordLength)
            stagPoints = self.stagnation_points(dataDict)
            a = stagPoints[0][0]
            b = stagPoints[0][1]
            c = angleList[k]
            #Output formatting
            if k==0:
                str += "{}  {:.4f}  ( {:.4f},  {:.4f})  {:.4f}\n".format(c, lift_coeff, a, b, stagPoints[1])
            elif k==1:     
                str += " {}   {:.4f}  ( {:.4f},  {:.4f})  {:.4f}\n".format(c, abs(lift_coeff), a, b, stagPoints[1])
            else:
                str += " {}   {:.4f}  ( {:.4f}, {:.4f})  {:.4f}\n".format(c[1:], lift_coeff, a, b, stagPoints[1])
            k += 1
        s1 = "alpha     cl          stagnation pt\n"
        s2 = "-----  -------  --------------------------\n"
        return s1+s2+str

airfoil = Airfoil('naca0012/')
#print(airfoil)
a = airfoil.read_files("xy.dat", "alpha+3.0.dat")

length = airfoil.chord_length(a)
#print(length)
#print(airfoil.lift_coeff(a, -3, length))
#print(airfoil.stagnation_points(a)[0
