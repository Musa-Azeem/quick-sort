#!/usr/bin/env python3

import pandas as pd
import seaborn as sns
import sys
import os
import subprocess

INPUT_FILE_GEN_EXE = 'Azeem_Musa_InputFileGenerator'
QUICK_SORT_EXE = 'Azeem_Musa_QuickSort'
FILES_DIR = '__time_quick_sort_files__'
INPUT_FILES_DIR = f'{FILES_DIR}/input-files'
OUTPUT_FILES_DIR_10 = f'{FILES_DIR}/output-files/10'
OUTPUT_FILES_DIR_100 = f'{FILES_DIR}/output-files/100'
OUTPUT_FILES_DIR_1000 = f'{FILES_DIR}/output-files/1000'



def setup():
    """ 
        Sets up working directory
         - Creates new working directory to store input and output files
           of C++ code
         - Compiles C++ code
    """
    # Make directories
    if os.path.isdir(FILES_DIR):
        subprocess.run(['rm', '-rf', FILES_DIR])
    subprocess.run(['mkdir', '-p', INPUT_FILES_DIR])
    subprocess.run(['mkdir', '-p', OUTPUT_FILES_DIR_10])
    subprocess.run(['mkdir', '-p', OUTPUT_FILES_DIR_100])
    subprocess.run(['mkdir', '-p', OUTPUT_FILES_DIR_1000])
    
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
    for i,file in enumerate(os.listdir(INPUT_FILES_DIR)):
        if file.startswith('10-'):
            # Files with this name contain 10 values
            subprocess.run([
                f'./{QUICK_SORT_EXE}',
                os.path.join(INPUT_FILES_DIR, file), 
                os.path.join(OUTPUT_FILES_DIR_10,f'{file[3]}.txt')])
        elif file.startswith('100-'):
            # Files with this name contain 100 values
            subprocess.run([
                f'./{QUICK_SORT_EXE}',
                os.path.join(INPUT_FILES_DIR, file), 
                os.path.join(OUTPUT_FILES_DIR_100,f'{file[4]}.txt')])
        elif file.startswith('1000-'):
            # Files with this name contain 1000 values
            subprocess.run([
                f'./{QUICK_SORT_EXE}',
                os.path.join(INPUT_FILES_DIR, file), 
                os.path.join(OUTPUT_FILES_DIR_1000,f'{file[5]}.txt')])


if __name__ == '__main__':
    # create output file directory
    if len(sys.argv) != 0:
        print("Usage: ./time_quick_sort.py")

    # Setup up working directory
    setup()

    # Run C++ : Create input files and run/time quick sort
    create_input_files()
    run_quick_sort()

    # Read into pandas