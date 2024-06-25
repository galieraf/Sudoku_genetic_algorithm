//
// Created by rafael on 19.4.24.
//
#ifndef BI_ZUM_GALIERAF_GENETICALGORITHM_H
#define BI_ZUM_GALIERAF_GENETICALGORITHM_H

#include "Individual.h"
#include <vector>
#include <random>

using namespace std;
/**
 * GeneticAlgorithm class manages the genetic algorithm operations for solving problems.
 */
class GeneticAlgorithm {
  public:

    /**
     * Constructor for GeneticAlgorithm.
     * @param populationSize Size of the population.
     * @param nRoot Base size of the Sudoku block (e.g., 3 for 9x9 Sudoku).
     * @param mutationRate Probability of mutation per individual.
     */
    GeneticAlgorithm(int populationSize, int nRoot, double mutationRate);

    /**
     *
     * @param populationSize Size of the population.
     * @param nRoot Base size of the Sudoku block (e.g., 3 for 9x9 Sudoku).
     * @param mutationRate Probability of mutation per individual.
     * @param initialPopulation Pre-filled sudoku grid
     */
    GeneticAlgorithm(int populationSize, int nRoot, double mutationRate, const vector<vector<int>>& initialSudoku);

    /**
     * Main method to run the genetic algorithm.
     */
    void run();

  private:
    /**
     * Initializes the population with randomly generated individuals.
     */
    void initPopulation();

    /**
     * Initializes a population based on a pre-filled sudoku
     * @param sudoku Pre-filled sudoku grid
     */
    void initPopulationWithSudoku(const vector<vector<int>>& sudoku);

    /**
     * Performs roulette wheel selection to select individuals based on their fitness scores.
     * @return A vector of selected individuals.
     */
    vector<Individual> rouletteSelection() const;

    /**
     * Create randomly new population
     * @return A vector of new individuals.
     */
    vector<Individual> restart();

    /**
     * Applies mutation to an individual based on the mutation rate.
     * @param i Reference to the individual to mutate.
     */
    void mutation(Individual& i) const;

    /**
     * Performs crossover between two parents to generate two offspring.
     * @param parent1 The first parent.
     * @param parent2 The second parent.
     * @return A pair of offspring resulting from the crossover.
     */
    pair<Individual, Individual> crossover(const Individual& parent1, const Individual& parent2) const;

    /**
     *
     * @param parent1 The first parent.
     * @param parent2 The second parent.
     * @return A pair of offspring resulting from the crossover.
     */
    pair<Individual, Individual> crossoverOnePoint(const Individual &parent1, const Individual &parent2) const;
    /**
     *
     * @param parent1 The first parent.
     * @param parent2 The second parent.
     * @return A pair of offspring resulting from the crossover.
     */
    pair<Individual, Individual> crossoverTwoPoint(const Individual &parent1, const Individual &parent2) const;

    /**
     * Displays the sudoku board of an individual along with its fitness and generation number.
     * @param i The individual whose board is to be displayed.
     * @param generation The current generation number.
     * @param restarts The number of algorithm restarts.
     */
    void printBoard(const Individual& i, int generation, int restarts) const;

    int blockSize;                              // The size of one block in the sudoku puzzle.
    int popSize;                                // The size of the population.
    double mutRate;                             // The mutation rate.
    vector<Individual> population;              // The current population of individuals.
    vector<vector<int>> initialSudoku;          // The initial pre-filled sudoku grid
    mutable default_random_engine rng;          // Random number generator for stochastic processes.
};


#endif //BI_ZUM_GALIERAF_GENETICALGORITHM_H
