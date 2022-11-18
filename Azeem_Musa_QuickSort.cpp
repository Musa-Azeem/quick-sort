/**
 * Author :     Musa Azeem
 * Date   :     12/2/2022
 * File   :     Azeem_Musa_QuickSort.cpp
 * 
 * This C++ program implements the QuickSort Algorithm
 *  - It sorts an array of floating points given in an input file 
 *  - It outputs the sorted array in a given output file
 *  - It also outputs a file with the execution time of the algorithm
 *  
 * 
 * Input Format:
 *  - Input file should be an ASCII file that contains a list of unsorted
 *    floating-point numbers separated by a blank space
 * 
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <exception>
#include <cstdlib>
#include <cmath>

// Function Headers
int read_file(const char *filename, std::vector<double> &A);
int quick_sort(std::vector<double> & A);
int quick_sort(std::vector<double> & A, const int l, const int r);
int hoarse_partition(std::vector<double> &A, const int l, const int r);
int generate_random_int(int lower, int upper);
void swap(std::vector<double> &A, const int i, const int j);
int write_file(const char *filename, const std::vector<double> A);
void print_array(const std::vector<double> A);

// Entry Point - Driver Code to run QuickSort on input array
int main(int argc, char **argv) {
    
    // Ensure the right number of command line arguments were given
    if (argc != 3){
        std::cout << "Usage: ./quicksort [input file] [output file]" << std::endl;
        return 1;
    }

    std::srand(time(0));        // Used for random pivot generation
    
    std::vector<double> A;      // Vector to represent input values
    
    // Read file into array A
    if (!read_file(argv[1], A)) {
        // If read failed, write empty array to output file and exit
        write_file(argv[2], A);
        return 1;
    }

    // Sort the array
    quick_sort(A);

    print_array(A);
    write_file(argv[2], A);

    return 0;
}

int quick_sort(std::vector<double> & A) {
    /**
     * Implements the QuickSort algorithm using  the 
     * Hoarse Partition Algorithm with random pivots
     * Algorithm is implemented in overloaded recursive function
     * 
     * Paramters:
     *      A (vector<double>)  :   Array to sort
     * 
     * Returns:
     *      (int)   :   returns 1 to indicate success
     * 
     */

    if (A.size() == 0 || A.size() == 1) {
        // If array is empty or has only one element, do nothing
        return 1;
    }

    // QuickSort starting with first and last indices
    return quick_sort(A, 0, A.size()-1);     
}

int quick_sort(std::vector<double> &A, const int l, const int r) {
    /**
     * Implements the QuickSort algorithm to sort a given array
     * 
     * Parameters:
     *  A (vector<double>)  :   subarray to sort
     *  l (int) :   Index to start subarray
     *  r (int) :   Index to stop subarray
     * 
     * Returns:
     *  (int)   : returns 1 to indicate success
     */

    int s = 0;      // Partition index
    // Continue until indices overlap
    if (l < r) {
        // Parition array and get index to split
        s = hoarse_partition(A, l, r);
        quick_sort(A, l, s-1);      // Sort left partition
        quick_sort(A, s+1, r);      // Sort right partition
    }
    return 1;
}

int hoarse_partition(std::vector<double> &A, const int l, const int r) {
    /**
     * Implements a hoarse partition on a provided subarray
     * Input:
     *      A (vector<double>)  :   array to partition
     *      l (int) :   Index to start subarray
     *      r (int) :   Index to end subarray
     * 
     * Returns:
     *  (int)   :   Index that split occurs
     */

    // Generate random pivot and move the value to beginning of subarray
    swap(A, l, generate_random_int(l, r+1));
    int p = A[l]; 
    
    int i = l;    // Start i at left index after pivot
    int j = r+1;      // Start j at right index

    // std::clog << "Pivot for subarray A[" << l << ".." << r << "] is: " << p << std::endl;

    // Repeat iteration until i and j overlap
    while (i < j) {
        do i++; while(A[i] < p && i < r);    // Increment i until a value greater than p is reached
        do j--; while(A[j] > p && j > l);      // Decrement j until a value less than p is reached
        swap(A, i, j);
    }
    swap(A, i, j);          // Undo last swap when i >= j
    swap(A, l, j);          // Swap pivot with j, the new partition

    return j;           // Return the partition index
}

int generate_random_int(int lower, int upper) {
    /**
     * Generates a random integer in the range [lower, upper)
     * 
     * Parameters:
     *  lower (int) :   lower limit of random number to generate (inclusive)
     *  upper (int) :   upper limit of random number to generate (exclusive)
     * 
     * Returns:
     *  (int)   :   Random integer within given range
     */
    
    if (lower > upper) {
        // Return lower limit and log error if range is invalid
        std::cerr << "generate_random_int : invalid range" 
            << " - returning lower limit" << std::endl;
        return lower;
    }
    if (lower == upper) {
        // Return lower limit if the range is 0
        return lower;
    }
    // Return a random integer within given range
    int range = upper - lower;
    return std::floor(std::rand() % range + lower);
}

void swap(std::vector<double> &A, const int i, const int j) {
    /**
     * This function swaps the values at the given indices
     * 
     * Parameters:
     *      A (vector<double>)  :   array to modify
     *      i (int) :   first index to swap
     *      j (int) :   second index to swap
     */

    if (i >= A.size()) {
        // If first index is out of range, do nothing
        std::cerr << "swap(" << i << ", " << j << ") : index i is out of range "
            << "- swap incomplete" << std::endl;
        return;
    }
    if (j >= A.size()) {
        // If second index is out of range, do nothing
        std::cerr << "swap(" << i << ", " << j << ") : index j is out of range "
            << "- swap incomplete" << std::endl;
        return;
    }
    if (i == j) {
        // If indices are equal, do nothing
        return;
    }

    // Swap values at given indices
    double tmp = A[i];
    A[i] = A[j];
    A[j] = tmp;
}

int read_file(const char *filename, std::vector<double> &A){
    /**
     * This function reads a file from a given filename and populates the
     *  given array with the values
     * Input file
     *  - File containing floating point numbers seperated by whitespace
     * 
     * Parameters:
     *      filename (char *)   :   name of file to read values from
     *      A (vector<double>)  :   Output vector with values read from file
     */

    std::ifstream in_file(filename);    // Input file stream
    std::string line;                   // line of input file
    double value;                       // each value read from file

    if (in_file) {
        // Get each line of input file 
        while(std::getline(in_file, line)) { 
            std::istringstream iss(line);   // Delim file line by whitespace
            while (iss >> value) {
                A.push_back(value);         // add each value to vector
            }
        }
    }
    else {
        std::cerr << "Error Opening Input File" << std::endl;
        return 0;   // return 0 to indicate failure
    }

    // Check if file was empty or did not exist
    if (A.size() == 0) {
        std::cerr << "Input file is empty or does not exist - sorting not complete" << std::endl;
        return 0;   // return 0 to indicate failure
    }
    
    return 1;
}

int write_file(const char *filename, const std::vector<double> A) {
    /**
     * This function writes a vector to a file of a given name
     * Output file
     *  - Outputs values of array seperated by whitespace
     * 
     * Parameters:
     *      filename (char *)   :   Name of file to write values to
     *      A (vector<double>)  :   Vector with values to write to file
     */

    std::ofstream out_file(filename);

    if (out_file) {
        for (auto val : A) {
            out_file << val << " ";
        }
    }    
    else {
        std::cerr << "Error Opening Output File" << std::endl;
        return 0;   // return 0 to indicate failure
    }

    return 1;
}

void print_array(const std::vector<double> A){
    for (auto num : A){
        std::cout << num  << " ";
    }
    std::cout << std::endl;
}