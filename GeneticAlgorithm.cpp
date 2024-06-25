//
// Created by rafael on 19.4.24.
//
#include "GeneticAlgorithm.h"
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <random>
#include <chrono>
#include <unordered_map>

#define ONE_POINT_CROSSOVER 0
#define TWO_POINT_CROSSOVER 1
#define MAX_ITER_NO_IMPROVEMENT 300

//----------------------------------------------------------------------------------------------------------------------
GeneticAlgorithm::GeneticAlgorithm(int populationSize, int nRoot, double mutationRate) : blockSize(nRoot),
popSize(populationSize), mutRate(mutationRate), rng(chrono::system_clock::now().time_since_epoch().count()) {
    initPopulation();
}
//----------------------------------------------------------------------------------------------------------------------
GeneticAlgorithm::GeneticAlgorithm(int populationSize, int nRoot, double mutationRate, const vector<vector<int>>& initialSudoku) :
        blockSize(nRoot), popSize(populationSize), mutRate(mutationRate),
        rng(chrono::system_clock::now().time_since_epoch().count()) {
    initPopulationWithSudoku(initialSudoku);
    this->initialSudoku = initialSudoku;
}
//----------------------------------------------------------------------------------------------------------------------
void GeneticAlgorithm::run() {
    int generation = 0;
    int restarts = 0;
    int stagnantGenerations = 0;
    double lastBestFitness = numeric_limits<double>::max();

    for(;;generation++) {

        auto bestIndividual = max_element(population.begin(), population.end(),
                                          [](const Individual &a, const Individual &b) {
                                              return a.fitness < b.fitness;
                                          });
        //print board
        printBoard(*bestIndividual, generation, restarts);

        if (bestIndividual->fitness == 0) break;

        // check stagnation
        if (lastBestFitness >= bestIndividual->fitness) {
            ++stagnantGenerations;
        } else {
            lastBestFitness = bestIndividual->fitness;
            stagnantGenerations = 0;
        }

        //selection
        vector<Individual> selected;
        selected.reserve(popSize);
        if (stagnantGenerations >= MAX_ITER_NO_IMPROVEMENT) {
            // random selection to escape local minimum
            selected = restart();
            restarts++;
            stagnantGenerations = 0;

        } else {
            selected = rouletteSelection();
        }

        //crossover
        vector<Individual> children;
        children.reserve(popSize);
        for (int i = 0; i < popSize; i = i+2) {
            auto res = crossover(selected[i], selected[i+1]);
            children.emplace_back(std::move(res.first));
            children.emplace_back(std::move(res.second));
        }

        //mutation
        for (auto& ind : children )
            mutation(ind);

        //new population
        this->population = children;

    }
}
//----------------------------------------------------------------------------------------------------------------------
void GeneticAlgorithm::initPopulation() {
    population.clear();
    for (int i = 0; i < popSize; i++) {
        population.emplace_back(blockSize);
    }
}
//----------------------------------------------------------------------------------------------------------------------
void GeneticAlgorithm::initPopulationWithSudoku(const vector<vector<int>>& sudoku ) {
    population.clear();
    for (int i = 0; i < popSize; i++) {
        population.emplace_back(blockSize, sudoku);
    }
}
//----------------------------------------------------------------------------------------------------------------------
vector<Individual> GeneticAlgorithm::restart() {
    if (!initialSudoku.empty()) {
        initPopulationWithSudoku(initialSudoku);
    }
    else initPopulation();

    return std::move(population);
}
//----------------------------------------------------------------------------------------------------------------------
vector<Individual> GeneticAlgorithm::rouletteSelection() const {
    vector<Individual> selected;
    selected.reserve(population.size());

    // First we find the minimum fitness
    double minFitness = numeric_limits<double>::max();
    for (const auto& ind : population) {
        if (ind.fitness < minFitness) {
            minFitness = ind.fitness;
        }
    }

    // smth like normalisation
    vector<double> adjustedFitnesses;
    adjustedFitnesses.reserve(population.size());
    for (const auto& ind : population) {
        adjustedFitnesses.push_back(ind.fitness - minFitness);
    }

    // total fitness
    double totalAdjustedFitness = 0.0;
    for (auto fitness : adjustedFitnesses) {
        totalAdjustedFitness += fitness;
    }

    // roulette
    uniform_real_distribution<double> distribution(0.0, totalAdjustedFitness);
    for (size_t i = 0; i < population.size(); ++i) {
        double slice = distribution(rng);

        double sum = 0.0;
        for (size_t j = 0; j < population.size(); ++j) {
            sum += adjustedFitnesses[j];
            if (sum >= slice) {
                selected.push_back(population[j]);
                break;
            }
        }
    }

    return selected;
}
//----------------------------------------------------------------------------------------------------------------------
void GeneticAlgorithm::mutation(Individual &i) const {
    // random generator init
    int gridSize = blockSize * blockSize;
    uniform_real_distribution<double> probability(0.0, 1.0);
    uniform_int_distribution<int> blockIndex(0, (int)i.fields.size() - 1);
    uniform_int_distribution<int> elementIndex(0, gridSize - 1);

    // go throw all blocks
    int j = 0;
    for (auto &block : i.fields) {
        // will this block mutate
        if (probability(rng) < mutRate) {
            // swap diff indexes
            int index1 = elementIndex(rng);
            int index2 = elementIndex(rng);
            auto it1 = i.fixedPositions.find({j, index1});
            auto it2 = i.fixedPositions.find({j, index2});
            if (it1 == i.fixedPositions.end() && it2 == i.fixedPositions.end()) {
                swap(block[index1], block[index2]);
            }
        }
        j++;
    }
}
//----------------------------------------------------------------------------------------------------------------------
pair<Individual, Individual>
GeneticAlgorithm::crossover(const Individual &parent1, const Individual &parent2) const {

    vector<vector<int>> child1;
    vector<vector<int>> child2;

    uniform_int_distribution<> distrib(0, 1);
    int crossoverIndex = distrib(rng);


    pair<Individual, Individual> res;
    if (crossoverIndex == ONE_POINT_CROSSOVER) {
        res = crossoverOnePoint(parent1, parent2);
    }
    else if (crossoverIndex == TWO_POINT_CROSSOVER) {
        res = crossoverTwoPoint(parent1, parent2);
    }

    return {std::move(res.first), std::move(res.second)};
}
//----------------------------------------------------------------------------------------------------------------------
pair<Individual, Individual>
GeneticAlgorithm::crossoverOnePoint(const Individual &parent1, const Individual &parent2) const {
    int numBlocks = blockSize * blockSize;
    uniform_int_distribution<> distrib(1, numBlocks - 1);  // avoid crossover at ends
    int crossoverPoint = distrib(rng);

    vector<vector<int>> child1Fields = parent1.fields;
    vector<vector<int>> child2Fields = parent2.fields;

    for (int i = crossoverPoint; i < numBlocks; ++i) {
        swap(child1Fields[i], child2Fields[i]);
    }

    return {{child1Fields, parent1.fixedPositions}, {child2Fields, parent1.fixedPositions}};
}
//----------------------------------------------------------------------------------------------------------------------
pair<Individual, Individual>
GeneticAlgorithm::crossoverTwoPoint(const Individual &parent1, const Individual &parent2) const {
    int numBlocks = blockSize * blockSize;
    uniform_int_distribution<> distrib(1, numBlocks - 2);  // ensure space for two points
    int point1 = distrib(rng);
    int point2 = distrib(rng);

    if (point1 > point2) swap(point1, point2);

    vector<vector<int>> child1Fields = parent1.fields;
    vector<vector<int>> child2Fields = parent2.fields;

    for (int i = point1; i <= point2; ++i) {
        swap(child1Fields[i], child2Fields[i]);
    }
    return {{child1Fields, parent1.fixedPositions}, {child2Fields, parent1.fixedPositions}};
}
//----------------------------------------------------------------------------------------------------------------------
void GeneticAlgorithm::printBoard(const Individual &indv, int generation, int restarts) const {
    if (system("clear") != 0) {
        cerr << "Failed to clear the screen.\n";
    }
    int boardSize = blockSize * blockSize;
    int cellWidth = (int)to_string(boardSize).length();

    // printing the top frame
    cout << "+";
    for (int i = 0; i < boardSize; ++i) {
        cout << string(cellWidth + 1, '-') << (i % blockSize == blockSize - 1 ? "-+" : "");
    }
    cout << "\n";

    for (int row = 0; row < boardSize; ++row) {
        for (int col = 0; col < boardSize; ++col) {
            // left frame
            if (col % blockSize == 0) cout << "| ";
            // get value
            int blockIndex = (row / blockSize) * blockSize + (col / blockSize);
            int indexInBlock = (row % blockSize) * blockSize + (col % blockSize);
            int value = indv.fields[blockIndex][indexInBlock];
            cout << setw(cellWidth) << value << " ";
        }
        // right frame
        cout << "|\n";

        // horizontal frame after each block row
        if ((row + 1) % blockSize == 0 && row + 1 < boardSize) {
            cout << "+";
            for (int i = 0; i < boardSize; ++i) {
                cout << string(cellWidth + 1, '-') << (i % blockSize == blockSize - 1 ? "-+" : "");
            }
            cout << "\n";
        }
    }

    // printing the bottom frame
    cout << "+";
    for (int i = 0; i < boardSize; ++i) {
        cout << string(cellWidth + 1, '-') << (i % blockSize == blockSize - 1 ? "-+" : "");
    }
    cout << "\n"; // closing the bottom frame

    cout << "Best fitness = " << indv.fitness << endl;
    cout << "Generation = " << generation << endl;
    cout << "Restarts = " << restarts << endl;
}
//----------------------------------------------------------------------------------------------------------------------
