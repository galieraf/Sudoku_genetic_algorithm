//
// Created by rafael on 19.4.24.
//
#include "Individual.h"
#include <random>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <utility>
//----------------------------------------------------------------------------------------------------------------------
Individual::Individual(int n) : blockSize(n)  {
    //random generator
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine engine(seed);

    fields = vector<vector<int>> (blockSize*blockSize, vector<int>(blockSize*blockSize));

    // fill the sudoku
    for (auto& inner_vec : fields) {
        // fill the field
        for (int i = 0; i < blockSize * blockSize; ++i) {
            inner_vec[i] = i + 1;
        }
        // shuffle
        shuffle(inner_vec.begin(), inner_vec.end(), engine);
    }

    fitness = calculateFitness();

}
//----------------------------------------------------------------------------------------------------------------------
Individual::Individual(const vector<vector<int>> &sudoku, const set<pair<int,int>>& fixed) {
    fields = sudoku;
    blockSize = (int)sqrt(sudoku.size());
    fixedPositions = fixed;
    fitness = calculateFitness();
}
//----------------------------------------------------------------------------------------------------------------------
int Individual::calculateFitness() {
    int conflicts = 0;
    int gridSize = blockSize * blockSize;

    // check rows for conflicts
    for (int i = 0; i < gridSize; ++i) {
        vector<bool> check(gridSize + 1, false);
        for (int j = 0; j < gridSize; ++j) {
            // look on blocks by rows
            int value = fields[(i / blockSize) * blockSize + (j / blockSize)][(i % blockSize) * blockSize + (j % blockSize)];
            if (check[value]) {
                ++conflicts;
            } else {
                check[value] = true;
            }
        }
    }

    // check columns for conflicts
    for (int i = 0; i < gridSize; ++i) {
        vector<bool> check(gridSize + 1, false);
        for (int j = 0; j < gridSize; ++j) {
            // look on blocks by columns
            int value = fields[(j / blockSize) * blockSize + (i / blockSize)][(j % blockSize) * blockSize + (i % blockSize)];
            if (check[value]) {
                ++conflicts;
            } else {
                check[value] = true;
            }
        }
    }

    return -conflicts;
}
//----------------------------------------------------------------------------------------------------------------------
Individual::Individual() {

}
//----------------------------------------------------------------------------------------------------------------------
Individual::Individual(int blockSize, const vector<vector<int>> &sudoku) {
    this->blockSize = blockSize;
    int gridSize = blockSize * blockSize;
    if (sudoku.size() != (size_t)gridSize || sudoku[0].size() != (size_t)gridSize) {
        throw invalid_argument("Invalid sudoku size");
    }

    fields.resize(gridSize, vector<int>(gridSize, 0));
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            int blockIndex = (i / blockSize) * blockSize + (j / blockSize);
            int indexInBlock = (i % blockSize) * blockSize + (j % blockSize);
            fields[blockIndex][indexInBlock] = sudoku[i][j];
            if (sudoku[i][j] != 0) {
                fixedPositions.insert( {blockIndex, indexInBlock });
            }
        }
    }

    // Filling empty spaces in blocks with random values taking into account fixed values
    random_device rd;
    mt19937 g(rd());
    for (auto& block : fields) {
        vector<int> missingNumbers;
        set<int> numbersPresent;
        for (int num : block) {
            if (num != 0) numbersPresent.insert(num);
        }
        for (int num = 1; num <= gridSize; ++num) {
            if (numbersPresent.count(num) == 0) {
                missingNumbers.push_back(num);
            }
        }
        shuffle(missingNumbers.begin(), missingNumbers.end(), g);

        for (int& num : block) {
            if (num == 0) {
                num = missingNumbers.back();
                missingNumbers.pop_back();
            }
        }
    }

    fitness = calculateFitness();
}
//----------------------------------------------------------------------------------------------------------------------