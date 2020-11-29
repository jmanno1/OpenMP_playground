#include<iostream>
#include<string>
#include<chrono>
#include<vector>
#include<numeric>
#include<omp.h>

#include"fmt/color.h"

#define NUM_THREADS 10
constexpr int BUF_LEN = 1024 * 1024;

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

	fmt::print("\nSetting thread count to {}\n\n", NUM_THREADS);
	omp_set_num_threads(NUM_THREADS);
#pragma omp parallel
	{
		count(value);
		print_a_msg(value);
	}

	omp_set_num_threads(1);

	fmt::print("\n\nChose option 2 (critical) to see a solution to bad output\n\n");
}

//case 2
void parallel_block_critical() {
	int value = 0;

	fmt::print("\nSetting thread count to {}\n\n", NUM_THREADS);
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

	fmt::print("\n\nCritical selection enables synchronization on resources (I/O, etc)\n\n");
}

//case 3
void parallel_loop_task() {
	vector<long long> a(BUF_LEN), b(BUF_LEN), c(BUF_LEN);

	// intialize source buffers
	std::iota(b.begin(), b.end(), 1);
	std::iota(c.begin(), c.end(), 1);

	auto tp_1 = high_resolution_clock::now();
	for (int i = 0; i < a.size(); i++) {
		a[i] = b[i] * c[i];
	}
	auto tp_2 = high_resolution_clock::now();
	duration<double, std::milli> ms = tp_2 - tp_1;
	fmt::print(fg(fmt::color::yellow), "Operations complete, single threaded, in {} milliseconds\n", ms.count());

	// intialize source buffers
	std::iota(b.begin(), b.end(), 1);
	std::iota(c.begin(), c.end(), 1);

	omp_set_num_threads(NUM_THREADS);
	tp_1 = high_resolution_clock::now();
#pragma omp parallel for
	for (int i = 0; i < a.size(); i++) {
		a[i] = b[i] * c[i];
	}
	tp_2 = high_resolution_clock::now();
	omp_set_num_threads(1);

	ms = tp_2 - tp_1;
	fmt::print(fg(fmt::color::yellow), "Operations complete with threading in {} milliseconds\n", ms.count());
}

// case 4
void parallel_loop_pi() {
	long constexpr num_steps = 100000000;
	double constexpr step = 1.0 / num_steps;

	fmt::print(fg(fmt::color::yellow), "Without Parallel loops (Single thread mode): ");
	double pi, x, d_sum = 0.0;
	auto tp_1 = high_resolution_clock::now();
	for (int i = 0; i < num_steps; i++) {
		x = (i + 0.5) * step;
		d_sum += 4.0 / (1.0 + x * x);
	}
	pi = d_sum * step;
	auto tp_2 = high_resolution_clock::now();
	duration<double, std::micro> ms = tp_2 - tp_1;

	fmt::print(fg(fmt::color::yellow), "PI = {:.09}, Time = {} microseconds\n", pi, ms.count());
	d_sum = 0.0;
	omp_set_num_threads(NUM_THREADS);
	fmt::print(fg(fmt::color::yellow), "With Parallel loops (multi thread mode) using {} threads: ", NUM_THREADS);

	tp_1 = high_resolution_clock::now();

#pragma omp parallel for reduction(+:d_sum)
	for (int i = 0; i < num_steps; i++) {
		x = (i + 0.5) * step;
		d_sum += 4.0 / (1.0 + x * x);
	}
	pi = d_sum * step;
	tp_2 = high_resolution_clock::now();
	ms = tp_2 - tp_1;

	fmt::print(fg(fmt::color::yellow), "PI = {:.09}, Time = {} microseconds\n", pi, ms.count());
	omp_set_num_threads(1);
}