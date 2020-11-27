#include<iostream>
#include<string>
#include<chrono>
#include<omp.h>

#include"fmt/color.h"

#define NUM_THREADS 10

using namespace std;
using namespace chrono;

void print_a_msg(int value) {
	fmt::print(fg(fmt::color::yellow),
		"Thread# {}. Print a msg, value: {}\n",
		omp_get_thread_num(), value);
}

void count(int& value) {
	value++;
}

//case 1
void parallel_block() {
	int value = 0;

	cout << "\nSetting thread count to " << NUM_THREADS << "\n\n";
	omp_set_num_threads(NUM_THREADS);
#pragma omp parallel
	{
		count(value);
		print_a_msg(value);
	}

	omp_set_num_threads(1);

	cout << "\n\nChose option 2 (critical) to see a solution to bad output\n\n";
}

//case 2
void parallel_block_critical() {
	int value = 0;

	cout << "\nSetting thread count to " << NUM_THREADS << "\n\n";
	omp_set_num_threads(NUM_THREADS);
#pragma omp parallel
	{
#pragma omp critical
		{
			count(value);
			print_a_msg(value);
		}
	}

	omp_set_num_threads(1);

	cout << "\n\nCritical selection enables synchronization on resources (I/O, etc)\n\n";
}

void parallel_loop_example() {
	long constexpr num_steps = 100000000;
	double constexpr step = 1.0 / num_steps;

	cout << "Without Parallel loops (Single thread mode): ";
	double pi, x, d_sum = 0.0;
	auto tp_1 = high_resolution_clock::now();
	for (int i = 0; i < num_steps; i++) {
		x = (i + 0.5) * step;
		d_sum += 4.0 / (1.0 + x * x);
	}
	pi = d_sum * step;
	auto tp_2 = high_resolution_clock::now();
	duration<double, std::micro> ms = tp_2 - tp_1;

	printf("PI = %0.9f, Time = %f microseconds\n", pi, ms.count());
	d_sum = 0.0;
	omp_set_num_threads(NUM_THREADS);
	cout << "With Parallel loops (multi thread mode) << using " << NUM_THREADS << " threads: ";

	tp_1 = high_resolution_clock::now();
#pragma omp parallel for reduction(+:d_sum)
	for (int i = 0; i < num_steps; i++) {
		x = (i + 0.5) * step;
		d_sum += 4.0 / (1.0 + x * x);
	}
	pi = d_sum * step;
	tp_2 = high_resolution_clock::now();
	ms = tp_2 - tp_1;

	printf("PI = %0.9f, Time = %f microseconds\n\n", pi, ms.count());
	omp_set_num_threads(1);
}