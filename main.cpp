#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include "GeneticAlgorithm.h"
#define ITERATIONS 5
const static vector<vector<int>> sudoku3x3_wiki_solved {
    {5, 3, 4, 6, 7, 8, 9, 1, 2},
    {6, 7, 2, 1, 9, 5, 3, 4, 8},
    {1, 9, 8, 3, 4, 2, 5, 6, 7},
    {8, 5, 9, 7, 6, 1, 4, 2, 3},
    {4, 2, 6, 8, 5, 3, 7, 9, 1},
    {7, 1, 3, 9, 2, 4, 8, 5, 6},
    {9, 6, 1, 5, 3, 7, 2, 8, 4},
    {2, 8, 7, 4, 1, 9, 6, 3, 5},
    {3, 4, 5, 2, 8, 6, 1, 7, 9}
};

const static vector<vector<int>> sudoku3x3_wiki_10 {
    {0, 3, 4, 6, 7, 8, 9, 1, 2},
    {6, 7, 2, 0, 9, 5, 3, 0, 8},
    {1, 9, 8, 3, 4, 2, 5, 6, 0},
    {8, 5, 9, 0, 6, 1, 4, 2, 3},
    {4, 2, 6, 8, 5, 3, 7, 9, 1},
    {7, 1, 3, 9, 2, 4, 0, 5, 0},
    {0, 6, 1, 5, 3, 7, 2, 8, 4},
    {2, 8, 7, 4, 0, 9, 6, 3, 5},
    {3, 4, 5, 2, 8, 6, 1, 0, 9}
};

const static vector<vector<int>> sudoku3x3_wiki_15 {
    {0, 3, 4, 0, 7, 8, 9, 1, 2},
    {6, 7, 2, 0, 9, 5, 3, 0, 8},
    {1, 9, 8, 3, 4, 2, 5, 6, 0},
    {8, 5, 9, 0, 6, 1, 4, 0, 3},
    {4, 2, 6, 8, 5, 3, 7, 9, 1},
    {7, 1, 3, 9, 2, 4, 0, 5, 0},
    {0, 6, 1, 5, 3, 7, 2, 8, 4},
    {2, 0, 7, 4, 0, 9, 0, 3, 5},
    {0, 4, 5, 2, 8, 6, 1, 0, 9}
};

const static vector<vector<int>> sudoku3x3_wiki_25 {
    {0, 3, 4, 0, 0, 8, 9, 1, 0},
    {6, 7, 2, 0, 9, 5, 3, 0, 8},
    {1, 9, 8, 0, 0, 2, 0, 6, 0},
    {8, 5, 9, 0, 6, 1, 4, 0, 3},
    {4, 0, 6, 0, 5, 3, 7, 0, 1},
    {7, 1, 3, 9, 2, 4, 0, 5, 0},
    {0, 6, 1, 0, 3, 7, 2, 8, 4},
    {2, 0, 7, 4, 0, 9, 0, 3, 5},
    {0, 4, 0, 2, 8, 6, 1, 0, 9}
};

const static vector<vector<int>> sudoku3x3_wiki_35 {
    {0, 3, 4, 0, 0, 8, 9, 0, 0},
    {6, 0, 2, 0, 9, 5, 0, 0, 8},
    {1, 0, 8, 0, 0, 0, 0, 6, 0},
    {8, 5, 9, 0, 6, 1, 4, 0, 3},
    {0, 0, 6, 0, 5, 3, 7, 0, 1},
    {7, 0, 3, 9, 2, 4, 0, 5, 0},
    {0, 0, 1, 0, 3, 7, 2, 0, 4},
    {2, 0, 7, 4, 0, 9, 0, 3, 5},
    {0, 4, 0, 0, 8, 6, 1, 0, 9}
};

const static vector<vector<int>> sudoku3x3_43 {
    {0, 8, 0, 0, 0, 0, 0, 9, 0},
    {0, 0, 7, 5, 0, 2, 8, 0, 0},
    {6, 0, 0, 8, 0, 7, 0, 0, 5},
    {3, 7, 0, 0, 8, 0, 0, 5, 1},
    {2, 0, 0, 0, 0, 0, 0, 0, 8},
    {9, 5, 0, 0, 4, 0, 0, 3, 2},
    {8, 0, 0, 1, 0, 4, 0, 0, 9},
    {0, 0, 1, 9, 0, 3, 6, 0, 0},
    {0, 4, 0, 0, 0, 0, 0, 2, 0}
};

const static vector<vector<int>> sudoku3x3_49 {
    {8, 0, 2, 0, 0, 3, 5, 1, 0},
    {0, 6, 0, 0, 9, 1, 0, 0, 3},
    {7, 0, 1, 0, 0, 0, 8, 9, 4},
    {6, 0, 8, 0, 0, 4, 0, 2, 1},
    {0, 0, 0, 2, 5, 8, 0, 6, 0},
    {9, 2, 0, 3, 1, 0, 4, 0, 0},
    {0, 0, 0, 4, 0, 2, 7, 8, 0},
    {0, 0, 5, 0, 8, 9, 0, 0, 0},
    {2, 0, 0, 0, 0, 7, 1, 0, 0}
};

const static vector<vector<int>> sudoku3x3_57 {
    {0, 0, 6, 0, 0, 0, 0, 0, 0},
    {0, 8, 0, 0, 5, 4, 2, 0, 0},
    {0, 4, 0, 0, 9, 0, 0, 7, 0},
    {0, 0, 7, 9, 0, 0, 3, 0, 0},
    {0, 0, 0, 0, 8, 0, 4, 0, 0},
    {6, 0, 0, 0, 0, 0, 1, 0, 0},
    {2, 0, 3, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 5, 0, 0, 0, 4, 0},
    {0, 0, 8, 3, 0, 0, 5, 0, 2}
};

const static vector<vector<int>> sudoku3x3_59 {
    {0, 0, 2, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 1, 0, 0, 0, 6},
    {0, 4, 0, 0, 2, 0, 0, 3, 0},
    {1, 0, 0, 0, 0, 3, 0, 0, 9},
    {0, 0, 5, 0, 9, 0, 4, 0, 0},
    {2, 0, 0, 6, 0, 0, 0, 0, 8},
    {0, 9, 0, 0, 7, 0, 0, 4, 0},
    {7, 0, 0, 0, 8, 0, 5, 0, 0},
    {0, 0, 0, 0, 0, 0, 3, 0, 0}
};

const static vector<vector<int>> sudoku4x4 {
    {0, 0, 0, 0, 0, 5, 0, 0, 11, 9, 15, 14, 0, 0, 7, 3},
    {3, 7, 14, 5, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0},
    {0, 0, 1, 11, 3, 0, 0, 0, 0, 0, 5, 0, 0, 10, 0, 13},
    {0, 0, 0, 15, 0, 10, 7, 0, 1, 0, 0, 0, 8, 6, 0, 16},
    {9, 12, 0, 0, 0, 0, 14, 0, 5, 7, 0, 0, 0, 0, 0, 11},
    {0, 11, 0, 6, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0},
    {13, 16, 0, 3, 10, 0, 0, 8, 15, 0, 14, 0, 2, 0, 0, 0},
    {1, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 8, 10},
    {0, 3, 0, 12, 8, 6, 0, 0, 0, 0, 0, 13, 4, 5, 0, 0},
    {0, 0, 4, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0},
    {14, 10, 13, 0, 0, 0, 5, 9, 0, 0, 0, 4, 0, 0, 0, 7},
    {0, 8, 9, 0, 13, 4, 0, 10, 16, 0, 6, 0, 0, 15, 14, 1},
    {8, 0, 0, 0, 16, 0, 9, 1, 4, 5, 0, 0, 0, 0, 0, 0},
    {0, 0, 11, 0, 12, 2, 0, 0, 6, 0, 0, 8, 15, 0, 0, 0},
    {0, 6, 0, 0, 11, 7, 3, 13, 14, 0, 0, 1, 12, 0, 0, 0},
    {12, 0, 0, 0, 0, 0, 0, 4, 0, 11, 9, 0, 0, 0, 1, 0}
};

/**
 * Entry point for the Genetic Algorithm program.
 * This function parses command-line arguments to configure and start the genetic algorithm simulation.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 *        - argv[1]: The size of the n-queens board (n) and also the size of one side of the sudoku block.
 *        - argv[2]: The size of generations (must be even and greater than 2).
 *        - argv[3]: The mutation rate (must be between 0 and 1).
 * @return Returns 0 if the program terminates successfully, or 1 if an error occurs.
 */
//----------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    // checking arguments
    if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " <n> <size_of_generations> <mutation_rate>" << std::endl;
        exit(1);
    }

    try {
        int n = std::stoi(argv[1]);
        if (n <= 0) {
            std::cerr << "The number of queens must be greater than 0" << std::endl;
            exit(1);
        }

        int size_of_generations = std::stoi(argv[2]);
        if (size_of_generations <= 1 || size_of_generations % 2) {
            std::cerr << "The generation size must be even and at greater than 2" << std::endl;
            exit(1);
        }

        double mutation_rate = std::stod(argv[3]);
        if (mutation_rate < 0 || mutation_rate > 1) {
            std::cerr << "The mutation rate must be in [0, 1]" << std::endl;
            exit(1);
        }

        /*for (int i = 0; i < ITERATIONS; i++) {
            GeneticAlgorithm ga(size_of_generations, n, mutation_rate, sudoku3x3_57);
            ga.run();
        }*/
        GeneticAlgorithm ga(size_of_generations, n, mutation_rate, sudoku3x3_43);
        ga.run();

    } catch (std::invalid_argument const& ex) {
        std::cerr << "Invalid number: " << ex.what() << std::endl;
        exit(1);
    } catch (std::out_of_range const& ex) {
        std::cerr << "Number out of range: " << ex.what() << std::endl;
        exit(1);
    }

    return 0;
}
//----------------------------------------------------------------------------------------------------------------------