#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>

#ifndef REPETITIONS
    #define REPETITIONS 50
#endif

using Type = int;

size_t test_memory(const std::vector<Type*>& memory)
{
    size_t sum = 0;
    size_t size = memory.size();

    using Clock = std::chrono::steady_clock;
    auto start = Clock::now();

    for (int i = 0; i < REPETITIONS; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            sum += *memory[j];
        }
    }

    std::cerr << std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - start).count() << std::endl;
    return sum;
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cout << "Usage: random-accesses <shuffle> <size>" << std::endl;
        return 1;
    }

    int shuffle = std::stoi(argv[1]);
    int size = std::stoi(argv[2]) / sizeof(Type);

    std::vector<Type> data(size, 0);
    std::vector<Type*> pointers;
    for (int i = 0; i < size; i++)
    {
        data[i] = i;
        pointers.push_back(data.data() + i);
    }

    if (shuffle == 1)
    {
        std::default_random_engine rng;
        rng.seed(0);
        std::shuffle(pointers.begin(), pointers.end(), rng);
    }

    test_memory(pointers);

    return 0;
}
