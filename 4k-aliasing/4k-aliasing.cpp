#include <iostream>
#include <vector>
#include <chrono>

#ifndef SIZE
    #define SIZE 1024
#endif

#ifndef REPETITIONS
    #define REPETITIONS 2 * 1024 * 1024UL
#endif

using Type = float;

void test_memory(Type* a, const Type* b)
{
    using Clock = std::chrono::steady_clock;
    auto start = Clock::now();

    for (size_t r = 0; r < REPETITIONS; r++)
    {
        for (int i = 0; i < SIZE; i++)
        {
            a[i] += b[i];
        }
    }

    std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count() << std::endl;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: 4k-aliasing <increment>" << std::endl;
        return 1;
    }

    std::vector<Type> data(SIZE * 10, 1);

    int increment = std::stoi(argv[1]);

    Type* a = data.data();
    Type* b = data.data() + (increment / sizeof(Type));

    test_memory(a, b);

    return 0;
}
