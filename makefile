# compiler
cc := g++

# source code
quick_sort_src := Azeem_Musa_QuickSort.cpp
num_gen_src := Azeem_Musa_InputFileGenerator.cpp
exec_time_src := average_execution_time.py


# final executables
quick_sort_exe := Azeem_Musa_QuickSort
num_gen_exe := Azeem_Musa_InputFileGenerator

# compile flags
flags := -std=c++17 -Wall

# compile command
compile.cc = $(cc) $(flags) $^ -o $@

# make
$(quick_sort_exe): $(quick_sort_src)
	$(compile.cc)

$(num_gen_exe): $(num_gen_src)
	$(compile.cc)

clean:
	rm -r $(quick_sort_exe) $(num_gen_exe)
