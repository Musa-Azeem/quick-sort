/**
 * Author :     Musa Azeem
 * Date   :     12/2/2022
 * File   :     Azeem_Musa_InputFileGenerator.cpp
 * 
 * This C++ program produces random input files to test and time the quick sort
 *   implementation in Azeem_Musa_QuickSort.cpp
 * It generates 75 random input files (25 each of 10, 100, and 1000 
 *   floating point numbers)
 * 
 * Output Format:
 *  - Output files are ASCII and contain random floating-point numbers
 *    seperated by whitespace
 *  - Output files are saved in the "input-generated" directory 
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>

int generate_files();

int main(int argc, char **argv) {
    if (argc != 1) {
        std::cout << "Usage: ./generate_files" << std::endl;
        return 1;
    }
    
    generate_files();
}