#include <iostream>
#include <vector>
#include <chrono>
#include <memory>

#ifndef REPETITIONS
    #define REPETITIONS 50
#endif

#ifndef SIZE
    #define SIZE 4 * 1024 * 1024
#endif

using Type = double;

template <int VERSION>
void test_memory(std::vector<Type>& memory, const std::vector<Type>& src)
{
    size_t size = src.size();

    using Clock = std::chrono::steady_clock;
    auto start = Clock::now();

    for (int i = 0; i < REPETITIONS; i++)
    {
        for (size_t j = 0; j < size - 1; j++)
        {
            if (VERSION == 0)
            {
                memory[0] += src[j];
                memory[0] += src[j];
                memory[0] += src[j + 1];
                memory[0] += src[j + 1];
            }
            else
            {
                memory[0] += src[j];
                memory[1] += src[j];
                memory[2] += src[j + 1];
                memory[3] += src[j + 1];
            }
        }
    }

    Type sum = memory[0] + memory[1] + memory[2] + memory[3];

    std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count() << std::endl;
    std::cout << "Sum: " << sum << std::endl;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: data-dependency <version>" << std::endl;
        return 1;
    }

    int version = std::stoi(argv[1]);

    std::vector<Type> data(4, 0);
    std::vector<Type> src(SIZE, static_cast<const Type&>(5));

    if (version == 0) test_memory<0>(data, src);
    else test_memory<1>(data, src);

    return 0;
}
