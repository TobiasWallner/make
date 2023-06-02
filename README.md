make
====

`make` lets you have `make.txt` file multiple batch-file targets in one file. 
Make searches for the top level `make.txt` before execution. So even if you are in a 
subdirectory you may invoke make to compile or test your code. 

invoke just `make` to get a list of all targets.
invoke `make --version` returns the version of this program.
invoke `make [list of targets]` executes all batch scripts to all listed targets

What does a make file look like?
--------------------------------

A make file has two different components.
1. a target (no indent)
2. batch commands to that target (indent)

A `make.txt` file may look like the following:
```
release
	g++ main.cpp -o main_release.exe -O3 -std=c++20
	
debug
	echo g++ main.cpp -o main_debug.exe -O0 -g -std=c++20
	
all
	make release
	make debug
```

Compile
-------

Compile with C++20 for example with GCC:

```
g++ main.cpp -o make.exe -O3 -std=c++20
```

