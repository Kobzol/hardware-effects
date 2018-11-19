#include <iostream>
#include <vector>
#include <chrono>
#include <memory>
#include <random>
#include <algorithm>
#include <xmmintrin.h>

#define REPETITIONS 10
#define SIZE 16 * 1024 * 1024
#define CSR_FLUSH_TO_ZERO (1 << 15)

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
        unsigned csr = _mm_getcsr();
        csr |= CSR_FLUSH_TO_ZERO;
        _mm_setcsr(csr);
    }

    std::vector<Type> data(SIZE, value);
    test_memory(data, value);

    return 0;
}
