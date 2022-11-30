# compiler
cc := g++

# source code
quick_sort_src := Azeem_Musa_QuickSort.cpp
num_gen_src := InputFileGenerator.cpp

src := $(quick_sort_src) $(num_gen_src)

# final executables
quick_sort_exe := Azeem_Musa_QuickSort
num_gen_exe := InputFileGenerator
exe := $(quick_sort_exe) $(num_gen_exe)

# compile flags
flags := -std=c++17

# compile command
compile.cc = $(cc) $(flags) $^ -o $@

input_files_dir := _input_files
user_in := _user_input.txt

# make
$(quick_sort_exe): $(quick_sort_src)
	$(compile.cc)

$(num_gen_exe): $(num_gen_src)
	$(compile.cc)

run: $(exe)
	rm -rf $(input_files_dir)
	echo "$(input_files_dir)/10 10 n $(input_files_dir)/100 100 n $(input_files_dir)/1000 1000 y" > $(user_in)
	./$(num_gen_exe) $(input_files_dir)
	./$(quick_sort_exe) < $(user_in)
	make clean_all

clean:
	rm -rf $(exe)

clean_all: clean
	rm -rf $(input_files_dir) $(user_in)