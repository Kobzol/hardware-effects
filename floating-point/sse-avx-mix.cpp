/**
 * Code adapted from https://stackoverflow.com/questions/41303780/why-is-this-sse-code-6-times-slower-without-vzeroupper-on-skylake
 */

#include <immintrin.h>
#include <iostream>
#include <chrono>

#ifndef REPETITIONS
    #define REPETITIONS 100 * 1024 * 1024
#endif

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: sse-avx-mix <zero-upper>" << std::endl;
        return 1;
    }

    // execute AVX-256 instruction, just to make sure
    float data[16];
    for (int i = 0; i < 16; i++) data[i] = (float) i;
    __asm__ __volatile__(
        "vmovups    (%0, %%rcx), %%ymm0" // move data to YMM0
        :: "r"(data): "%ymm0"
    );

    using Clock = std::chrono::steady_clock;
    auto start = Clock::now();

    if (std::stoi(argv[1]) == 1)
    {
        __asm__ __volatile__("vzeroupper" :::); // zero upper parts of AVX registers
    }

    int r = 0;
    for (int i = 0; i < REPETITIONS; i++)
    {
        __m128d a = _mm_set_sd(i);
        __m128d b = _mm_add_sd(a, a);
        __m128d sqr = _mm_sqrt_sd(a, b);
        r += _mm_movemask_pd(sqr);
    }

    std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count() << std::endl;
    std::cout << r << std::endl;

    return 0;
}
