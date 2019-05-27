#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <cassert>
#include <vector>
#include <cstring>
#include <immintrin.h>

#ifndef ARRAY_SIZE
    #define ARRAY_SIZE 2 * 1024 * 1024
#endif

#ifndef REPETITIONS
    #define REPETITIONS 20
#endif

using Type = size_t;
using Clock = std::chrono::steady_clock;
using Regions = std::vector<std::pair<int, int>>;

Regions createWriteRegions(int arrayCount, int writeIncrement)
{
    Regions regions;

    int start = 0;
    while (start < arrayCount)
    {
        regions.emplace_back(start, std::min(arrayCount, start + writeIncrement));
        start += writeIncrement;
    }

    return regions;
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cout << "Usage: write-combining <array-count> <write-increment>" << std::endl;
        return 1;
    }

    int arrayCount = std::stoi(argv[1]);
    int writeIncrement = std::min(arrayCount, std::stoi(argv[2]));

    Regions writeRegions = createWriteRegions(arrayCount, writeIncrement);

    std::vector<std::unique_ptr<Type[]>> arrays;
    for (int i = 0; i < arrayCount; i++)
    {
        arrays.emplace_back(new Type[ARRAY_SIZE]());
    }

    auto start = Clock::now();

    for (int r = 0; r < REPETITIONS; r++)
    {
        for (auto& region: writeRegions)
        {
            for (int l = 0; l < ARRAY_SIZE; l++)
            {
                for (int i = region.first; i < region.second; i++)
                {
                    // non-temporal store, bypasses cache
                    _mm_stream_si64(reinterpret_cast<long long *>(&arrays[i][l]), static_cast<Type>(l));
                }
            }
        }
    }

    std::cerr << std::setw(5) << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count() << std::endl;

    return 0;
}
