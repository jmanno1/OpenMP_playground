#include<iostream>
#include<string>
#include<chrono>
#include<omp.h>
#include"cases.h"

#include"fmt/color.h"

auto const menu = 
	"Choices:\n"
	"\t 1. Parallel code block\n"
	"\t 2. Parallel code block with critical\n"
	"\t 3. Parallel loop - Highly Parallel task\n"
	"\t 4. Parallel loop - Calculate Pi task\n"
	"\t 5. Parallel sections - Merge sort\n"
	"\t 6. Data-Sharing clauses\n"
	"\t 7. Single / Master\n"
	"\t 8. Barrier / Nowait\n"
	"\t-1. Quit this program\n\n";


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
			fmt::print("\n*** Parallel sections - Merge sort ***\n\n");
			parallel_sections_merge_sort();
			break;
		case 6:
			fmt::print("\n*** Data-Sharing clauses ***\n\n");
			data_sharing_private();
			break;
		case 7:
			fmt::print("\n*** Single / Master ***\n\n");
			single_master();
			break;
		case 8:
			fmt::print("\n*** Barrier / Nowait ***\n\n");
			 barrier_nowait();
			 break;
		case -1:
			fmt::print("\nGood bye!\n");
			return 0;
		default:
			fmt::print("Please enter a valid choice\n");
		}		
	}

}