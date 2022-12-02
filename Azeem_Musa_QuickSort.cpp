/**
 * Author :     Musa Azeem
 * Date   :     12/2/2022
 * File   :     Azeem_Musa_QuickSort.cpp
 * 
 * This C++ program implements the quick sort algorithm in the QuickSort class
 *  - It sorts an array of floating points given in an input file 
 *  - It outputs the sorted array in a given output file
 *  - It outputs the execution time of the algorithm to a given output file
 * 
 * This C++ program also implements a UI to run quick sort on input files
 *  - Can provide any number of directories containing input files
 *  - Will sort the array of values in each file, and output the sorted arrays
 *  - Will save the execution time for each file
 *  - Will combine the execution times in a tab seperating file
 *  - Will find the average execution for each input size and save it in a file
 * 
 * 
 * Usage: ./Azeem_Musa_QuickSort
 *  
 * 
 * Input Format:
 *  - Input file should be an ASCII file that contains a list of unsorted
 *    floating-point numbers separated by a blank space
 *  - Input directories should contain n input files, each containing the same
 *    number of values
 * 
 * Output Format:
 *  - Files are saved in a directory, with the following format:
 * 
 *      output
 *      ├── Azeem_Musa_averageExecutionTime.txt
 *      ├── Azeem_Musa_executionTime.txt
 *      ├── X
 *      │   ├── execution-time
 *      │   │   ├── 0.txt
 *      │   │   ├── ...
 *      │   │   └── n.txt
 *      │   └── sorted
 *      │       ├── 0.txt
 *      │       ├── ...
 *      │       └── n.txt
 *      └── Y
 *          ├── ...
 *    
 *    where X, Y, ... are the input directories and files 0 to n are 
 *    the files within them
 *  - Each "sorted" output file is an ASCII file containing a list of sorted 
 *    floating-point numbers seperated by a blank space
 *  - Each "execution-time" output file is an ASCII file containing the 
 *    execution time in milliseconds taken to run the quick sort algorithm
 *    for that file
 *  - Azeem_Musa_averageExecutionTime.txt contains the average execution time
 *    for all of the input files combined. It is a tab seperated file with the format:
 *          [Input Size    Average Execution Time (ms)]
 *  - Azeem_Musa_executionTime.txt contains the execution time for all of the input 
 *    files combined. It is a tab seperated file with the format:
 *          [Input Size    Execution Time (ms)]
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
#include <ctime>
#include <map>

namespace fs = std::filesystem;


std::map<std::string,int> get_dirs_from_user();
int run_quick_sort_on_input_files(const std::map<std::string,int> &dirs);
int find_average_and_save_times(const std::string out_dir, 
    const std::map<int, std::vector<double>> &exe_times);

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
        double get_exe_time() const;
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
        std::cout << "Enter the path to the directory:" << std::endl;
        std::cin >> path;
        if (!fs::is_directory(fs::status(path))) {
            std::cout << "Directory does not exist" << std::endl;
            continue;
        }

        // Get input size
        std::cout << "Enter the input size of the files in this directory:" 
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
     *          (.first = path to directory)
     *          (.second = input size)
     * 
     * Returns:
     *      int :   returns 1 to indicate success or 0 for failure
     */
    
    // Initialize instance of QuickSort
    QuickSort q;

    // Create Root Output Directory
    std::time_t time;
	std::time(&time);
	tm * curr_tm = localtime(&time);
	char _out_dir[100];
    std::strftime(_out_dir, 50, "output_%y-%m-%d_%H_%M_%S", curr_tm);
    std::string out_dir(_out_dir);

    std::string in_path;
    std::string in_fn;
    std::string sorted_dir;
    std::string sorted_path;

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
            // If this input size is new, create new entry
            exe_times.insert(std::pair<int, std::vector<double>>(
                input_size, std::vector<double>()));
        }

        // Create Output Directory
        std::string dir_name = in_dir.substr(in_dir.find_last_of("/"));
        sorted_dir = fs::path(out_dir +"/"+ dir_name + "-sorted");
        fs::create_directories(sorted_dir);

        // Read each input file in this dir and run quick sort on them
        for (const auto & entry : fs::directory_iterator(in_dir)) {
            in_path = std::string(entry.path());    // Path to each file
            in_fn = in_path.substr(in_path.find_last_of("/") + 1);  // filename
            sorted_path = fs::path(sorted_dir +"/"+ in_fn);

            // Read File
            if (!q.read_file(in_path)) {
                // If read failed, write empty array to output file and exit
                q.write_file(sorted_path);
                return 0;
            }

            // Run Quick Sort
            q.quick_sort();

            // Get execution time
            exe_times[input_size].push_back(q.get_exe_time());

            // Write Sorted Array
            q.write_file(sorted_path);
        }
    }

    // Write times and averages
    return find_average_and_save_times(out_dir, exe_times);
} 

int find_average_and_save_times(const std::string out_dir, 
    const std::map<int, std::vector<double>> &exe_times) {
    /**
     * Finds the average execution time of all the given files for each input size
     * Writes the execution time and the averages to output directory
     *
     * Parameters:
     *      exe_times (map<int, vector<double>>): Execution times for each input size
     *          (.first = input size)
     *          (.second = array of execution times)
     *
     * Returns:
     *      int :   returns 1 for success
     */

    std::ofstream time_out_file(fs::path(out_dir+"/Azeem_Musa_executionTime.txt"));
    if (!time_out_file) {
        std::cerr << "Error Opening Execution Time Output File" << std::endl;
        return 0;   // return 0 to indicate failure
    }
    time_out_file << "Input Size    Execution Time (ms)" << std::endl;

    std::map<int, double> averages;       // Average exe time for each input size
    double sum = 0;
    int input_size;

    // Repeat for each input size
    for (auto m : exe_times) {
        sum = 0;
        input_size = m.first;
        // Write execution times and find average for each input size
        for (auto exe_time : m.second) {
            sum += exe_time;
            time_out_file << input_size << "    " << exe_time << std::endl;
        }

        // Save average
        if(!averages.count(input_size)) {
            // Create new entry if new input size
            averages.insert(std::pair<int, double>(
                input_size, sum/m.second.size()));
        }
        else {
            // Combine average with previous entry
            double avg = sum/m.second.size();
            averages[input_size] = (averages[input_size] + avg) / 2;
        }
    }
    time_out_file.close();

    // Write Averages to file
    std::ofstream avg_out_file(fs::path(out_dir+"/Azeem_Musa_averageExecutionTime.txt"));
    if (!avg_out_file) {
        std::cerr << "Error Opening Average Output File" << std::endl;
        return 0;   // return 0 to indicate failure
    }

    avg_out_file << "Input Size    Average Execution Time (ms)" << std::endl;
    for (auto m : averages) {
        avg_out_file << m.first << "    " << m.second << std::endl;
    }
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

double QuickSort::get_exe_time() const{
    /**
     * This function calculates and returns the execution time in milliseconds
     *      of the most recent quick sort algorithm that was run
     * 
     * Returns:
     *      (double)    :   execution time in milliseconds
     */

    return (end_time - start_time).count()/1000.0;
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