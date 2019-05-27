#include <iostream>
#include <chrono>

#ifndef REPETITIONS
    #define REPETITIONS 1000000
#endif

// taken from https://stackoverflow.com/a/8556436/1107768
#define REP0(X)
#define REP1(X) X
#define REP2(X) REP1(X) X
#define REP3(X) REP2(X) X
#define REP4(X) REP3(X) X
#define REP5(X) REP4(X) X
#define REP6(X) REP5(X) X
#define REP7(X) REP6(X) X
#define REP8(X) REP7(X) X
#define REP9(X) REP8(X) X
#define REP10(X) REP9(X) X

#define REP(HUNDREDS,TENS,ONES,X) \
  REP##HUNDREDS(REP10(REP10(X))) \
  REP##TENS(REP10(X)) \
  REP##ONES(X)

int main()
{
    volatile float sum = 0;

    using Clock = std::chrono::steady_clock;

    auto start = Clock::now();
    for (int i = 0; i < REPETITIONS; i++)
    {
        REP(0, 5, 6, sum = i;) // modify this to change the number of stores
        REP(5, 0, 0, __asm__ __volatile__("nop");)
    }
    std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count() << std::endl;

    return 0;
}
