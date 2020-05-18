## Hardware store elimination
Writing data to memory is a very expensive operation for modern CPUs. They thus leverage multi-level cache hierarchies
that try to delay or avoid writes to RAM as long as possible. When there is no space in a cache for a cache line and the
cache line has been modified, it needs to be evicted and written back to a higher cache level or to RAM.

Intuitively, this process does not depend on the actual value of the cache line, it should be just opaque data for the processor.
However, Travis Downs [has found out](https://travisdowns.github.io/blog/2020/05/13/intel-zero-opt.html) that client Skylake Intel
microarchitectures (Skylake, Kaby Lake, Coffee Lake, Whiskey Lake, ...) seem to use a peculiar heuristic that sometimes skips
the writeback to memory when the value of the cache line is exactly zero. (Repeatedly) writing zeros to memory can thus
potentially achieve higher bandwidth than writing other values.

Usage:
```bash
$ hardware-store-elimination <size> <value>
```

This program will create an array with `size` bytes. It will then repeatedly write the given `value` to all elements of
the array and report the achieved bandwidth (how many MiB/s were transferred). If you have a Skylake client microarchitecture,
you should see that once you get to L2/L3/RAM size, writing zeros is faster than writing other values. 

You can observe this effect by checking the `l2_lines_out.silent` and `l2_lines_out.non_silent` performance counters, which
measure how many times was some cache line evicted (`non_silent`) or dropped (`silent`) from the L2 cache.

```bash
$  perf stat -e l2_lines_out.silent,l2_lines_out.non_silent hardware-store-elimination 8388608 1
16694.3
        1 445 170    l2_lines_out.silent
      130 429 669    l2_lines_out.non_silent

$  perf stat -e l2_lines_out.silent,l2_lines_out.non_silent hardware-store-elimination 8388608 0
19524.3
       38 832 738    l2_lines_out.silent
       92 787 097    l2_lines_out.non_silent
```

You can run the Python benchmark script to plot how the write bandwidth depends on the written value.
```bash
$ python3 benchmark.py <path-to-executable>
```

You can find detailed observations about this effect at Travis Downs' blog:
- https://travisdowns.github.io/blog/2020/05/13/intel-zero-opt.html
- https://travisdowns.github.io/blog/2020/05/18/icelake-zero-opt.html
