#include <chrono>
#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>

#define CACHE_LINE_SIZE 64

#ifndef REPETITIONS
    #define REPETITIONS 100 * 1024 * 1024
#endif

using Type = uint32_t;

void test_memory(std::vector<Type*>& memory)
{
    using Clock = std::chrono::steady_clock;

    size_t size = memory.size();
    auto start = Clock::now();

    for (int i = 0; i < REPETITIONS; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            *memory[j] += j;
        }
    }

    std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count() << std::endl;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: misaligned-access <offset>" << std::endl;
        return 1;
    }

    int offset = std::stoi(argv[1]);

    Type* memory;

    // allocated two cache lines
    size_t size = CACHE_LINE_SIZE * 2;
#ifdef _MSC_VER
    memory = _aligned_malloc(size, CACHE_LINE_SIZE);
#else
    if (posix_memalign((void**) &memory, CACHE_LINE_SIZE, size))
    {
        std::cerr << "Couldn't allocate memory" << std::endl;
        return 1;
    }
#endif
    assert((reinterpret_cast<uintptr_t>(memory) & 63) == 0);

    // pointer is kept in vector to prevent the compiler from optimizing the writes to a register
    std::vector<Type*> pointers(1, reinterpret_cast<Type*>(reinterpret_cast<char*>(memory) + offset));
    test_memory(pointers);

#ifdef _MSC_VER
    _aligned_free(memory);
#else
    free(memory);
#endif

    return 0;
}
