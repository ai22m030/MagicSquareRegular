#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

// Calculate the magic sum of a square of given size
#define MAGIC_SUM(square_size) ((square_size * (square_size * square_size + 1)) / 2)

// Define the size of the magic square
const int SIZE = 5;

// Define the maximum number of iterations
const int MAX_ITERATIONS = 10000;

// Define the population size
const int POPULATION_SIZE = 50000;

// Define the mutation rate
const double BASE_MUTATION_PROBABILITY = 0.1;

// Define the random number generator
std::mt19937 rng(std::random_device{}());

/**
 * Base structure of a single magic square.
 * The size is defined by const.
 *
 */
struct MagicSquare {
    int values[SIZE][SIZE]{};
    int fitness = 0;

    /**
     * Print content of square. Is used for debugging purposes.
     */
    void print() {
        for (auto &value: this->values) {
            for (int j: value) {
                std::cout << j << " ";
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }
};

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
 * @param candidate
 * @param magicSum
 */
void evaluateFitness(MagicSquare &candidate, int magicSum) {
    candidate.fitness = 0;

    candidate.fitness += fitnessRows(candidate, magicSum);
    candidate.fitness += fitnessColumns(candidate, magicSum);
    candidate.fitness += fitnessDiagonal1(candidate, magicSum);
    candidate.fitness += fitnessDiagonal2(candidate, magicSum);
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

/**
 * Entry point
 *
 * @return
 */
int main() {
    // Generate the initial population
    std::vector<MagicSquare> population;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        population.push_back(randomSquare());
    }

    // Repeat the evolutionary algorithm
    int lastFitness = -1;
    int magicSum = MAGIC_SUM(SIZE);
    double mutationProbability = BASE_MUTATION_PROBABILITY;
    int unchanged_count = 0;

    for (int iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
        // Evaluate the fitness of each square solution
        for (auto &candidate: population) {
            evaluateFitness(candidate, magicSum);
        }

        // Check if we have found a solution
        auto it = std::min_element(
                population.begin(),
                population.end(), [](const MagicSquare &a, const MagicSquare &b) {
            return a.fitness < b.fitness;
        });

        // Stop algorithm and print out solution if found
        if (it->fitness == 0) {
            std::cout << "Right solution:\n";
            it->print();
            break;
        } else {
            int max_output = 0;
            std::cout << "Best solution:\n";
            it->print();
            while (max_output < 5) {
                std::cout << it->fitness << std::endl;
                max_output++;
                it++;
            }
        }

        // Perform selection, crossover, and mutation
        std::vector<MagicSquare> selected = parentSelection(population);
        std::vector<MagicSquare> offspring = crossover(selected, population);

        // Change mutation probability depending on fitness
        if (it->fitness == lastFitness) {
            unchanged_count++;

            if (mutationProbability < 1) mutationProbability += 0.1;
        } else {
            if (unchanged_count > 0) unchanged_count--;

            if (mutationProbability >= 1) mutationProbability = BASE_MUTATION_PROBABILITY;

            if (mutationProbability > BASE_MUTATION_PROBABILITY) mutationProbability -= 0.1;
        }

        mutation(offspring, mutationProbability);
        lastFitness = it->fitness;

        // Drop all except of one child if children stay same
        if (unchanged_count % 50 != 0) {
            if(unchanged_count % 100 != 0) {
                for (int i = 0; i < POPULATION_SIZE - 1; i++) swap(population[i]);

                unchanged_count = 0;

                continue;
            }

            for (int i = 1; i < POPULATION_SIZE - 1; i++) swap(population[i]);

            continue;
        }

        // Add some new squares in between
        for (int i = POPULATION_SIZE / 4; i < POPULATION_SIZE / 2; i++) population[i] = randomSquare();

        // Replace the worst squares with the offspring
        for (int i = POPULATION_SIZE / 2; i < POPULATION_SIZE; i++) population[i] = offspring[i - POPULATION_SIZE / 2];
    }

    return 0;
}