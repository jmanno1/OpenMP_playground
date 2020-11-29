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
	"\t3. Parallel loop - Highly Parallel task\n"
	"\t4. Parallel loop - Calculate Pi task\n\n";


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
			fmt::print("\n*** Parallel block example ***\n\n");
			parallel_block();
			
			break;
		case 2:
			fmt::print("\n*** Parallel block with Critical example ***\n\n");
			parallel_block_critical();
			//
			break;
		case 3:
			fmt::print("\n*** Parallel loop - Highly Parallel task ***\n\n");
			parallel_loop_task();
			break;
		case 4:
			fmt::print("\n*** Parallel loop - Calculate Pi task ***\n\n");
			parallel_loop_pi();
			break;
		case 5:
			//
			break;
		default:
			fmt::print("Please enter a valid choice\n");
		}		
	}

}