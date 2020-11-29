#include<iostream>
#include<string>
#include<chrono>
#include<vector>
#include<numeric>
#include<algorithm>
#include <random>
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
	fmt::print(fg(fmt::color::yellow), "Operations complete with threading in {} milliseconds\n\n", ms.count());
}

// case 4
void parallel_loop_pi() {
	long constexpr num_steps = 100000000;
	double constexpr step = 1.0 / num_steps;

	fmt::print(fg(fmt::color::yellow), "Without Parallel loops (Single thread mode):\n");
	double pi, x, d_sum = 0.0;
	auto tp_1 = high_resolution_clock::now();
	for (int i = 0; i < num_steps; i++) {
		x = (i + 0.5) * step;
		d_sum += 4.0 / (1.0 + x * x);
	}
	pi = d_sum * step;
	auto tp_2 = high_resolution_clock::now();
	duration<double, std::milli> ms = tp_2 - tp_1;

	fmt::print(fg(fmt::color::yellow), "PI = {:.09}, Time = {} milliseconds\n", pi, ms.count());

	d_sum = 0.0;
	omp_set_num_threads(NUM_THREADS);
	fmt::print(fg(fmt::color::yellow), "With Parallel loops (multi thread mode) using {} threads:\n", NUM_THREADS);

	tp_1 = high_resolution_clock::now();

#pragma omp parallel for reduction(+:d_sum)
	for (int i = 0; i < num_steps; i++) {
		x = (i + 0.5) * step;
		d_sum += 4.0 / (1.0 + x * x);
	}
	pi = d_sum * step;
	tp_2 = high_resolution_clock::now();
	ms = tp_2 - tp_1;

	fmt::print(fg(fmt::color::yellow), "PI = {:.09}, Time = {} milliseconds\n\n", pi, ms.count());
	omp_set_num_threads(1);
}

// case 5
// Original merge sort code taken from: https://www.geeksforgeeks.org/merge-sort/
void merge(int* arr, int l, int m, int r)
{
	int n1 = m - l + 1;
	int n2 = r - m;

	// Create temp arrays
	vector<int> L(n1), R(n2);
	//int L[n1], R[n2];

#pragma omp parallel sections // starts a new team
	{
		// Copy data to temp arrays L[] and R[]
#pragma omp section 
		{
			for (int i = 0; i < n1; i++) {
				L[i] = arr[l + i];
			}
		}

#pragma omp section 
		{
			for (int j = 0; j < n2; j++) {
				R[j] = arr[m + 1 + j];
			}
		}
	}

	// Merge the temp arrays back into arr[l..r]
	// Initial index of first (i), second (j), and merged (k) subarray
	int i = 0;
	int j = 0;
	int k = l;

	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			arr[k] = L[i];
			i++;
		}
		else {
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	// Copy the remaining elements of
	// L[], if there are any
	while (i < n1) {
		arr[k] = L[i];
		i++;
		k++;
	}

	// Copy the remaining elements of
	// R[], if there are any
	while (j < n2) {
		arr[k] = R[j];
		j++;
		k++;
	}
}

void mergeSort(int* arr, int l, int r) {
	if (l >= r) {
		return; //returns recursively
	}
	int m = (l + r - 1) / 2;
#pragma omp parallel sections // starts a new team
	{
#pragma omp section 
		{
			mergeSort(arr, l, m);
		}
#pragma omp section
		{
			mergeSort(arr, m + 1, r);
		}
	}
	merge(arr, l, m, r);
}

void parallel_sections_merge_sort() {
	omp_set_num_threads(NUM_THREADS);
	random_device rd;
	mt19937 g(rd());

	// creating random buffer
	vector<int> v(BUF_LEN);
	std::iota(v.begin(), v.end(), 1);
	shuffle(v.begin(), v.end(), g);

	// Print buffer details - before sort
	fmt::print(fg(fmt::color::yellow), "Buffer size to sort: {}\nBefore Sorting - First and last five\n", BUF_LEN);

	for_each_n(v.begin(), 5, [](auto i) {fmt::print("{} ", i); });
	fmt::print("\n...\n");
	for_each_n(v.end() -5, 5, [](auto i) {fmt::print("{} ", i); });

	// sort
	auto tp_1 = high_resolution_clock::now();
	mergeSort(v.data(), 0, v.size() - 1);
	sort(v.begin(), v.end());
	auto tp_2 = high_resolution_clock::now();
	duration<double, std::milli> ms = tp_2 - tp_1;

	// Print buffer details - after sort
	fmt::print(fg(fmt::color::yellow), "\n\nAfter Sorting - First and last five\n");
	for_each_n(v.begin(), 5, [](auto i) {fmt::print("{} ", i); });
	fmt::print("\n...\n");
	for_each_n(v.end()-5, 5, [](auto i) {fmt::print("{} ", i); });
	fmt::print(fg(fmt::color::yellow), "\nParallel merge time = {} milliseconds\n\n", ms.count());
	omp_set_num_threads(1);
}

// case 6
void data_sharing_private() {
	omp_set_num_threads(NUM_THREADS);

	int i = -1;
	fmt::print(fg(fmt::color::yellow), "'i' before: {}\n", i);

#pragma omp parallel for private(i)
		for (int x = 0; x < 5; x++) {
			i = omp_get_thread_num();
		}
	// i equals -1 still.
	fmt::print(fg(fmt::color::yellow), "'i' after 'private': {}\n", i);

#pragma omp parallel for lastprivate(i)
		for (int x = 0; x < 5; x++) {
			i = omp_get_thread_num();
		}

	// will be the value of the last thread to use 'i'
		fmt::print(fg(fmt::color::yellow), "'i' after 'lastprivate': {}\n\n", i);
		omp_set_num_threads(1);
}

//case 7
void single_master() {

#pragma omp parallel num_threads(NUM_THREADS)
	{
#pragma omp critical 
		fmt::print(fg(fmt::color::yellow),
			"All threads print. Thread: {}\n",
			omp_get_thread_num());

#pragma omp single
		fmt::print(fg(fmt::color::light_green),
			"\nOne thread prints. Thread: {}\n\n",
			omp_get_thread_num());

	// implicit barrier wait for single, no wait on master

#pragma omp master
		fmt::print(fg(fmt::color::light_green),
			"\nMaster thread prints. Thread: {}\n\n",
			omp_get_thread_num());

#pragma omp critical 
		fmt::print(fg(fmt::color::yellow),
			"All threads print. Thread: {}\n",
			omp_get_thread_num());
	}
	fmt::print("\n");
}