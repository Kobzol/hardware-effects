## Bandwidth saturation
Bandwidth between the CPU and RAM is limited and when you heavily use it from more threads at once,
you will sooner or later hit a ceiling. From that point on adding more threads that heavily access memory will
make things worse. Typically the bandwidth cannot fully serve all cores at once, so for example you might have four cores
but two of them can already saturate the bandwidth.

Usage:
```bash
$ bandwidth-saturation <non-temporal> <thread-count>
```

The program will create `thread-count` threads. Each thread will have its own large array of numbers and it will
repeatedly write to all of its elements. If `non-temporal` is 1, the threads will use non-temporal stores to write to their
arrays, so they will not use the cache. If `non-temporal` is 0, the threads will use classic memory writes.

With the non-temporal stores, one could expect that since the threads operate on separate arrays, adding more
threads (up to the physical core count of course) shouldn't affect the execution time much. You should indeed observe
that up to some point, the time shouldn't change, however from that point adding more threads should make the program run
slower, because the memory bus will not be able to serve all of the cores at full bandwidth at once.

With classic writes the effect will be much worse, because not only will the threads share the memory bandwidth,
they will also pollute and thrash the caches (both L3 for all cores and L1/L2 for SMT hyper-threads). This is amplified
by the fact that the allocated arrays are large and are thus allocated at page boundaries, which causes cache conflicts
between the individual thread arrays. This is a good use-case for non-temporal stores.

The bandwidth effect should be even more visible if vector (SSE, AVX) instructions were used.

Perf output for the classic and non-temporal stores and increasing thread count on my PC:
```bash
$  perf stat -e LLC-store-misses bandwidth-saturation 0 1
986
141 045 819      LLC-store-misses

$  perf stat -e LLC-store-misses bandwidth-saturation 0 2
2054
582 818 110      LLC-store-misses

$  perf stat -e LLC-store-misses bandwidth-saturation 1 1
1106
2 277 983      LLC-store-misses

$  perf stat -e LLC-store-misses bandwidth-saturation 1 2
1093
4 535 311      LLC-store-misses
```

It is obvious that when using the cache, adding more threads in this scenario won't help, as it is causing heavy cache
misses.

You can use the provided `benchmark.py` script to test various `non-temporal/thread-count` combinations
and their relative speeds.

```bash
$ python3 benchmark.py <path-to-executable>
```
