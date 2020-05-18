#include <chrono>
#include <iostream>
#include <vector>

#ifndef REPETITIONS
    #define REPETITIONS 1000
#endif

using Type = size_t;

void __attribute__ ((noinline)) test_memory(std::vector<Type>& memory, Type value)
{
    using Clock = std::chrono::steady_clock;

    size_t size = memory.size();

    auto start = Clock::now();

    for (int i = 0; i < REPETITIONS; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            memory[j] = value;
        }
    }

    auto time = std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - start).count();
    auto timePerIter = time / (double) REPETITIONS;
    auto bytes = size * sizeof(Type);

    std::cerr << bytes / timePerIter << std::endl;  // MiB/s
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cout << "Usage: hardware-store-elimination <size> <value>" << std::endl;
        return 1;
    }

    size_t count = std::stoll(argv[1]) / sizeof(Type);
    size_t value = std::stoll(argv[2]);
    std::vector<Type> data(count, value);

    test_memory(data, value);

    return 0;
}
