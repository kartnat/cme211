import math
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
import os
import scipy.sparse.linalg
import scipy.sparse 
import sys
import warnings

class Truss:
    """Class Truss: This class is designed to take in two files, a (i) joint
    file that lists the number of joints, their x/y coordinates, any external
    forces, and an indicator for whether a hinge is fixed or not, as well as
    (ii) a beams file that lists the number of beams and the joints they
    connect. It then uses the method of joints to attempt to solve for the
    individual beam forces under the assumption that the truss is in static
    equilibrium, that is the sum of the forces acting at each joint is 0."""
    
    def __init__(self, joint, beam, plotFile):
        #Files are absolute paths to the file locations
        self.joint = str(joint)
        self.beam = str(beam)
        self.plotFile = str(plotFile)
        
    def PlotGeometry(self, beamDict, jointDict):
	    #Outputs a simple (x, y) plot with connected line segments of the
        #given truss geometry, saved to the location designated by the optional
        #plot file location. The inputs are the joints and beams dictionaries
        #outputted from read files.
        
        #Going through every beam and pair of connected joints, and plots the 
        #segment
        for key in sorted(beamDict.keys()):
            Ja, Jb = beamDict[key][0], beamDict[key][1]
            x1, x2 = jointDict[Ja][0], jointDict[Jb][0]
            y1, y2 = jointDict[Ja][1], jointDict[Jb][1]
            plt.plot(tuple((x1, x2)),tuple((y1,y2)), 'b')
        #Adjusting the axes limits
        xmin = min(v[0] for v in jointDict.values())
        xmax = max(v[0] for v in jointDict.values())
        ymin = min(v[1] for v in jointDict.values())
        ymax = max(v[1] for v in jointDict.values())
        plt.xlim(xmin-0.5, xmax+0.5)
        plt.ylim(ymin-0.5, ymax+0.5)
        #Saving the file to disk
        plt.savefig(self.plotFile)

    def read_files(self):
        """Reads in the joints and beams files and stores them in dictionaries.
        Three dictionaries are outputted, one for the joints information,
        one for the beams, and one which combines the two in a convenient way
        for later use."""
        joint = open(self.joint, "r")
        beam = open(self.beam, "r")
        next(joint)
        next(beam)
        joint_dict = {}
        beam_dict = {}
        #Reads in joint and beams files into dictionaries
        for line in joint:
            line = line.split()
            a = float(line[1])
            b = float(line[2])
            c = float(line[3])
            d = float(line[4])
            e = float(line[5])
            joint_dict[int(line[0])] = [a,b,c,d,e]
        for line in beam:
            line = line.split()
            beam_dict[int(line[0])] = [int(line[1]),int(line[2])]
        
        matrixDict = {}
        newBeamDict = {}
        #Initializes new dictionary with keys the joints and values the
        #beams and associated coordinates
        for jointKey in sorted(joint_dict.keys()):
            matrixDict[jointKey] = {}
            for beamKey in beam_dict.keys():
                if jointKey in beam_dict[beamKey]:
                    Ja = beam_dict[beamKey][0]
                    Jb = beam_dict[beamKey][1]
                    if Ja == jointKey:
                        coord = joint_dict[Jb][0], joint_dict[Jb][1]
                        matrixDict[jointKey][beamKey] = coord
                    else:
                        coord = joint_dict[Ja][0], joint_dict[Ja][1]
                        matrixDict[jointKey][beamKey] = coord
        return beam_dict, joint_dict, matrixDict

    def get_coeff(self, joint, coord, eqn_ind, joint_Dict):
        """Computes the elements of the matrix in CRS format, by
        splitting into cases based on the component the equation i
        s representing (eqn ind parameter), and the relationship between
        the coordinates of the joint this equation corresponds to and the
        other joints connected to it by other beams (encapsulated in the
        parameters joint and coord)."""
        xjointCoord = joint_Dict[joint][0]
        yjointCoord = joint_Dict[joint][1]
        #for x direction equation
        if eqn_ind == 'x' and yjointCoord == coord[1]:
            if xjointCoord > coord[0]:
                return 1
            else:
                return -1
        #For y direction equation
        elif eqn_ind == 'y' and xjointCoord == coord[0]:
            if yjointCoord > coord[1]:
                return 1
            else:
                return -1
        #For either direction, but when the coordinate is connected to the 
        #joint by a diagonal line segment
        else:
            xlength = coord[0] - xjointCoord
            ylength = coord[1] - yjointCoord
            angle = math.atan(ylength/xlength)
            xcomponent = abs(math.cos(angle))
            ycomponent = abs(math.sin(angle))
            if xlength > 0 and eqn_ind == 'x':
                return -xcomponent
            elif xlength < 0 and eqn_ind == 'x':
                return xcomponent
            elif ylength > 0 and eqn_ind == 'y':
                return -ycomponent
            elif ylength < 0 and eqn_ind == 'y':
                return ycomponent
        
    def system_setup(self, matrixDict,beamDict, jointDict):
        """Uses the three aforementioned dictionaries and creates the thre
        e vectors (one for the nonzero elements in row order, one for the column
        indices, and one for the row storage) needed to store the sparse matrix
        in CRS style. The matrix is conceptually one which has two equations
        for each beam, and progresses row-wise in beam order. That is, the first
        two rows of the matrix correspond the x/y equations for beam 1, the second
        two for beam 2, etc. It depends on the get_coeff method to actually
        compute the coefficients."""
        beamNum = len(beamDict.keys())
        V = []
        cols = []
        rows = []
        rows.append(0)
        reactCnt = 0
        rowCnt = 0
        index = 0
        #Goes through every key and appends nonzero coefficients.
        #Also records column index and the count of the nonzero elements in each row,
        #for the rows vector.
        for jointKey in sorted(matrixDict.keys()):
            #x equation loop
            for beamKey in sorted(matrixDict[jointKey].keys()):
                if jointDict[jointKey][0] != matrixDict[jointKey][beamKey][0]:
                    V.append(self.get_coeff(jointKey,matrixDict[jointKey][beamKey], 'x', jointDict))
                    cols.append(beamKey-1)
                    rowCnt += 1
            #If there is no displacement of joint, append an entry for the reaction force
            if jointDict[jointKey][4] == 1:
                V.append(1)
                cols.append(beamNum - 1+2*reactCnt+1)
                rowCnt += 1
            #Append the number of nonzero entries to rows vector
            rows.append(rowCnt)
          
            #y equation loop
            for beamKey in sorted(matrixDict[jointKey].keys()):
                if jointDict[jointKey][1] != matrixDict[jointKey][beamKey][1]:
                    V.append(self.get_coeff(jointKey, matrixDict[jointKey][beamKey], 'y', jointDict))
                    cols.append(beamKey-1)
                    rowCnt += 1
                    
            if jointDict[jointKey][4] == 1:
                V.append(1)
                cols.append(beamNum - 1+2*reactCnt+2)   
                reactCnt += 1
                rowCnt += 1
                   
            rows.append(rowCnt)
        return V, cols, rows
        
    def matrixSolve(self, data, cols, rows, jointDict):
        """This method converts inputted vectors associated with the CRS form into
        csr matrix format and uses the scipy sparse solver to solve for the force
        vector. Errors are raised for singular matrices or over/underdetermined systems."""
        bList = []
        #Convert to sparse matrix format
        finalMatrix = scipy.sparse.csr_matrix((data, cols, rows), shape=(len(rows)-1, len(rows)-1))
        #Constructing the b vector in Ax=b (either 0 or one of F_x, F_y)
        for key in sorted(jointDict.keys()):
            bList.append(-jointDict[key][2])
            bList.append(-jointDict[key][3])
        b = np.array(bList)
        #Catch warnings as exceptions
        warnings.filterwarnings('error')
        try:
            soln = scipy.sparse.linalg.spsolve(finalMatrix.tocsr(), b)
        except RuntimeError:
            raise RuntimeError("Truss geometry not suitable for static equilibrium analysis")
        except:
            raise RuntimeError("Cannot solve the linear system, unstable truss?")
        return soln
        
    def __str__(self):
        """String representation method which nicely prints the beams in
        beam order and the associated forces to console. It will also
        call the PlotGeometry method to save a plot of the truss geometry
        to the designated file location, if provided."""
        s1 = " Beam       Force\n"
        s2 = "-----------------\n"
        s3 = ""
        beam_dict, joint_dict, matrixdict =self.read_files()
        if self.plotFile != 'no plot':
            self.PlotGeometry(beam_dict, joint_dict)
        V, cols, rows = self.system_setup(matrixdict,beam_dict, joint_dict)
        soln = self.matrixSolve(V, cols, rows, joint_dict)
        ctr = 0
        #Going line by line, printing the output to console
        for key in sorted(beam_dict.keys()):
            if soln[ctr] >= 0 and str(soln[ctr])[0] != '-':
                s3 += "    {}       {:.3f}\n".format(key, soln[ctr])
            else:
                s3 += "    {}      {:.3f}\n".format(key, soln[ctr])
            ctr += 1
        return s1+s2+s3


