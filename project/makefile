CXX := g++ -O3 -std=c++11

CXXFLAGS := -Wall -Wextra -Wconversion -pedantic

main: main.cpp heat.cpp heat.hpp sparse.cpp sparse.hpp CGSolver.cpp CGSolver.hpp matvecops.cpp matvecops.hpp COO2CSR.cpp COO2CSR.hpp
	$(CXX) $(CXXFLAGS) -o main main.cpp heat.cpp sparse.cpp CGSolver.cpp matvecops.cpp COO2CSR.cpp
    
.PHONY: clean
clean:
	$(RM) main
