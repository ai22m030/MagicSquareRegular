//
// Created by Adnan Vatric on 18.02.23.
//

#include <iostream>
#include <vector>
#include <algorithm>

#include "program_options.h"
#include "magic_square.h"

/**
 * Run the evolutionary algorithm to solve a magic square.
 *
 * @param maxIterations
 * @param infiniteLoop
 */
void run_ea(int maxIterations, bool infiniteLoop) {
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

    for (int iteration = 0; (iteration < maxIterations) || infiniteLoop; iteration++) {
        // Evaluate the fitness of each square solution
        for (auto &square: population) {
            evaluateFitness(square, magicSum);
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

        // Perform selection and crossover
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

        // Manipulate population if stopped evolving
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
}

/**
 * Entry point
 *
 * @return
 */
int main(int argc, char** argv) {
    const std::vector<std::string_view> args(argv, argv + argc);
    const bool interactiveFlag = program_options::has(args, "-i");
    bool infiniteLoop = program_options::has(args, "-inf");
    int maxIterations = MAX_ITERATIONS;

    if(interactiveFlag) {
        char type;

        do
        {
            std::cout << "Run in infinite loop? [y/n]" << std::endl;
            std::cin >> type;
        }
        while(!std::cin.fail() && type!='y' && type!='n');

        if(type == 'y') infiniteLoop = true;

        if(!infiniteLoop) {
            std::cout << "Amount of iterations? [1 000 - 100 000]" << std::endl;
            std::cin >> maxIterations;

            while(!std::cin.good() || (maxIterations < 1000 || maxIterations > 100000)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Amount of iterations? [1 000 - 100 000]" << std::endl;
                std::cin >> maxIterations;
            }
        }
    } else {
        if(program_options::has(args, "-i"))
            maxIterations = std::stoi(program_options::get(args, "-i").begin());
    }

    run_ea(maxIterations, infiniteLoop);

    return EXIT_SUCCESS;
}
