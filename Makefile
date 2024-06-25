# Compiler name
CXX = g++

# Compilation flags
CXXFLAGS = -Wall -g -std=c++20 -O3

# Source files
SOURCES = main.cpp GeneticAlgorithm.cpp Individual.cpp

# Header files
HEADERS = GeneticAlgorithm.h Individual.h

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Final executable file
EXECUTABLE = GeneticSolver

# Default rule
all: $(EXECUTABLE)

# Rule for creating the executable
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule for creating object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule for cleaning the project
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# Rule to run
run: $(EXECUTABLE)
	./$(EXECUTABLE)
