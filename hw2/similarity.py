import random
import sys
import time

if __name__ == "__main__":
    if len(sys.argv) < 3:
        # no argumemnts, print usage message
        print("Usage:")
        print(" $ python3 similarity.py <data_file> <output_file> [user_thresh (default = 5)]")
        sys.exit()

#Save arguments
data_file = str(sys.argv[1])
output_file = str(sys.argv[2])
if len(sys.argv) == 4:
    user_thresh = int(sys.argv[3])
else: user_thresh = 5


def create_data_dict(data_file):
    """ This function takes a raw data text file as input and returns a dictionary data structure, along with print parameters number of lines read and number of unique user ids"""
    data_file = open(data_file, "r")    
    data = {}
    userSet = set()
    lineNum = 0
    #Reading in data into a dictionary
    for line in data_file:
        curLine = line.split()   #Splitting line into strings
    #Initialize a new entry in the dictionary if movie is new and we haven't reached the end of the file, otherwise append a value entry (which is also a dictionary) for the existing movie
        if curLine != [] and int(curLine[1]) not in data:
            data[int(curLine[1])] = {curLine[0]: int(curLine[2])}
            userSet.add(curLine[0])
            lineNum += 1
        elif curLine != []:
            data[int(curLine[1])][curLine[0]] = int(curLine[2])
            userSet.add(curLine[0])
            lineNum += 1
    return data, lineNum, len(list(data.keys())), len(userSet)


def similarity_computation(movieDicta, movieDictb):
    """ This function takes two movie ids and returns the cosine similarity if it exists and there are at least a specificed (user_thresh) number of users of both movies, which here is at least five."""
    #Computes the average rating for both movies
    avgRatingMovie_a = sum(movieDicta.values())/len(movieDicta.values())
    avgRatingMovie_b = sum(movieDictb.values())/len(movieDictb.values())
    commonUsers=0
    numSum = 0
    denomSumMovie_a = 0
    denomSumMovie_b = 0
    #Computing the numerator and denominator of the cosine similarity formula
    for key in movieDictb.keys() & movieDicta.keys():
        commonUsers += 1
        numSum += (movieDicta[key] - avgRatingMovie_a)*(movieDictb[key] - avgRatingMovie_b)       
        denomSumMovie_a += (movieDicta[key] - avgRatingMovie_a)**2
        denomSumMovie_b += (movieDictb[key] - avgRatingMovie_b)**2
    denom = (denomSumMovie_a*denomSumMovie_b)**0.5
    #If the similarity ratio exists and there are enough common viewers, returns the desired value
    if denom == 0 or commonUsers < user_thresh:
        return 'Not Similar', commonUsers
    else:
        cos_sim = numSum/denom
        return cos_sim, commonUsers


#print(similarity_computation(movieLens, '88', '22'))
#print(similarity_computation(movieLens, '1', '22'))
#print(similarity_computation(movieLens, '88', '1'))
def write_similarities(movieDict, output_file):
    """ This function takes every pair of movies, stores the calculated similarity ratios from the above function, finds the maximum and writes the output to output_file"""
    f = open(output_file, "w")
    movieList = list(movieDict.keys())
    movieList.sort()    #Produces sorted list of movie ids
    #Calls the computation function for every pair and writes the desired parameters to the output file
    for movie_a in movieList:   
        maxSim = -2
        for movie_b in movieList:
            similarity, userNum = similarity_computation(movieDict[movie_a], movieDict[movie_b])
            #Updating the maximum ratio, corresponding movie_id, and number of common users
            if movie_a != movie_b and similarity != 'Not Similar' and similarity > maxSim:
                maxSim = similarity
                mostSimMovie = movie_b
                mostUserNum = userNum
        #Writing output to file in the desired format
        f.write("{} ".format(movie_a))
        if maxSim == -2:
            f.write("\n")
        else:
            f.write("({},{:.2f},{})\n".format(mostSimMovie,maxSim,mostUserNum))
    f.close()

#begin timer
startTime = time.time()

#function calls
movieLens, numLines, numMovies, numUsers = create_data_dict(data_file)
write_similarities(movieLens, output_file)

#end timer
endTime = time.time()
elapsedTime = endTime - startTime

print("Input MovieLens file: {}".format(data_file))
print("Output file for similarity data: {}".format(output_file))
print("Minimum number of common users: {}".format(user_thresh))
print("Read {} lines with total of {} movies and {} users".format(numLines, numMovies, numUsers))
print("Computed similarities in {} seconds".format(elapsedTime))
