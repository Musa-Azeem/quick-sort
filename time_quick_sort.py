#!/usr/bin/env python3

import pandas as pd
import seaborn as sns
import sys
import os
import subprocess

INPUT_FILE_GEN_EXE = 'Azeem_Musa_InputFileGenerator'
QUICK_SORT_EXE = 'Azeem_Musa_QuickSort'


def create_input_files(input_files_dir):
    """ Runs c++ InputFileGenerator code to generate 75 input files
        in the given directory

    Args:
        input_files (str): directory to store generated input files
    """

    # make directory
    if os.path.isdir(input_files_dir):
        subprocess.run(['rm', '-rf', input_files_dir])
    subprocess.run(['mkdir', input_files_dir])
    
    # compile c++ code
    subprocess.run(['make', INPUT_FILE_GEN_EXE])

    # Run InputFileGenerator
    subprocess.run([f'./{INPUT_FILE_GEN_EXE}', input_files_dir])
    

if __name__ == '__main__':
    # create output file directory
    if len(sys.argv) != 0:
        print("Usage: ./time_quick_sort.py")
    
    input_files_dir = '__input-files__'
    create_input_files(input_files_dir)