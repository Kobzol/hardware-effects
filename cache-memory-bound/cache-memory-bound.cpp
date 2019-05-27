#include <iostream>
#include <vector>
#include <chrono>
#include <memory>
#include <random>
#include <algorithm>

#ifndef REPETITIONS
    #define REPETITIONS 10
#endif

#ifndef SIZE
    #define SIZE 32 * 1024 * 1024
#endif

using Type = int;

void test_memory(std::vector<Type>& memory, int increment)
{
    size_t size = memory.size();

    using Clock = std::chrono::steady_clock;
    auto start = Clock::now();

    for (int i = 0; i < REPETITIONS; i++)
    {
        for (size_t j = 0; j < size; j += increment)
        {
            memory[j] *= 5;
        }
    }

    std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count() << std::endl;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: cache-memory-bound <increment>" << std::endl;
        return 1;
    }

    std::vector<Type> data(SIZE, 1);
    int increment = std::stoi(argv[1]);

    test_memory(data, increment);

    return 0;
}
