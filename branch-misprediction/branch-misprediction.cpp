#include <iostream>
#include <vector>
#include <chrono>
#include <memory>
#include <random>
#include <algorithm>

#ifndef REPETITIONS
    #define REPETITIONS 20
#endif

#ifndef SIZE
    #define SIZE 20 * 1024 * 1024
#endif

using Type = float;

Type test_memory(const std::vector<Type>& memory, Type value)
{
    Type sum = 0;
    size_t size = memory.size();

    using Clock = std::chrono::steady_clock;
    auto start = Clock::now();

    for (int i = 0; i < REPETITIONS; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            if (memory[j] < value)
            {
                sum += memory[j];
            }
        }
    }

    std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count() << std::endl;
    return sum;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: branch-misprediction <sort>" << std::endl;
        return 1;
    }

    std::default_random_engine rng(0);
    std::uniform_int_distribution<int> dist(1, 10);

    std::vector<Type> data(SIZE);
    std::generate(std::begin(data), std::end(data), [&dist, &rng]() {
        return dist(rng);
    });

    if (std::stoi(argv[1]) == 1)
    {
        std::sort(data.begin(), data.end());
    }

    test_memory(data, 6);

    return 0;
}
