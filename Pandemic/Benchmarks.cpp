#include <stdio.h>
#include "Benchmarks.h"
#include "Tools.h"
#include "Color.h"

#define NUM_ITERATIONS		100000000
Color colors[NUM_ITERATIONS * 2];

void RunBenchmarks()
{
	// Make random data to work with
	for(int i = 0; i < NUM_ITERATIONS; i++)
	{
		colors[i] = Color(static_cast<byte>(Random(0, 255)), static_cast<byte>(Random(0, 255)), static_cast<byte>(Random(0, 255)), static_cast<byte>(Random(0, 255)));
		colors[i + NUM_ITERATIONS] = Color(static_cast<byte>(Random(0, 255)), static_cast<byte>(Random(0, 255)), static_cast<byte>(Random(0, 255)), static_cast<byte>(Random(0, 255)));
	}

	TimePoint t1 = Clock::now();

	// Benchmark blending
	for(int i = 0; i < NUM_ITERATIONS; i++)
	{
		colors[i].Blend(colors[i + NUM_ITERATIONS]);
	}

	TimePoint t2 = Clock::now();

	// Benchmark adding
	for(int i = 0; i < NUM_ITERATIONS; i++)
	{
		colors[i].Add(colors[i + NUM_ITERATIONS]);
	}

	TimePoint t3 = Clock::now();

	// Benchmark modulate
	for(int i = 0; i < NUM_ITERATIONS; i++)
	{
		colors[i].ModulateRGBA(colors[i + NUM_ITERATIONS].a);
	}

	TimePoint t4 = Clock::now();

	std::cout << "Blend duration: " << ch::ToMilliseconds(t2 - t1) << " ms" << std::endl;
	std::cout << "Add duration: " << ch::ToMilliseconds(t3 - t2) << " ms" << std::endl;
	std::cout << "Modulate duration: " << ch::ToMilliseconds(t4 - t3) << " ms" << std::endl;
}
