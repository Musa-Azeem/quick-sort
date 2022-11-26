#!/usr/bin/env python3

"""
    Author :     Musa Azeem
    Date   :     12/2/2022
    File   :     time_quick_sort.py

    This python script runs C++ code to:
     - generate input files of different sizes
     - run the quick sort algorithm of each size
     - record the execution time for each input file
     - save and plot the average execution time for each input size in the
       given directory

    Usage: ./time_quick_sort.py [output directory]


    Output Files:
     - Saves each execution time in an ASCII tab seperated file 
       "execution_time.txt" in the format:
       [input size] \t [Execution Time]
    - Saves the average execution time for each input size in an ASCII
      tab seperated file
      "average_execution_time.txt in the format:
      [input size] \t [Average Execution Time]
    - Generates and saves a plot of the average execution times in a
      file "plot_average_execution_time.jpg" in the jpg format,
      with x being the input size and y being the average execution time
      in milliseconds
"""

import pandas as pd
import seaborn as sns
import sys
import os
import subprocess
import math

INPUT_FILE_GEN_EXE = 'Azeem_Musa_InputFileGenerator'
QUICK_SORT_EXE = 'Azeem_Musa_QuickSort'
FILES_DIR = '__time_quick_sort_files__'
INPUT_FILES_DIR = f'{FILES_DIR}/input-files'
OUTPUT_FILES_DIR_10 = f'{FILES_DIR}/output-files/10'
OUTPUT_FILES_DIR_100 = f'{FILES_DIR}/output-files/100'
OUTPUT_FILES_DIR_1000 = f'{FILES_DIR}/output-files/1000'
OUTPUT_EXE_TIMES_FN = "Azeem_Musa_executionTime.txt"
OUTPUT_AVG_EXE_TIMES_FN = "Azeem_Musa_averageExecutionTime.txt"
OUTPUT_PLOT_FN = "Azeem_Musa_plotAverageExecutionTime.jpg"



def setup(final_output_dir):
    """ 
        Sets up working directory
         - Creates new working directory to store input and output files
           of C++ code
         - Compiles C++ code
    """

    # Remove directories if they exist
    subprocess.run(['rm', '-rf', FILES_DIR])
    subprocess.run(['rm', '-rf', final_output_dir])

    # Make directories
    subprocess.run(['mkdir', '-p', INPUT_FILES_DIR])
    subprocess.run(['mkdir', '-p', OUTPUT_FILES_DIR_10])
    subprocess.run(['mkdir', '-p', OUTPUT_FILES_DIR_100])
    subprocess.run(['mkdir', '-p', OUTPUT_FILES_DIR_1000])
    subprocess.run(['mkdir', '-p', final_output_dir])
    
    # Compile C++ code
    subprocess.run(['make', INPUT_FILE_GEN_EXE], stdout=subprocess.DEVNULL)
    subprocess.run(['make', QUICK_SORT_EXE], stdout=subprocess.DEVNULL)


def create_input_files():
    """ 
    Runs C++ InputFileGenerator code to generate 75 input files
        in the given directory
    """

    # Run C++ : Create input files
    subprocess.run([f'./{INPUT_FILE_GEN_EXE}', INPUT_FILES_DIR])

def run_quick_sort():
    """ 
        Runs C++ QuickSort code to sort and time the 75 generated input files
         - Saves output of each 25 files of the same length in directories
    """

    # Run C++ : Run quick sort and record execution time
    # Run quick sort on each file
    ofn = ""
    for i,file in enumerate(os.listdir(INPUT_FILES_DIR)):
        ofn = file.split(".")[0]
        if file.startswith('10-'):
            # Files with this name contain 10 values
            subprocess.run([
                f'./{QUICK_SORT_EXE}',
                os.path.join(INPUT_FILES_DIR, file), 
                os.path.join(OUTPUT_FILES_DIR_10,f'{ofn}.txt')])
        elif file.startswith('100-'):
            # Files with this name contain 100 values
            subprocess.run([
                f'./{QUICK_SORT_EXE}',
                os.path.join(INPUT_FILES_DIR, file), 
                os.path.join(OUTPUT_FILES_DIR_100,f'{ofn}.txt')])
        elif file.startswith('1000-'):
            # Files with this name contain 1000 values
            subprocess.run([
                f'./{QUICK_SORT_EXE}',
                os.path.join(INPUT_FILES_DIR, file), 
                os.path.join(OUTPUT_FILES_DIR_1000,f'{ofn}.txt')])


def process_exe_times():
    """
        Read the quicksort execution times for each input size into a dataframe

    Returns:
        DataFrame: Execution times dataframe
    """

    df = pd.DataFrame(columns=["input size", "execution time (ms)"])

    exe_time_millis = 0

    # Read 10 input size files
    for file in os.listdir(OUTPUT_FILES_DIR_10):
        # only read execution time files
        if "exe_time_millis" in file:
            exe_time_millis = float(open(os.path.join(OUTPUT_FILES_DIR_10, file)).read().splitlines()[0])
            df.loc[len(df)] = [10, exe_time_millis]

    # Read 100 input size files
    for file in os.listdir(OUTPUT_FILES_DIR_100):
        # only read execution time files
        if "exe_time_millis" in file:
            exe_time_millis = float(open(os.path.join(OUTPUT_FILES_DIR_100, file)).read().splitlines()[0])
            df.loc[len(df)] = [100, exe_time_millis]

    # Read 1000 input size files
    for file in os.listdir(OUTPUT_FILES_DIR_1000):
        # only read execution time files
        if "exe_time_millis" in file:
            exe_time_millis = float(open(os.path.join(OUTPUT_FILES_DIR_1000, file)).read().splitlines()[0])
            df.loc[len(df)] = [1000, exe_time_millis]

    return df


def save_and_plot(df: pd.DataFrame, final_output_dir):
    """
        Saves execution times for each file and the average executions times
            for each input size
        Generates and saves plot of average execution times

    Args:
        df (DataFrame): Execution times dataframe
        final_output_dir (str): directory to save output files
    """

    # Save execution time for each file
    with open(os.path.join(final_output_dir, OUTPUT_EXE_TIMES_FN), 'w+') as f:
        f.write(df.to_csv(sep='\t', 
                          header=['Input Size', 'Execution Time (ms)'],
                          index=False).replace('\t', '    '))

    # Save average execution time for each input size
    ave_exe_time = {'input size': [10, 100,1000], 
                    'average execution time (ms)': [df[df['input size'] == 10]['execution time (ms)'].mean(),
                                                    df[df['input size'] == 100]['execution time (ms)'].mean(),
                                                    df[df['input size'] == 1000]['execution time (ms)'].mean()]}
    df_avg = pd.DataFrame(ave_exe_time)

    with open(os.path.join(final_output_dir, OUTPUT_AVG_EXE_TIMES_FN), 'w+') as f:
        f.write(df_avg.to_csv(sep='\t', 
                              header=['Input Size', 'Average Execution Time (ms)'],
                              index=False).replace('\t', '    '))
    
    # Plot Average Execution Time
    plot = sns.lineplot(data=df_avg, x='input size', y='average execution time (ms)')
    plot.set_xticks(range(0, 1001, 100))
    plot.set_yticks(range(0, math.ceil(max(df_avg['average execution time (ms)']))+10, 10))
    plot.set_xlabel('Input Size')
    plot.set_ylabel('Average Execution Time (ms)')
    plot.set_title('Average Execution Time for Quick Sort')
    plot.get_figure().savefig(os.path.join(final_output_dir, OUTPUT_PLOT_FN))
    

# Entry point
if __name__ == '__main__':
    # create output file directory
    if len(sys.argv) != 2:
        print("Usage: ./time_quick_sort.py [output directory]")
        exit(1)

    final_output_dir = sys.argv[1]

    # Setup up working directory
    setup(final_output_dir)

    # Run C++ : Create input files and run/time quick sort
    create_input_files()
    run_quick_sort()

    # Process and plot execution times
    df = process_exe_times()
    save_and_plot(df, final_output_dir)