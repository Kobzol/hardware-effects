#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <immintrin.h>

#ifndef REPETITIONS
    #define REPETITIONS 100
#endif

#ifndef SIZE
    #define SIZE 4 * 1024 * 1024
#endif

using Type = float;

void test_memory(std::vector<Type>& memory, Type value)
{
    size_t size = memory.size();

    using Clock = std::chrono::steady_clock;
    auto start = Clock::now();

    for (int i = 0; i < REPETITIONS; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            memory[j] *= value;
        }
    }

    std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count() << std::endl;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: denormals <flush> <value>" << std::endl;
        return 1;
    }

    int flush = std::stoi(argv[1]);
    Type value = static_cast<Type>(std::stod(argv[2]));

    if (flush == 1)
    {
        _mm_setcsr(_mm_getcsr() | 0x8040);
    }

    std::vector<Type> data(SIZE, 1);
    test_memory(data, value);

    return 0;
}
