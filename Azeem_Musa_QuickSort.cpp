/**
 * Author :     Musa Azeem
 * Date   :     12/2/2022
 * File   :     Azeem_Musa_QuickSort.cpp
 * 
 * This C++ program implements the quick sort algorithm in the QuickSort class
 *  - It sorts an array of floating points given in an input file 
 *  - It outputs the sorted array in a given output file
 *  - It also outputs a file with the execution time of the algorithm
 * 
 * Usage: ./Azeem_Musa_QuickSort [input_file] [output_file]
 *  
 * 
 * Input Format:
 *  - Input file should be an ASCII file that contains a list of unsorted
 *    floating-point numbers separated by a blank space
 * 
 * Output Format:
 *  - First output file is an ASCII file containing a list of sorted 
 *    floating-point numbers seperated by a blank space
 *  - Second output file (named <filename>-exe_time_millis.txt) is an
 *    ASCII file containing the execution time in milliseconds taken to run
 *    the quick sort algorithm
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <exception>
#include <cstdlib>
#include <cmath>
#include <chrono>

std::string get_time_out_fn(std::string out_fn);

class QuickSort {


    private:
        std::vector<double> A;
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
        std::chrono::time_point<std::chrono::high_resolution_clock> end_time;

        int hoarse_partition(const int l, const int r);
        int quick_sort(const int l, const int r);
        void swap(const int i, const int j);
        int generate_random_int(const int lower, const int upper);
    
    public:
        QuickSort();
        int read_file(const std::string filename);
        int quick_sort();
        int write_file(const std::string filename) const;
        int write_time_to_file(const std::string filename) const;
        void print_array() const;
};

// Entry Point - Driver Code to run QuickSort on input array
int main(int argc, char **argv) {
    
    // Ensure the right number of command line arguments were given
    if (argc != 3){
        std::cout << "Usage: ./quicksort [input file] [output file]" << std::endl;
        return 1;
    }

    // Get string filenames
    std::string in_fn = argv[1];
    std::string out_fn = argv[2];
    std::string time_out_fn = get_time_out_fn(out_fn);
    
    QuickSort quickSort;

    // Read Input File
    if (!quickSort.read_file(in_fn)) {
        // If read failed, write empty array to output file and exit
        quickSort.write_file(out_fn);
        return 1;
    }


    // Run and time quick sort algorithm
    quickSort.quick_sort();

    // Write Sorted Array to Output File
    quickSort.write_file(out_fn);

    // Write Execution Time to Output File
    quickSort.write_time_to_file(time_out_fn);

    return 0;
}

std::string get_time_out_fn(std::string out_fn) {
    int ext_pos = out_fn.find(".");
    if (ext_pos == std::string::npos) {
        return out_fn.append("-exe_time_millis.txt");
    }
    else {
        int ext_len = out_fn.length() - ext_pos;
        return out_fn.replace(ext_pos, ext_len, "").append("-exe_time_millis.txt");
    }
}


// QuickSort Functions

QuickSort::QuickSort() {
    /**
     * Default Constructor
     * 
     * Initialize A as empty vector
     * Provides seed for random number generation later
     */
    // A = std::vector<double>();
    std::srand(time(0));            // Used for random pivot generation
}

int QuickSort::read_file(std::string filename){
    /**
     * Reads a file from a given filename and populates the "A" vector
     * 
     * Input file
     *  - File containing floating point numbers seperated by whitespace
     * 
     * Parameters:
     *      filename (string)   :   name of file to read values from
     * 
     * Returns:
     *      int :   Returns 1 if file reading was successful, 0 if not
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
        std::cerr << "Input file is empty or does not exist - array not populated" << std::endl;
        return 0;   // return 0 to indicate failure
    }
    
    return 1;
}

int QuickSort::quick_sort() {
    /**
     * Implements the QuickSort algorithm using the Hoarse Partition 
     *  Algorithm with random pivots
     * Records start and end time of algorithm
     * Algorithm is implemented in overloaded recursive function
     * 
     * Returns:
     *      (int)   :   returns 1 to indicate success
     * 
     */

    start_time = std::chrono::high_resolution_clock::now();     // Start timer

    if (A.size() == 0 || A.size() == 1) {
        // If array is empty or has only one element, do nothing
        return 1;
    }

    // QuickSort starting with first and last indices
    int ret = quick_sort(0, A.size()-1);

    end_time = std::chrono::high_resolution_clock::now();       // Stop timer

    return ret;
}

int QuickSort::quick_sort(const int l, const int r) {
    /**
     * Implements the QuickSort algorithm to sort "A"
     * 
     * Parameters:
     *  A (vector<double>)  :   subarray to sort
     *  l (size_t) :   Index to start subarray
     *  r (size_t) :   Index to stop subarray
     * 
     * Returns:
     *  (int)   : returns 1 to indicate success
     */

    int s = 0;      // Partition index
    // Continue until indices overlap
    if (l < r) {
        // Parition array and get index to split
        s = hoarse_partition(l, r);
        quick_sort(l, s-1);         // Sort left partition
        quick_sort(s+1, r);         // Sort right partition
    }
    return 1;
}

int QuickSort::hoarse_partition(const int l, const int r) {
    /**
     * Implements a hoarse partition on a provided subarray
     * 
     * Parameters:
     *      l (size_t)  :   Index to start subarray
     *      r (size_t)  :   Index to end subarray
     * 
     * Returns:
     *  (size_t)   :   Index that split occurs
     */

    // Generate random pivot and move the value to beginning of subarray
    swap(l, generate_random_int(l, r+1));
    int p = A[l]; 
    
    int i = l;    // Start i at left index after pivot
    int j = r+1;      // Start j at right index

    // Repeat iteration until i and j overlap
    while (i < j) {
        do i++; while(A[i] < p && i < r);    // Increment i until a value greater than p is reached
        do j--; while(A[j] > p && j > l);      // Decrement j until a value less than p is reached
        swap(i, j);
    }
    swap(i, j);          // Undo last swap when i >= j
    swap(l, j);          // Swap pivot with j, the new partition

    return j;           // Return the partition index
}

int QuickSort::generate_random_int(const int lower, const int upper) {
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

void QuickSort::swap(const int i, const int j) {
    /**
     * This function swaps the values at the given indices
     * 
     * Parameters:
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

int QuickSort::write_file(const std::string filename) const {
    /**
     * This function writes a vector to a file of a given name
     * Output file
     *  - Outputs values of array seperated by whitespace
     * 
     * Parameters:
     *      filename (string)   :   Name of file to write values to
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

int QuickSort::write_time_to_file(const std::string filename) const{
    /**
     * This function writes the execution time of the most recent quick sort
     *  algorithm that was run to a file of a given name
     * Output file
     *  - Writes floating point time delta
     * 
     * Parameters:
     *      filename (string)   :   Name of file to write to
     */

    // Calculate execution time of most recent quick_sort
    double delta_millis = (end_time - start_time).count()/1000;

    std::ofstream out_file(filename);

    if (out_file) {
        out_file << delta_millis;
    }    
    else {
        std::cerr << "Error Opening Output File" << std::endl;
        return 0;   // return 0 to indicate failure
    }

    return 1;
}

void QuickSort::print_array() const {
    /**
     * Prints "A" to stdout
     */
    for (auto num : A){
        std::cout << num  << " ";
    }
    std::cout << std::endl;
}