#include <iostream>
#include <chrono>
#include <thread>
#include <cassert>
#include <vector>
#include <cstring>

#ifndef REPETITIONS
    #define REPETITIONS 1024 * 1024 * 256UL
#endif

using Type = size_t;

void thread_fn(int tid, int increment, Type* memory, int size)
{
    int target = tid * increment;

    for (size_t r = 0; r < REPETITIONS; r++)
    {
        for (int i = 0; i < size; i++)
        {
            memory[target + i]++;
        }
    }
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cout << "Usage: false-sharing <thread-count> <increment>" << std::endl;
        return 1;
    }

    int threadCount = std::stoi(argv[1]);
    int increment = std::stoi(argv[2]);

    size_t totalSize = threadCount * increment * sizeof(size_t);

    size_t* memory;
#ifdef _MSC_VER
    memory = _aligned_malloc(totalSize, 64);
#else
    // try to allocate 64-byte aligned memory to make sure that we start on a cache line boundary
    if (posix_memalign((void**) &memory, 64, totalSize))
    {
        std::cerr << "Couldn't allocate memory" << std::endl;
        return 1;
    }
    assert(((uintptr_t)memory & 0x3F) == 0);
#endif

    using Clock = std::chrono::system_clock;
    auto start = Clock::now();

    std::vector<std::thread> threads;
    for (int i = 0; i < threadCount; i++)
    {
        threads.emplace_back(thread_fn, i, increment, memory, 1);
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count() << std::endl;

#ifdef _MSC_VER
    _aligned_free(memory);
#else
    free(memory);
#endif

    return 0;
}
