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
#include <filesystem>
#include <vector>
#include <string>
#include <sstream>
#include <exception>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <map>

namespace fs = std::filesystem;


std::map<std::string,int> get_dirs_from_user();
int run_quick_sort_on_input_files(const std::map<std::string,int> &dirs);
int find_average_and_save_times(const std::vector<std::vector<double>> exe_times);

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
        double write_time_to_file(const std::string filename) const;
        void print_array() const;
};

// Entry Point - Driver Code to run QuickSort on input array
int main(int argc, char **argv) {
    
    // Ensure the right number of command line arguments were given
    if (argc != 1){
        std::cout << "Usage: ./quicksort" << std::endl;
        return 1;
    }

    std::map<std::string, int> dirs = get_dirs_from_user();

    // Run quick sort on each of the 75 input files
    if (!run_quick_sort_on_input_files(dirs)) {
        return 1;
    }

    return 0;
}

std::map<std::string,int> get_dirs_from_user() {
    /**
     * Gets the path to directories with input files from users
     * User provides the number of input numbers in the files of this directory
     * Iterates until user quits
     * 
     * Returns:
     *  map<int,int>    :   map of filepath to the input size of its files
     */

    std::map<std::string, int> dirs;
    bool done = false;
    std::string path;
    std::string response;
    int input_size;

    std::cout << "Enter directories containing input files with the same number"
        <<" of values per file" << std::endl;

    while (!done) {
        // Get directory path
        std::cout << "Enter the path to the directory: " << std::endl;
        std::cin >> path;
        if (!fs::is_directory(fs::status(path))) {
            std::cout << "Directory does not exist" << std::endl;
            continue;
        }

        // Get input size
        std::cout << "Enter the input size of the files in this directory " 
            << std::endl;
        std::cin >> input_size;
        while(std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid entry. Enter an Integer: ";
            std::cin >> input_size;
        }

        // Create new entry in map
        dirs.insert(std::pair<std::string, int>(path, input_size));

        std::cout << "Done? (y/n)" << std::endl;
        std::cin >> response;
        if (response == "y") {
            done = true;
        }
    }

    for(auto k:dirs) {
        std::cout << k.first << " " << k.second << std::endl;
    }

    return dirs;
}

int run_quick_sort_on_input_files(const std::map<std::string,int> &dirs) {
    /**
     * Runs the quick sort algorithm on each of the files in the given directory
     * Outputs the sorted arrays and the execution times for each input size
     * 
     * Parameters:
     *      dirs (map<string, int>) :   map of directory names to the input size of
     *          the files they contain
     *          (dir.first = path to directory)
     *          (dir.second = input size)
     * 
     * Returns:
     *      int :   returns 1 to indicate success or 0 for failure
     */
    
    // Initialize instance of QuickSort
    QuickSort q;

    // Create Root Output Directory
    std::string out_dir = "output";     // TODO time
    fs::create_directory(out_dir);

    std::string in_path;
    std::string in_fn;
    std::string sorted_dir;
    std::string time_dir;
    std::string sorted_path;
    std::string time_path;

    // map of input size to vector execution times of each file
    std::map<int, std::vector<double>> exe_times;

    // Repeat for each provided directory
    for (auto m : dirs) {
        // Create entry in exe_times for this directory if this input size is new
        std::string in_dir = m.first;
        int input_size = m.second;

        // Check if directory exists
        if (!fs::is_directory(fs::status(in_dir))) {
            std::cout << "Input Directory does not exist - Exiting" << std::endl;
            return 0;
        }

        if(!exe_times.count(input_size)) {
            exe_times.insert(std::pair<int, std::vector<double>>(
                input_size, std::vector<double>()));
        }

        // Create Output Directories
        std::string dir_name = in_dir.substr(in_dir.find_last_of("/"));
        sorted_dir = fs::path(out_dir +"/"+ dir_name + "/sorted");
        time_dir = fs::path(out_dir +"/"+ dir_name + "/execution-time");
        // fs::create_directory(fs::path(out_dir+"/"+dir_name));
        fs::create_directories(sorted_dir);
        fs::create_directories(time_dir);

        // Read each input file in this dir and run quick sort on them
        for (const auto & entry : fs::directory_iterator(in_dir)) {
            in_path = std::string(entry.path());    // Path to each file
            in_fn = in_path.substr(in_path.find_last_of("/") + 1);  // filename
            sorted_path = fs::path(sorted_dir +"/"+ in_fn);
            time_path = fs::path(time_dir +"/"+ in_fn);

            // Read File
            if (!q.read_file(in_path)) {
                // If read failed, write empty array to output file and exit
                q.write_file(sorted_path);
                return 0;
            }

            // Run Quick Sort
            q.quick_sort();

            // Write Sorted Array
            q.write_file(sorted_path);

            // Write and save execution time
            exe_times[input_size].push_back(q.write_time_to_file(time_path));
        }
    }

    // // Read each input file and run quick sort on them
    // for (const auto & entry : fs::directory_iterator(in_dir)) {
    //     in_path = std::string(entry.path());

    //     // Read File
    //     if (!q.read_file(in_path)) {
    //         // If read failed, write empty array to output file and exit
    //         q.write_file(sorted_path);
    //         return 0;
    //     }

    //     // Run Quick Sort
    //     q.quick_sort();

    //     // Write Sorted Array
    //     q.write_file(sorted_path);

    //     // Write and save execution time
    //     exe_time_millis = q.write_time_to_file(time_path);
    //     if (exe_time_millis >= 0) {
    //         if (in_path.find("10-") != std::string::npos) {
    //             // This input file should have 10 inputs
    //             exe_times[0].push_back(exe_time_millis);
    //         }
    //         else if (in_path.find("100-") != std::string::npos) {
    //             // This input file should have 100 inputs
    //             exe_times[1].push_back(exe_time_millis);
    //         }
    //         else if (in_path.find("1000-") != std::string::npos) {
    //             // This input file should have 1000 inputs
    //             exe_times[2].push_back(exe_time_millis);
    //         }
    //     }
    // }

    // Write times and averages
    // find_average_and_save_times(exe_times);
    return 1;
} 

int find_average_and_save_times(const std::vector<std::vector<double>> exe_times) {
    /**
     * Finds the average execution time for 10, 100, and 1000 input sizes
     * Writes the execution time and theaverages to a file in the given output time directory
     *
     * Parameters:
     *      exe_times (vector<vector<double>>): Total number of files
     *          and total execution times for each input size
     *
     * Returns:
     *      (int)   :   returns 1 for success
     */

    std::ofstream time_out_file("Azeem_Musa_executionTime.txt");
    if (!time_out_file) {
        std::cerr << "Error Opening Output File" << std::endl;
        return 0;   // return 0 to indicate failure
    }
    time_out_file << "Input Size    Execution Time (ms)" << std::endl;

    std::vector<double> averages;
    double sum = 0;
    int input_size = 1;

    for (int i=0; i<3; i++) {
        input_size *= 10;
        for (auto exe_time : exe_times[i]) {
            // Write execution times and find average for each input size
            sum += exe_time;
            time_out_file << input_size << "    " << exe_time << std::endl;
        }

        // Save average
        averages.push_back(sum / exe_times[i].size());
        sum = 0;
    }
    time_out_file.close();

    // Write Averages to file
    std::ofstream avg_out_file("Azeem_Musa_averageExecutionTime.txt");
    if (!avg_out_file) {
        std::cerr << "Error Opening Average Output File" << std::endl;
        return 0;   // return 0 to indicate failure
    }

    avg_out_file << "Input Size    Average Execution Time (ms)" << std::endl;
    avg_out_file << "10    " << averages[0] << std::endl;
    avg_out_file << "100    " << averages[1] << std::endl;
    avg_out_file << "1000    " << averages[2] << std::endl;

    avg_out_file.close();
    return 1;
}

// QuickSort Functions

QuickSort::QuickSort() {
    /**
     * Default Constructor
     * 
     * Initialize A as empty vector
     * Provides seed for random number generation later
     */
    A = std::vector<double>();
    std::srand(time(0));            // Used for random pivot generation
}

int QuickSort::read_file(const std::string filename){
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

    std::string line;                   // line of input file
    double value;                       // each value read from file

    A = std::vector<double>();          // Initialize new array

    // Open file
    std::ifstream in_file(filename);    // Input file stream
    if (!in_file) {
        std::cerr << "Error Opening Input File" << std::endl;
        return 0;   // return 0 to indicate failure
    }

    // Get each line of input file 
    while(std::getline(in_file, line)) { 
        std::istringstream iss(line);   // Delim file line by whitespace
        while (iss >> value) {
            A.push_back(value);         // add each value to vector
        }
    }
    in_file.close();

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
    if (!out_file) {
        std::cerr << "Error Opening Output File" << std::endl;
        return 0;   // return 0 to indicate failure
    }

    // Write to file
    for (auto val : A) {
        out_file << val << " ";
    }
    out_file.close();
    
    return 1;
}

double QuickSort::write_time_to_file(const std::string filename) const{
    /**
     * This function writes the execution time in milliseconds of the most 
     *   recent quick sort algorithm that was run to a file of a given name
     * Output file
     *  - Writes floating point time delta
     * 
     * Parameters:
     *      filename (string)   :   Name of file to write to
     *
     * Returns:
     *      (double)    :   execution time in milliseconds
     */

    // Calculate execution time of most recent quick_sort
    double delta_millis = (end_time - start_time).count()/1000.0;

    std::ofstream out_file(filename);

    if (!out_file) {
        std::cerr << "Error Opening Output File" << std::endl;
        return -1;   // return -1 to indicate failure
    }

    // Write to file
    out_file << delta_millis;
    out_file.close();

    return delta_millis;
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