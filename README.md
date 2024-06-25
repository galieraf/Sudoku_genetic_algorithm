# Sudoku Solver Using Genetic Algorithm

This project implements a genetic algorithm to solve Sudoku puzzles. The genetic algorithm mimics the process of natural selection, evolving a population of potential solutions over generations.

## Project Files

- **GeneticAlgorithm.cpp**: Implementation of the genetic algorithm.
- **GeneticAlgorithm.h**: Header file for the genetic algorithm.
- **Individual.cpp**: Implementation of the individual class representing a Sudoku solution.
- **Individual.h**: Header file for the individual class.
- **main.cpp**: Main program file to run the genetic algorithm.
- **Makefile**: Makefile to compile the project.
- **LICENSE**: License information for the project.
- **report.pdf**: Detailed report on the project and its implementation.

## Features

- Solves Sudoku puzzles using a genetic algorithm.
- Implements single-point and two-point crossover mechanisms.
- Utilizes roulette wheel selection for evolving solutions.
- Applies mutation carefully to maintain Sudoku rules.
- Includes a restart mechanism to avoid convergence on local optima.

## Installation

1. **Clone the repository**:
    ```sh
    git clone https://github.com/yourusername/sudoku-genetic-algorithm.git
    cd sudoku-genetic-algorithm
    ```

2. **Compile the project**:
    ```sh
    make
    ```
   
## Usage

You can modify the puzzle and parameters directly in the `main.cpp` file at line 171:
```cpp
GeneticAlgorithm ga(size_of_generations, n, mutation_rate, sudoku3x3_43);
```
To solve a Sudoku puzzle, run the compiled executable. 
```sh
./GeneticSolver <size_of_sudoku> <size_of_generations> <mutation_rate>
```

## Parameters

1. The sudoku size must be greater than 0
2. The generation size must be even and at greater than 2
3. The mutation rate must be in [0, 1]

## Example
Here is a basic example of how to set up and solve a Sudoku puzzle using the provided code:

Open main.cpp and set your Sudoku puzzle in the appropriate section.
Compile and run the program:
```sh
./GeneticSolver 3 100 0.1
```
The program will output the solution to the console.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Report

For a detailed explanation of the algorithm and its performance, refer to the report.pdf file.

## Author

Rafael Galiev

[galieraf](https://github.com/galieraf)
