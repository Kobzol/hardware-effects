## False sharing
This example demonstrates false sharing of cache lines.
Each CPU core usually has its own private L1/L2 cache and so there is a coherency protocol
to keep them consistent (https://en.wikipedia.org/wiki/MESI_protocol). You have to keep
the caches in sync, otherwise you would start to see inconsistencies.

This also has some performance implications, even if you do not use synchronizations
primitives like locks, memory fences, atomic increments, CAS etc.
If a thread T1 accesses a cache line that is in T2's cache, the cache line has to be transferred from T2 to T1 and invalidated
for T2. If this happens very often in short timespans it can significantly degrade performance of your program.
It is therefore important to align data so that if nearby things are accessed by different threads,
the individual data items are be placed on different cache lines.

Usage:
```bash
$ false-sharing <thread-count> <increment>
```

The program will create `thread-count` threads. Each thread will have its own
8-byte integer and it will repeatedly write to it.
Addresses of those integers will be `increment * 8` bytes apart. So `increment 1` means they will be next to each other
in memory (on the same cache line), while `increment 8` means that each integer will get its own cache line.

You should observe significant slowdown when using `increment 1` with more than one thread.
On the other hand if every thread gets its own cache line (`increment 8`), the time shouldn't
change much if you keep adding threads (up to the point where you spawn more threads than you have logical cores of course).

I recommend to turn off Hyper-threading for benchmarking this. If two threads are allocated on the same core,
this effect will be gone.

```bash
echo 0 > /sys/devices/system/cpu/cpu4/online
echo 0 > /sys/devices/system/cpu/cpu5/online
...
```

False sharing can be measured by counting the number of Requests-for-ownership in the L2 cache.
RFOs are messages that invalidate a cache line in other cores because some other core wants to write to it.
You can also measure the number of times a cache line was loaded into the L1 cache.

```bash
$  perf stat -e l2_rqsts.all_rfo,l1d.replacement false-sharing 3 1
572
21 626 086    l2_rqsts.all_rfo
25 142 511    l1d.replacement

$  perf stat -e l2_rqsts.all_rfo,l1d.replacement false-sharing 3 8
389
    14 183    l2_rqsts.all_rfo
    64 585    l1d.replacement
```

You can use the provided `benchmark.py` script to test various `thread-count/increment` combinations
and plot their relative speeds.

```bash
$ python3 benchmark.py <path-to-executable>
```

False sharing explanation:
https://mechanical-sympathy.blogspot.com/2011/07/false-sharing.html
