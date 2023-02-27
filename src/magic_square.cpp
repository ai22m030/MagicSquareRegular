//
// Created by Adnan Vatric on 18.02.23.
//

#include "magic_square.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

// Define the random number generator
std::mt19937 rng(std::random_device{}());

/**
    * Print content of square. Is used for debugging purposes.
    */
void MagicSquare::print() {
    for (auto &value: this->values) {
        for (int j: value) {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

/**
 * Generate a random magic square using constants.
 *
 * @return
 */
MagicSquare randomSquare() {
    MagicSquare square{};
    std::vector<int> values(SIZE * SIZE);
    int *data = &square.values[0][0];
    std::iota(data, data + SIZE * SIZE, 1);
    std::shuffle(data, data + SIZE * SIZE, rng);

    square.fitness = 0;
    return square;
}

/**
 * Calculate fitness for all rows of a square.
 *
 * @param square
 * @param magicSum
 * @return
 */
int fitnessRows(MagicSquare &square, int magicSum) {
    int fitness = 0;

    for (auto &rows: square.values) {
        int sum = 0;

        for (int cols: rows) {
            sum += cols;
        }

        fitness += std::abs(sum - magicSum);
    }

    return fitness;
}

/**
 * Calculate fitness for all columns of a square.
 *
 * @param candidate
 * @param magicSum
 * @return
 */
int fitnessColumns(MagicSquare &candidate, int magicSum) {
    int fitness = 0;

    for (int cols = 0; cols < SIZE; cols++) {
        int sum = 0;

        for (auto &value: candidate.values) {
            sum += value[cols];
        }

        fitness += std::abs(sum - magicSum);
    }

    return fitness;
}

/**
 * Calculate fitness of diagonal (left top to right bottom)
 *
 * @param candidate
 * @param magicSum
 * @return
 */
int fitnessDiagonal1(MagicSquare &candidate, int magicSum) {
    int sum = 0;

    for (int i = 0; i < SIZE; i++) {
        sum += candidate.values[i][i];
    }

    return std::abs(sum - magicSum);
}

/**
 * Calculate fitness of diagonal (left bottom to right top)
 *
 * @param candidate
 * @param magicSum
 * @return
 */
int fitnessDiagonal2(MagicSquare &candidate, int magicSum) {
    int sum = 0;

    for (int i = SIZE - 1; i >= 0; i--) {
        sum += candidate.values[SIZE - (i + 1)][i];
    }

    return std::abs(sum - magicSum);
}

/**
 * Evaluate the fitness of a square solution.
 *
 * @param square
 * @param magicSum
 */
void evaluateFitness(MagicSquare &square, int magicSum) {
    square.fitness = 0;

    square.fitness += fitnessRows(square, magicSum);
    square.fitness += fitnessColumns(square, magicSum);
    square.fitness += fitnessDiagonal1(square, magicSum);
    square.fitness += fitnessDiagonal2(square, magicSum);
}

// Define a function to perform selection
/**
 * Select the best half of the population.
 *
 * @param population
 * @return
 */
std::vector<MagicSquare> parentSelection(std::vector<MagicSquare> &population) {
    std::vector<MagicSquare> selected;

    // Sort the population by fitness
    std::sort(population.begin(),
              population.end(),[](const MagicSquare &a, const MagicSquare &b) {
                return a.fitness < b.fitness;
            });

    for (int i = 0; i < POPULATION_SIZE / 2; i++) {
        selected.push_back(population[i]);
    }

    return selected;
}

/**
 * Combine selected square with random square from population.
 *
 * @param selected
 * @param population
 * @return
 */
std::vector<MagicSquare> crossover(const std::vector<MagicSquare> &selected, std::vector<MagicSquare> &population) {
    int magicSum = MAGIC_SUM(SIZE);
    std::vector<MagicSquare> offspring;

    std::for_each(selected.begin(),
                  selected.end(), [selected, magicSum, &offspring, &population](MagicSquare parent1) {
                std::mt19937 rng1(std::random_device{}());
                std::uniform_int_distribution<int> distFill(1, SIZE * SIZE);
                std::uniform_int_distribution<int> distParent(0, POPULATION_SIZE - 1);
                MagicSquare parent2 = population[distParent(rng1)];
                MagicSquare child{};

                // Copy rows from first parent
                int row = 0;
                for (auto &value: parent1.values) {
                    int sum = 0;

                    for (int j: value) sum += j;

                    if (sum == magicSum) {
                        int col = 0;

                        for (int j: value) {
                            child.values[row][col] = j;
                            col++;
                        }
                    }

                    row++;
                }

                // Copy Columns from first parent
                for (int col = 0; col < SIZE; col++) {
                    int sum = 0;

                    for (auto &value: parent1.values) sum += value[col];

                    if (sum == magicSum) {
                        row = 0;

                        for (auto &value: parent1.values) {
                            child.values[row][col] = value[col];
                            row++;
                        }
                    }
                }

                // Add some values from second parent
                row = 0;
                for (auto &value: parent2.values) {
                    int col = 0;

                    for (int j: value) {
                        if (child.values[row][col] == 0) {
                            int *data = &child.values[0][0];

                            if (std::find(data,
                                          data + SIZE * SIZE,
                                          j) == data + SIZE * SIZE) child.values[row][col] = j;
                        }

                        col++;
                    }

                    row++;
                }

                // Fill rest of empty values
                for (auto &childRow: child.values) {
                    for (auto &childColum: childRow) {
                        if (childColum == 0) {
                            int tmpValue = distFill(rng1);
                            int *data = &child.values[0][0];

                            do {
                                if (std::find(data,
                                              data + SIZE * SIZE,
                                              tmpValue) == data + SIZE * SIZE) childColum = tmpValue;

                                tmpValue = distFill(rng1);
                            } while (childColum == 0);
                        }
                    }
                }

                offspring.push_back(child);
            });

    return offspring;
}

void swap(MagicSquare &square) {
    std::uniform_int_distribution<int> dist(0, SIZE - 1);

    int fromRow = dist(rng);
    int toRow = dist(rng);
    int fromCol = dist(rng);
    int toCol = dist(rng);

    std::swap(square.values[fromRow][fromCol], square.values[toRow][toCol]);
}

/**
 * Change position of two random numbers.
 *
 * @param population
 * @param mutationProbability
 */
void mutation(std::vector<MagicSquare> &population, double mutationProbability) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    // Mutate each square with a certain probability
    for (auto &square: population) {
        if (dist(rng) <= mutationProbability) {
            swap(square);
        }
    }
}
