import math
import sys
import os

class Airfoil:
    def __init__(self, inputdir):
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
            raise RuntimeError("+6 degree angle of attack file does no exist")
            
        if os.path.exists(inputdir+"alpha+9.0.dat") == False:
            raise RuntimeError("+9 degree angle of attack file does no exist")    
        else:
            self.inputdir = str(inputdir)

    def read_em_files(self):
    #one dictionary of the x, y coords
    #one of the pressure measurments for each angle
        dict = {}
        coordFile = open(self.inputdir+"xy.dat", "r")
        MinusThree = open(self.inputdir+"alpha-3.0.dat", "r")
        Zero = open(self.inputdir+"alpha0.0.dat", "r")
        PlusThree = open(self.inputdir+"alpha+3.0.dat", "r")
        PlusSix = open(self.inputdir+"alpha+6.0.dat", "r")
        PlusNine = open(self.inputdir+"alpha+9.0.dat", "r")
    #initialize dictionaries
        lineNuma = 1
        next(coordFile)
        for line in coordFile:
            line = line.split()
            if not line[0] or not line[1]:
                raise RuntimeError("Line has missing x or y coordinate")
            else:
                dict[lineNuma] = [float(line[0]), float(line[1])]
                lineNuma += 1
        #print(dict)
        lineNumb = 1
        next(MinusThree)
        for line in MinusThree:
            line = line.split()
            if not line:
                raise RuntimeError("Line has missing pressure coefficient")
            else:
                dict[lineNumb].append(float(line[0]))
                lineNumb += 1
        if lineNumb != lineNuma + 1
            raise RuntimeError("Missing pressure coefficient for one more panels"
        return dict

    def chord_length(self, dict):
        chordDict = {}
        for key in dict:
            chordDict[dict[key][0]] = dict[key][1]
        trailCoordx = max(chordDict.keys())
        trailCoordy = abs(chordDict[trailCoordx])
        leadCoordx = min(chordDict.keys())
        leadCoordy = abs(chordDict[leadCoordx])
        lengthx = (trailCoordx - leadCoordx)**2
        lengthy = (trailCoordy - leadCoordy)**2
        length = (lengthx + lengthy)**.5
        return length
        
    def lift_coeff(self, dict, alpha, chordLength):
        #append deltay, deltax to dict
        #chordLength = self.chor
        #nextKey = 2
        #dict.pop(max(sorted(dict.keys())))
        for key in sorted(dict.keys()):
            if key != max(sorted(dict.keys())):
                currentx = dict[key][0]
                nextx = dict[key+1][0]
                currenty = dict[key][1]
                nexty = dict[key+1][1]
                dict[key].append(nextx - currentx)
                dict[key].append(nexty - currenty)
#        print(dict)
        c_x = 0
        c_y = 0
        x = 0
        y=0
        for key in sorted(dict.keys()):
            if key != max(sorted(dict.keys())):
                x_contrib = -dict[key][2]*dict[key][4]/chordLength
                y_contrib = dict[key][2]*dict[key][3]/chordLength
                c_x += x_contrib
                c_y += y_contrib
        alpha = math.radians(alpha)
        lift_coeff = c_y*math.cos(alpha) + c_x*math.sin(alpha)
        return(lift_coeff)
        
    def stagnation_points(self, dict):
        stagDict = {}
        for key in dict.keys():
            if key != max(sorted(dict.keys())):
                x_avg = (dict[key][0] + dict[key+1][0])/2
                y_avg = (dict[key][1] + dict[key+1][1])/2
                stagDict[dict[key][2]] = x_avg, y_avg
        maxPressure = max(stagDict.keys())
        return stagDict[maxPressure], maxPressure 

    def __str__(self):
        f = open(self.inputdir+"xy.dat", "r")
        return f.read()

airfoil = Airfoil('naca0012/')
a = airfoil.read_em_files()
#length = airfoil.chord_length(a)
#print(airfoil.lift_coeff(a, -3, length))
#print(airfoil.stagnation_points(a))
