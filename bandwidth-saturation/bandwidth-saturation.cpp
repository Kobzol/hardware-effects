#include <iostream>
#include <chrono>
#include <thread>
#include <cassert>
#include <vector>
#include <cstring>
#include <xmmintrin.h>

#ifndef COUNT
    #define COUNT 16 * 1024 * 1024
#endif

#ifndef REPETITIONS
#define REPETITIONS 20
#endif

using Type = size_t;

template <bool NonTemporal>
void thread_fn(Type* items, size_t size)
{
    for (int i = 0; i < REPETITIONS; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            if (NonTemporal)
            {
                _mm_stream_pi((__m64*) items + j, reinterpret_cast<__m64>(static_cast<Type>(j)));
            }
            else items[j] = j;
        }
    }
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: bandwidth-saturation <non-temporal> <thread-count>" << std::endl;
        return 1;
    }

    int nonTemporal = std::stoi(argv[1]);
    int threadCount = std::stoi(argv[2]);

    std::vector<std::unique_ptr<Type[]>> arrays;
    for (int i = 0; i < threadCount; i++)
    {
        arrays.push_back(std::unique_ptr<Type[]>(new Type[COUNT]()));
    }

    using Clock = std::chrono::system_clock;
    auto start = Clock::now();

    std::vector<std::thread> threads;
    for (int i = 0; i < threadCount; i++)
    {
        if (nonTemporal == 1)
        {
            threads.emplace_back(thread_fn<true>, arrays[i].get(), COUNT);
        }
        else threads.emplace_back(thread_fn<false>, arrays[i].get(), COUNT);
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count() << std::endl;

    return 0;
}
