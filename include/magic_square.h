//
// Created by Adnan Vatric on 18.02.23.
//

#ifndef MAGIC_SQUARE_MAGIC_SQUARE_H
#define MAGIC_SQUARE_MAGIC_SQUARE_H

#include <vector>

// Calculate the magic sum of a square of given size
#define MAGIC_SUM(square_size) ((square_size * (square_size * square_size + 1)) / 2)

// Define the size of the magic square
const int SIZE = 5;

// Define the maximum number of iterations
const int MAX_ITERATIONS = 10000;

// Define the population size
const int POPULATION_SIZE = 10000;

// Define the mutation rate
const double BASE_MUTATION_PROBABILITY = 0.1;

/**
 * Base structure of a single magic square.
 * The size is defined by const.
 *
 */
struct MagicSquare {
    int values[SIZE][SIZE]{};
    int fitness = 0;

    void print();
};

MagicSquare randomSquare();
int fitnessRows(MagicSquare&, int);
int fitnessColumns(MagicSquare&, int);
int fitnessDiagonal1(MagicSquare&, int);
int fitnessDiagonal2(MagicSquare&, int);
void evaluateFitness(MagicSquare&, int);
std::vector<MagicSquare> parentSelection(std::vector<MagicSquare>&);
std::vector<MagicSquare> crossover(const std::vector<MagicSquare>&, std::vector<MagicSquare>&);
void swap(MagicSquare&);
void mutation(std::vector<MagicSquare>&, double);

#endif //MAGIC_SQUARE_MAGIC_SQUARE_H
