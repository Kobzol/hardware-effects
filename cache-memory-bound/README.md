## Memory bound program
This example demonstrates that if a program is bound by memory latency, it doesn't really matter how many
instructions it actually executes.

Usage:
```bash
$ cache-memory-bound <increment>
```

This is a demonstration of a program bound by memory, not computation. It allocates a large array of 4-byte
integers, repeatedly goes over them in a cycle and multiplies them by a constant. The `increment` parameter
specifies increment between accessed elements in the cycle. With `increment=1` all integers will be accessed,
with `increment=2` every other integer will be accessed, with `increment=16` only 1/16 of the integers will be
accessed etc.

Naturally one could expect that when you only multiply half of the array, it will be roughly two times faster, when you
only multiply one eighth of the array, it will be eigth times faster because the program is doing only a fraction
of the work. However here that is not the case, because most of the program time is spent in actually fetching data from
and to memory, not by multiplying (hence it is called memory-bound).

The memory is fetched in (64 byte) cache lines, so even when the program accesses every second or every fourth 4-byte
integer, it will still load the same amount of cache lines from the memory. And because the memory
access dominates the execution time, the fact that only half of the multiplications is performed is not very important.

When you run the program, you should observe that the time will be almost the same with increments from 2 to 16.
Even though the number of multiplications will be reduced with each step, the number of accessed cache lines will
stay the same. On my CPU, with increment 1 it takes longer than with increment 2, which signifies that when all integers
on the cache line are multiplied, the computation part of the program starts to be significant.
From increment 2 onwards it's strictly memory-bound.

You can run the Python benchmark script to plot how the time depends on various increments.
```bash
$ python3 benchmark.py <path-to-executable>
```

You can find more about cache effects here:
http://igoro.com/archive/gallery-of-processor-cache-effects/
