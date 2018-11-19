#include <chrono>
#include <iostream>
#include <vector>

#define REPEAT 1000

using Type = double;

void test_memory(std::vector<Type>& memory)
{
    using Clock = std::chrono::steady_clock;

    size_t size = memory.size();

    Type value = rand();
    auto start = Clock::now();

    for (int i = 0; i < REPEAT; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            memory[j] = value;
        }
    }

    auto time = std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - start).count();
    auto timePerIter = time / (double) REPEAT;
    auto bytes = size * sizeof(Type);

    std::cerr << bytes / timePerIter << std::endl;  // MiB/s
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: cache-hierarchy-bandwidth <size>" << std::endl;
        return 1;
    }

    size_t count = std::stoll(argv[1]) / sizeof(Type);
    std::vector<Type> data(count, 0);

    test_memory(data);

    return 0;
}
