#include <iostream>
#include <vector>
#include <chrono>
#include <memory>

#ifndef REPETITIONS
    #define REPETITIONS 10000000
#endif

using Type = int;

void test_memory(std::vector<Type>& memory, size_t increment)
{
    using Clock = std::chrono::steady_clock;

    auto size = memory.size();
    auto start = Clock::now();

    for (int i = 0; i < REPETITIONS; i++)
    {
        for (size_t j = 0; j < size; j += increment)
        {
            memory[j] += j;
        }
    }

    std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count() << std::endl;
}


int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cout << "Usage: cache-conflicts <count> <increment>" << std::endl;
        return 1;
    }

    auto count = static_cast<size_t>(std::stoi(argv[1]));
    auto increment = static_cast<size_t>(std::stoi(argv[2])) / sizeof(Type);

    std::vector<Type> memory(count * increment);
    test_memory(memory, increment);

    return 0;
}
