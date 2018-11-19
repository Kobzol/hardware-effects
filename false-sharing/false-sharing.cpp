#include <iostream>
#include <chrono>
#include <thread>
#include <cassert>
#include <vector>
#include <cstring>

#define REPEAT 1024 * 1024 * 100UL

using Type = size_t;

void thread_fn(int tid, int increment, size_t repeat, Type* memory, int size)
{
    int target = tid * increment;

    for (size_t i = 0; i < repeat; i++)
    {
        // force memory access, memory clobber or assembly can also help
        std::memset(memory + target, i, size);
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
#ifdef __linux__
    // try to allocate 64-byte aligned memory to make sure that we start on a cache line beginning
    if (posix_memalign((void**) &memory, 64, totalSize))
    {
        std::cerr << "Couldn't allocate memory" << std::endl;
        return 1;
    }
    assert(((uintptr_t)memory & 0x3F) == 0);
#else
    memory = static_cast<size_t*>(malloc(totalSize));
#endif

    using Clock = std::chrono::system_clock;
    auto start = Clock::now();

    std::vector<std::thread> threads;
    for (int i = 0; i < threadCount; i++)
    {
        threads.emplace_back(thread_fn, i, increment, REPEAT, memory, sizeof(Type));
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    std::cout << memory[0] << std::endl;
    std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count() << std::endl;

    free(memory);

    return 0;
}
