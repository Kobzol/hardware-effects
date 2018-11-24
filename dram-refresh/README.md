## DRAM refresh
DRAM (Dynamic Random Access Memory) uses capacitors to store the logical value of a single bit. Because the capacitors
gradually lose their charge, they have to be periodically refreshed by the memory controller (memory accesses actually refresh the cells
that they read/write, but there is of course no guarantee that the CPU will access all the cells in the required period).

Current RAM modules usually have a refresh rate of [64 ms](https://en.wikipedia.org/wiki/Memory_refresh#Types_of_refresh_circuits),
during which all of its cells have to be refreshed. To spread this expensive operation into smaller
chunks, the memory refreshes 1/8192 of its cells every 7.8 μs (64 ms / 8192 = 7.8 μs).

During the refresh operation the memory cannot be accessed, which can influence the latency of memory accesses
served directly from the memory (not from the cache). This program tries to demonstrate this effect and measure the
RAM refresh interval.

This demonstration was heavily inspired by an awesome [blog post](https://blog.cloudflare.com/every-7-8us-your-computers-memory-has-a-hiccup/) from Marek Majkowski.

Usage:
```bash
$ dram-refresh
```

This program tries to measure memory access slowdown caused by DRAM refresh. It repeatedly loads a single number
from memory and records how long it took. To make sure that the number is served directly from memory, the cache line
with the number is flushed (cleared) after every access.

An average of the access times is used to keep only the accesses that were suspiciously slow (`>= average * 2`).
Then the program prints the times between successive slow accesses to the standard error output.

This output won't tell you much, so use the Python benchmark script that will remove outliers from the data and
plot a histogram. The X axis will show the time between slow accesses in nanoseconds
and the Y axis the frequency of each value.

On my computer I can consistently see most of the numbers around 7800 nanoseconds, which corresponds with the
7.8 μs refresh interval that my DRAM should have.

```bash
$ python3 benchmark.py <path-to-executable>
```

You can find more about DRAM refresh here:
https://blog.cloudflare.com/every-7-8us-your-computers-memory-has-a-hiccup/
https://en.wikipedia.org/wiki/Memory_refresh
