//
// Created by rafael on 19.4.24.
//
#ifndef BI_ZUM_GALIERAF_INDIVIDUAL_H
#define BI_ZUM_GALIERAF_INDIVIDUAL_H
#include <vector>
#include <set>

class GeneticAlgorithm;

using namespace std;
/**
 * Represents an individual in the genetic algorithm simulation for solving Sudoku puzzles.
 * An individual is characterized by a Sudoku grid and its fitness score.
 */
class Individual {
  public:
    /**
     * Default constructor
     */
    Individual();

    /**
     * Constructor that initializes an individual with a pre-filled Sudoku grid.
     * @param blockSize The size of the block in the Sudoku grid, where the total grid size is blockSize*blockSize.
     * @param sudoku Pre-filled sudoku grid
     */
    Individual(int blockSize, const vector<vector<int>>& sudoku);

    /**
     * Constructor that initializes an individual with a random Sudoku grid.
     * @param blockSize The size of the block in the Sudoku grid, where the total grid size is blockSize*blockSize.
     */
    Individual(int blockSize);

    /**
     * Constructor that initializes an individual with a given Sudoku grid.
     * @param sudoku A 2D vector representing a Sudoku grid.
     */
    Individual(const vector<vector<int>>& sudoku, const set<pair<int,int>>& fixed);

    /**
     * Calculates and returns the fitness of this individual.
     * Fitness is based on the number of conflicts (repeated numbers) in each row and column of the Sudoku grid.
     * @return The negative count of conflicts, with a higher conflict count resulting in a more negative fitness score.
     */
    int calculateFitness();
  private:
    friend class GeneticAlgorithm;
    vector<vector<int>> fields;                     // 2D vector representing the Sudoku grid.
    int fitness;                                    // Fitness score of the individual.
    int blockSize;                                  // Size of the Sudoku block.
    set<pair<int,int>> fixedPositions;              // Positions of fixed elements
};


#endif //BI_ZUM_GALIERAF_INDIVIDUAL_H
