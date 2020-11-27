#include<iostream>
#include<string>
#include<chrono>
#include<omp.h>
#include"cases.h"

#include"fmt/color.h"

auto const menu = 
	"Choices:\n"
	"\t1. Parallel code block\n"
	"\t2. Parallel code block with critical\n"
	"\t3. etc\n\n";


using namespace std;


int main() {
	string choice;
	int item = 0;

	fmt::print(fmt::emphasis::bold, "OpenMP test program\n\n");

	while (1) {
		fmt::print(fg(fmt::color::light_green), menu);
		fmt::print("Selection> ");

		if (!std::getline(std::cin, choice)) {
			cin.clear();
			cin.ignore();
			continue;
		}

		item = stoi(choice);
		switch (item)
		{
		case 1:
			cout << "\n*** Parallel block example ***\n\n";
			parallel_block();
			
			break;
		case 2:
			cout << "\n*** Parallel block with Critical example ***\n\n";
			parallel_block_critical();
			//
			break;
		case 3:
			//
			break;
		case 4:
			//
			break;
		case 5:
			//
			break;
		default:
			cout << "Please enter a valid choice\n";
		}

		
	}

}