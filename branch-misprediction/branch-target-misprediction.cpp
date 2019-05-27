#include <iostream>
#include <vector>
#include <chrono>
#include <memory>
#include <random>
#include <algorithm>

#ifndef REPETITIONS
    #define REPETITIONS 10
#endif

#ifndef SIZE
    #define SIZE 10 * 1024 * 1024
#endif

struct A
{
    virtual ~A() = default;
    virtual int id() const = 0;
    virtual void handle(size_t* data) const = 0;
};

struct B: public A
{
    int id() const override { return 1; }
    void handle(size_t* data) const override { *data += 1; }
};

struct C: public A
{
    int id() const override { return 2; }
    void handle(size_t* data) const override { *data += 2; }
};

size_t test_memory(const std::vector<A*>& memory)
{
    size_t sum = 0;
    size_t size = memory.size();

    using Clock = std::chrono::steady_clock;
    auto start = Clock::now();

    for (int i = 0; i < REPETITIONS; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            memory[j]->handle(&sum);
        }
    }

    std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count() << std::endl;
    return sum;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: branch-target-misprediction <sort>" << std::endl;
        return 1;
    }

    std::default_random_engine rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<int> dist(0, 1);

    std::vector<A*> data(SIZE);
    std::generate(std::begin(data), std::end(data), [&dist, &rng]() -> A* {
        if (dist(rng)) return new B;
        return new C;
    });

    if (std::stoi(argv[1]) == 1)
    {
        std::sort(data.begin(), data.end(), [](const A* lhs, const A* rhs) {
            return lhs->id() < rhs->id();
        });
    }

    test_memory(data);

    for (auto& item: data)
    {
        delete item;
    }

    return 0;
}
