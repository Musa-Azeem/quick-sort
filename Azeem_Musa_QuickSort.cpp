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

int read_file(const char *filename, std::vector<double> &A);
int quick_sort(std::vector<double> & A);
int write_file(const char *filename, const std::vector<double> A);
int print_array(const std::vector<double> A);

// Entry Point - Driver Code to run QuickSort on input array
int main(int argc, char **argv) {
    
    // Ensure the right number of command line arguments were given
    if (argc != 3){
        std::cout << "Usage: ./quicksort [input file] [output file]" << std::endl;
        return 1;
    }
    
    std::vector<double> A;      // Vector to represent input values
    
    // Read file into array A
    if (!read_file(argv[1], A)) {
        // If read failed, write empty array to output file and exit
        // write_file(argv[2], A);
        return 1;
    }



    return 0;
}

int read_file(const char *filename, std::vector<double> &A){
    /**
     * This function reads a file from a given filename and populates the
     *  given array with the values
     * Input file
     * 
     * Parameters:
     *      filename (char *)   :   name of file to read values from
     *      A (vector<double>)  :   Output vector with values read from file
     */

    std::ifstream in_file;      // Input file stream
    std::string line;           // line of input file
    double value;               // each value read from file

    try {
        in_file.open(filename);
        std::cout << "open" << std::endl;

        // Get each line of input file 
        while(std::getline(in_file, line)){ 
            std::istringstream iss(line);   // Delim file line by whitespace
            while (iss >> value){
                std::cout << "value: " << value << std::endl;
                A.push_back(value);         // add each value to vector
            }
        }
        in_file.close();
    }
    catch (const std::exception& e) {
        std::cout << "Error Reading File - " << e.what() << std::endl;
        return 0;   // return 0 to indicate failure
    }

    // Check if file was empty or did not exist
    if (A.size() == 0) {
        std::cout << "Input file is empty or does not exist - sorting not complete" << std::endl;
        return 0;   // return 0 to indicate failure
    }
    
    return 1;
}

int write_file(const char *filename, const std::vector<double> A) {
    // TODO write array to file
}


int quick_sort(std::vector<double> & A) {
    /**
     *  ALGORITHM Quicksort(A[l..r])
     *      // Input: Subarray of array A[0..n-1], defined by its left and
     *      //        right indices l and r
     *      // Output: Subarray A[l..r] sorted in nondecreasing order
     *      
     *      if l < r
     *          s <- LomutoPartition(A[l..r])     // s is a split position
     *          Quicksort(A[l..s-1])
     *          Quicksort(A[s+1..r])
     * 
     *  ALGORITHM LomutoPartition(A[l..r])
     *      // Partitions subarray by Lomuto’s algorithm using first element as 
     *         pivot
     *      // Input: A subarray A[l..r] of array A[0..n − 1], defined by its 
     *               left and right // indices l and r (l ≤ r)
     *      // Output: Partition of A[l..r] and the new position of the pivot
     * 
     *      p <- A[l]
     *      s <- l
     *      for i <- l+1 to r do
     *          if A[i] < p
     *              s <- s+1
     *              swap(A[s], A[i])
     *      swap(A[l], A[s])
     *      return s
     */
}

int print_array(const std::vector<double> A){
    for (auto num : A){
        std::cout << num  << " ";
    }
    std::cout << std::endl;
}
