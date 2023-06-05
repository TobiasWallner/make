

//C++ std
#include <cstdlib>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string_view>
#include <cctype>
#include <filesystem>

using Line = std::string;
using Target = std::vector<Line>;
using File = std::vector<Target>;

bool empty_line(const Line& line){
	if(line.empty()){
		return true;
	}else{
		for(char c : line){
			if(!std::isblank(c)){
				return false;
			}
		}
		return true;
	}
}

void trim_front(std::string& str){
	auto itr = str.begin();
	for(; itr != str.end(); ++itr){
		if(!std::isblank(*itr)){
			break;
		}
	}
	str.erase(str.begin(), itr);
}

File load_file(std::string_view filename){
	File file;
	std::ifstream fstream("make.txt");
	if(!fstream.is_open()){
		std::cout << "could not opend make file" << std::endl;
		exit(-1);
	}else{
		File::iterator insertTarget;
		while(!fstream.eof()){
			Line line;
			std::getline(fstream, line);
			if(!empty_line(line)){
				if(!std::isblank(line.front())){
					file.emplace_back();
					insertTarget = file.end() - 1;
				}else if(file.empty()){
					std::cout << "Error: you have to define a target first" << std::endl;
					exit(EXIT_FAILURE);
				}
				trim_front(line);
				insertTarget->emplace_back(std::move(line));
			}
			
		}
	}
	return file;
}

void print_all_targets(const File& file){
	// aka all lines that do not start with a whitespace
	for(const Target& para : file){
		std::cout << para.front() << "    ";
	}
}

const Target& find_target(const File& file, std::string_view target){
	for(const Target& para : file){
		if(para.front() == target){
			return para;
		}
	}
	std::cout << "Error: could not find target '" << target << "' in make file" << std::endl;
	exit(-1);
	return file.front();
}

void execute_target(const Target& target){
	for(auto itr = target.begin()+1; itr != target.end(); ++itr){
		if(itr->starts_with("cd ")){
			std::filesystem::current_path(itr->c_str() + sizeof("cd ") - 1);
		}else{
			const int errorLevel = system(itr->c_str());	
			if(errorLevel != 0){
				exit(EXIT_FAILURE);
			}
		}
	}
}

void change_to_top_level_make_dir(std::string_view filename){
	std::filesystem::path homePath = std::filesystem::current_path();
	bool run = true;
	while(run){
		const bool exists = std::filesystem::is_regular_file(std::filesystem::status(std::filesystem::current_path() / filename));
		if(exists){
			return;
		}else{
			auto oldPath = std::filesystem::current_path();
			std::filesystem::current_path("..");
			run = !(oldPath == std::filesystem::current_path());
			oldPath = std::filesystem::current_path();;
		}
	}
	std::cout << "Error: could not find a make file" << std::endl;
	std::filesystem::current_path(homePath);
	exit(EXIT_FAILURE);
}

int main(int argumentCount, char** argumentStrings){	
	--argumentCount;
	++argumentStrings;
	
	if(argumentCount != 0){
		if(std::strcmp(argumentStrings[0], "--version") == 0){
			std::cout << "version 1.2.1" << std::endl;
			return EXIT_SUCCESS;
		}	
	}	
	
	const char* filename = "make.txt";
	
	std::filesystem::path homePath = std::filesystem::current_path();
	change_to_top_level_make_dir(filename);
	std::cout << "found make at: " << (std::filesystem::current_path() / filename).string() << std::endl;
	
	auto file = load_file(filename);
	
	if(argumentCount == 0){
		print_all_targets(file);
	}else{
		for(int i = 0; i != argumentCount; ++i){
			const Target& target = find_target(file, argumentStrings[i]);
			execute_target(target);	
		}		
	}
	
	// return back to home - aka the place this programm was started in
	std::filesystem::current_path(homePath);
	
	return EXIT_SUCCESS;
}