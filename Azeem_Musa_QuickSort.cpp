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

int read_file(const char *filename, std::vector<double> &A);
int quick_sort(std::vector<double> & A);
int quick_sort(std::vector<double> & A, const int l, const int r);
int hoarse_partition(std::vector<double> &A, const int l, const int r);
int generate_random_int(int lower, int upper);
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

    write_file(argv[2], A);

    return 0;
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


int quick_sort(std::vector<double> & A) {
    /**
     * Implements the QuickSort algorithm, as shown in the psuedo code
     * Algorithm is implemented in overloaded recursive function
     * 
     * Paramters:
     *      A (vector<double>)  :   Array to sort
     * 
     * Returns:
     *      (int)   :   returns 1 to indicate success
     * 
     * Psuedo code:
     * 
     *  ALGORITHM Quicksort(A[l..r])
     *      // Input: Subarray of array A[0..n-1], defined by its left and
     *      //        right indices l and r
     *      // Output: Subarray A[l..r] sorted in nondecreasing order
     *      
     *      if l < r
     *          s <- HoarsePartition(A[l..r])     // s is a split position
     *          Quicksort(A[l..s-1])
     *          Quicksort(A[s+1..r])
     * 
     *  ALGORITHM HoarsePartition(A[l..r])
     *      // Partitions a subarray by Hoare’s algorithm, using a random pivot
     *      // Input: Subarray of array A[0..n − 1], defined by its left and right
     *      //        indices l and r (l < r)
     *      // Output: Partition of A[l..r], with the split position returned as
     *      //         this function’s value
     *      p <- A[random(l, r)]
     *      i <- l
     *      j <- r + 1
     *      repeat
     *          repeat i <- i + 1 until A[i] >= p 
     *          repeat j <- j − 1 until A[j ] <= p 
     *          swap(A[i], A[j])
     *      until i >= j
     *      swap(A[i], A[j]) //undo last swap when i >= j 
     *      swap(A[l], A[j])
     *      return j
     */

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

    hoarse_partition(A, l, r);

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

    int s = 0;
    int p = generate_random_int(l, r+1);
    return s;
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

void print_array(const std::vector<double> A){
    for (auto num : A){
        std::cout << num  << " ";
    }
    std::cout << std::endl;
}
