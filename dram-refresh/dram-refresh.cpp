/**
 * Code adapted from Marek Majkowski's blog post
 * https://blog.cloudflare.com/every-7-8us-your-computers-memory-has-a-hiccup
 */

#include <iostream>
#include <chrono>
#include <vector>
#include <memory>
#include <immintrin.h>
#include <cassert>

#ifndef REPETITIONS
    #define REPETITIONS 512 * 1024
#endif

struct Record
{
    uint32_t timestamp;
    uint32_t duration;
};

using Clock = std::chrono::steady_clock;

inline static size_t get_time()
{
    return static_cast<size_t>(std::chrono::time_point_cast<std::chrono::nanoseconds>(Clock::now())
            .time_since_epoch().count());
}

int main()
{
    auto memory = std::unique_ptr<size_t[]>(new size_t[4096]);
    assert((reinterpret_cast<uintptr_t>(memory.get()) & 15) == 0);

    auto* src = memory.get();

    std::vector<Record> records(REPETITIONS);

    auto root = get_time();

    // use volatile to prevent compiler from optimizing it out
    volatile size_t dest;
    for (int i = 0; i < REPETITIONS; i++)
    {
        _mm_clflush(src); // clear cache
        auto start = get_time();

        _mm_mfence(); // just to make sure that the access is not reordered
        dest = *src;
        _mm_mfence();

        auto end = get_time();

        records[i].timestamp = static_cast<uint32_t>(start - root);
        records[i].duration = static_cast<uint32_t>(end - start);
    }

    // calculate average access time
    size_t sum = 0;
    for (auto value: records) sum += value.duration;
    size_t avg = sum / records.size();

    // calculate durations between long access times (>= 2 * avg)
    auto last = records[0].timestamp;
    for (size_t i = 1; i < records.size(); i++)
    {
        auto& record = records[i];
        if (record.duration > avg * 2)
        {
            auto space = record.timestamp - last;
            std::cerr << space << std::endl;
            last = record.timestamp;
        }
    }

    return 0;
}
