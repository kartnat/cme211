CXX := g++

CXXFLAGS := -Wall -Wextra -Wconversion 

main: main.cpp CGSolver.cpp CGSolver.hpp matvecops.cpp matvecops.hpp COO2CSR.cpp COO2CSR.hpp
	$(CXX) $(CXXFLAGS) -o main main.cpp CGSolver.cpp matvecops.cpp COO2CSR.cpp
    
.PHONY: clean
clean:
	$(RM) main
