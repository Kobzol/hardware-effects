## False sharing
This example demonstrates false sharing of cache lines.
Each CPU core has its own private L1/L2 cache and so there has to be a coherency protocol
to keep them consistent (https://en.wikipedia.org/wiki/MESI_protocol).

If a thread T1 accesses a cache line that is in T2's cache, it has to be transferred from T2 to T1 and invalidated
for T2. If this happens very often in short timespans it can significantly degrade performance of your program.

The program will create `thread-count` threads. Each thread will repeatedly write to a single 8-byte integer.
Those integers will be `increment * 8` bytes apart. So `increment 1` means they will be next to each other
in memory (on the same cache lines), while `increment 8` means that each integer will get its own cache line.

You should observe significant slowdown when using `increment 1` with more than one thread.

Usage:
```bash
$ false-sharing <thread-count> <increment>
```

You can use the provided `benchmark.py` script to test various `thread-count/increment` combinations
and plot their relative speeds.

```bash
$ python3 benchmark.py <path-to-executable>
```

False sharing explanation:
https://mechanical-sympathy.blogspot.com/2011/07/false-sharing.html
