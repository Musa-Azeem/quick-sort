# compiler
cc := g++

# source code
quick_sort_src := Azeem_Musa_QuickSort.cpp
num_gen_src := InputFileGenerator.cpp
exec_time_src := average_execution_time.py


# final executables
quick_sort_exe := Azeem_Musa_QuickSort
num_gen_exe := InputFileGenerator

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
	rm -rf $(quick_sort_exe) $(num_gen_exe)

# python script and submission
output_dir := exe_times
py_exe := time_quick_sort.py
submission := CSCE350Project_Azeem_Musa.zip
sub_files := $(output_dir)/* $(quick_sort_src) $(num_gen_src) makefile README.md

run:
	make clean_all
	mkdir -p $(output_dir)
	chmod +x $(py_exe) 
	./$(py_exe) $(output_dir)

pack:
	zip $(submission) -j $(sub_files)

clean_all:
	rm -rf $(quick_sort_exe) $(num_gen_exe) $(output_dir) $(submission)
