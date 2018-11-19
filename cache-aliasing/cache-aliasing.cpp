#include <iostream>
#include <vector>
#include <chrono>
#include <memory>

#define REPETITIONS 10000000

void test_memory(const std::vector<int*>& memory)
{
    size_t sum = 0;
    size_t size = memory.size();

    using Clock = std::chrono::steady_clock;
    auto start = Clock::now();

    for (int i = 0; i < REPETITIONS; i++)
    {
        for (int j = 0; j < size; j++)
        {
            *memory[j] += j;
        }
    }

    std::cerr << std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - start).count() << std::endl;
    std::cout << "Sum: " << sum << std::endl;
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cout << "Usage: cache-aliasing <count> <increment>" << std::endl;
        return 1;
    }

    size_t count = static_cast<size_t>(std::stoi(argv[1]));
    size_t increment = static_cast<size_t>(std::stoi(argv[2]));

    auto memory = std::unique_ptr<int[]>(new int[count * increment]());
    auto startAddress = reinterpret_cast<size_t>(memory.get());

    std::vector<int*> data;

    for (int i = 0; i < count; i++)
    {
        data.push_back((int*)(startAddress + i * increment));
    }

    test_memory(data);

    return 0;
}
