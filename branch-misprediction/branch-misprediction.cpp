#include <iostream>
#include <vector>
#include <chrono>
#include <memory>
#include <random>
#include <algorithm>

#define REPETITIONS 20
#define SIZE 20 * 1024 * 1024

#ifdef _WIN32
#define NOINLINE __declspec(noinline)
#else
#define NOINLINE __attribute__ ((noinline))
#endif

void NOINLINE handle_sum(size_t* sum, const int* memory)
{
    *sum += *memory;
}

size_t test_memory(const std::vector<int>& memory, int value)
{
    size_t sum = 0;
    size_t size = memory.size();

    using Clock = std::chrono::steady_clock;
    auto start = Clock::now();

    for (int i = 0; i < REPETITIONS; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            if (memory[j] < value) handle_sum(&sum, memory.data() + j);
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

    std::default_random_engine rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<int> dist(1, 100);

    std::vector<int> data(SIZE);
    std::generate(std::begin(data), std::end(data), [&dist, &rng](){
        return dist(rng);
    });

    if (std::stoi(argv[1]) == 1)
    {
        std::sort(data.begin(), data.end());
    }

    test_memory(data, 50);

    return 0;
}
