/**
 * Author :     Musa Azeem
 * Date   :     12/2/2022
 * File   :     InputFileGenerator.cpp
 * 
 * This C++ program produces random input files to test and time the quick sort
 *   implementation in Azeem_Musa_QuickSort.cpp
 * It generates 75 random input files (25 each of 10, 100, and 1000 
 *   floating point numbers)
 * 
 * Usage: ./InputFileGenerator [Output Directory]
 * 
 * 
 * Output Format:
 *  - Output files are ASCII and contain random floating-point numbers
 *    seperated by whitespace
 *  - Output files are saved in the given directory 
 *  - Filenames are in format, 10-0, 10-1, ... ,100-0, 100-1, ... 1000-23, 1000-24
 */

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <filesystem>
#include <random>
#include <climits>

int generate_files(std::string dir);
int generate_files(int num_of_files, int num_of_values, std::string dir);

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: ./InputFileGenerator [Output Directory]" << std::endl;
        return 1;
    }

    std::string dir = argv[1];
    if (!generate_files(dir)) {
        std::cout << "Failed to write files " <<
            "- output directory does not exist" << std::endl;
    }
}

int generate_files(std::string dir) {
    /**
     * Generates ASCII files with random floating point numbers,
     *  seperated by whitespaces
     * Generates 75 files:
     *  - 25 with 10 values
     *  - 25 with 100 values
     *  - 25 with 1000 values
     */

    if (!generate_files(25, 10, dir) ||
        !generate_files(25, 100, dir) ||
        !generate_files(25, 1000, dir)) {
        
        return 0;
    }
    return 1;
}

int generate_files(int num_of_files, int num_of_values, std::string dir) {
    /**
     * Generates <number_of_files> files, 
     *   each with <num_of_values> random values
     * 
     * Parameters:
     *  num_of_files (int)  :   Number of files to generate
     *  num_of_values (int) :   Number of random values to generate
     * 
     * Returns:
     *  (int)   :   Returns 1 if successful, 0 if not
    */

    for (int i=0; i<num_of_files; i++) {

        std::filesystem::path p(dir + "/" + std::to_string(num_of_values)
            + "-" + std::to_string(i) + ".txt");

        std::ofstream out_file(p);
        if (out_file) {
            // Write <num_of_values> random double values
            for (int j=0; j<num_of_values; j++) {
                std::random_device rand_dev;            
                std::mt19937 generator(rand_dev());     // RNG
                std::uniform_int_distribution<int> distr(-100000, 100000);
                out_file << distr(generator) << " ";
            }
        }

        else {
            return 0;
        }            
    }
    return 1;
}