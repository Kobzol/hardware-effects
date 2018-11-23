#include <iostream>
#include <vector>
#include <chrono>
#include <memory>

using Type = uint8_t;

#define CACHE_LINE_SIZE 64
#define PAGE_SIZE 4096
#define REPETITIONS 100000

void test_memory(std::vector<Type*>& memory)
{
    size_t size = memory.size();

    using Clock = std::chrono::steady_clock;
    auto start = Clock::now();

    for (int i = 0; i < REPETITIONS; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            *memory[j] += i;
        }
    }

    std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count() << std::endl;
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cout << "tlb-aliasing <count> <page_stride>" << std::endl;
        return 1;
    }

    int count = std::stoi(argv[1]);
    int page_stride = std::stoi(argv[2]);

    auto block_size = static_cast<unsigned int>(PAGE_SIZE * page_stride);
    auto mem = std::unique_ptr<Type[]>(new Type[block_size * count]());

    std::vector<Type*> pointers(static_cast<size_t>(count), nullptr);

    // jump by at least a page, but hopefully there are no cache misses caused by cache aliasing after the
    // first iteration because we also offset by one cache line each time.
    for (int i = 0; i < count; i++)
    {
        pointers[i] = mem.get() + (i * block_size + ((i * CACHE_LINE_SIZE) % PAGE_SIZE));
    }

    test_memory(pointers);

    return 0;
}
