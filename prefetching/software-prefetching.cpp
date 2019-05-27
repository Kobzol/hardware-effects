#include <iostream>
#include <vector>
#include <chrono>
#include <memory>
#include <random>
#include <algorithm>
#include <cstring>
#include <xmmintrin.h>

#if defined(__clang__) || defined(__INTEL_COMPILER)
using _mm_hint = uint8_t;
#endif

#ifndef REPETITIONS
    #define REPETITIONS 5
#endif

#ifndef SIZE
    #define SIZE 10 * 1024 * 1024
#endif

struct Data
{
    uint8_t data[64];
};

using Type = Data;

template <bool Prefetch, _mm_hint Hint>
size_t test_memory(const std::vector<Type*>& memory, int distance)
{
    size_t sum = 0;
    size_t size = memory.size();

    using Clock = std::chrono::steady_clock;
    auto start = Clock::now();

    for (int i = 0; i < REPETITIONS; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            if (Prefetch) _mm_prefetch(memory[j + distance], Hint);
            memory[j]->data[0] += sum;
            sum += memory[j]->data[5];
            memory[j]->data[10] += memory[j]->data[5];
            memory[j]->data[5] *= sum / 2;
            memory[j]->data[10] += memory[j]->data[5];
        }
    }

    std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count() << std::endl;
    return sum;
}

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        std::cout << "Usage: software-prefetching <prefetch> <distance> <hint>" << std::endl;
        return 1;
    }

    std::default_random_engine rng;
    rng.seed(0);

    auto* data = new Type[SIZE]();
    std::vector<Type*> pointers;
    for (int i = 0; i < SIZE; i++)
    {
        std::memset(&data[i].data, i, sizeof(Data));
        pointers.push_back(data + i);
    }

    std::shuffle(pointers.begin(), pointers.end(), rng);

    int prefetch = std::stoi(argv[1]);
    int distance = std::stoi(argv[2]);
    _mm_hint hint = (_mm_hint) std::stoi(argv[3]);

    size_t sum = 0;

    if (prefetch)
    {
        if (hint == _MM_HINT_T0) sum = test_memory<true, _MM_HINT_T0>(pointers, distance);
        if (hint == _MM_HINT_T1) sum = test_memory<true, _MM_HINT_T1>(pointers, distance);
        if (hint == _MM_HINT_T2) sum = test_memory<true, _MM_HINT_T2>(pointers, distance);
        if (hint == _MM_HINT_NTA) sum = test_memory<true, _MM_HINT_NTA>(pointers, distance);
    }
    else
    {
        sum = test_memory<false, _MM_HINT_T0>(pointers, distance);
    }

    std::cout << "Sum: " << sum << std::endl;

    delete[] data;

    return 0;
}
