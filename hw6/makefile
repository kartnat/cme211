CXX := g++ 

CXXFLAGS := -O3 -std=c++11 -Wall -Wextra -Wconversion -pedantic 

main: main.cpp image.cpp image.hpp hw6.cpp hw6.hpp 
	$(CXX) $(CXXFLAGS) -o main main.cpp image.cpp hw6.cpp -ljpeg
      
.PHONY: clean
clean:
	$(RM) main
