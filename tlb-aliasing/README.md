## TLB aliasing
This example demonstrates how the TLB (Translation Lookaside Buffer) works as an
N-way associative cache. The TLB stores physical-to-virtual address mappings
from the currently active page table. This information is needed by the CPU for
any memory access, including cache accesses. So when there is a miss (the program
tries to access a virtual memory address that currently has no physical
translation loaded in the TLB), the CPU has to wait for the relevant part of the
page table to be loaded. This causes a stall similar to a cache miss.

As is the case with the data/instruction caches, if sequentially accessed addresses alias
in the cache (or if the cache is full), it may cause slowdown (so called TLB thrashing).

``` bash
$ tlb-aliasing <count> <page_stride>
```

The program will write to `count` addresses which are each `page_stride *
PAGE_SIZE` apart (as well as a small offset to help prevent extra cache misses from cache aliasing).
The TLB stores address translations at a page-level granularity, so each different
page that is written to corresponds to a different TLB entry.

Most recent Intel x86-64 processors have a two-level TLB for 4K pages, the
second level of which usually has more than 1000 entries and is highly associative (8/12-way).

If you run the program with the same `count` addresses but with varying strides, there
should be some strides that will cause slowdown because of the TLB entries aliasing in the cache.
You should also see significant slowdown when the `count` goes over the number of entries your
TLB can hold.

This is how it looks on my CPU when I allocate 800 pages with different strides. We can see that
different strides can vary the TLB miss count significantly and slow down the program. Note that the
L1 cache miss count stays the same (LLC cache miss count stayed constant too).

```bash
$ perf stat -edTLB-load-misses,dTLB-store-misses,L1-dcache-load-misses tlb-aliasing 800 1
189
4 234      dTLB-load-misses                                            
7 385      dTLB-store-misses                                           
183 050 721      L1-dcache-load-misses

$ perf stat -edTLB-load-misses,dTLB-store-misses,L1-dcache-load-misses tlb-aliasing 800 2
451
1 095 389      dTLB-load-misses                                            
1 319 622      dTLB-store-misses                                           
185 784 398      L1-dcache-load-misses           
 
```
These numbers should be taken with a grain of salt, I measured runs with `stride 2`
where the TLB miss count was 50x higher than the given number and runs where the miss count was
close to the numbers measured with `stride 1`. It seems to be strongly affected by 

I suspect that with very large strides, the TLB also suffers because the hardware prefetcher will
not be able to prefetch such large strides (which would cause the TLB entries to be prefetched
as well).

You can use the provided `benchmark.py` script to test various `count/page_stride` combinations
and plot their relative speeds.

```bash
$ python3 benchmark.py <path-to-executable>
```

You can find more about TLB here:
https://en.wikipedia.org/wiki/Translation_lookaside_buffer


### Getting Your TLB Layout Info
On Linux you could use the cpuid program and grep TLB information:
```bash
$ cpuid | grep TLB
```

You can also lookup your chip's architecture TLB details at https://en.wikichip.org/wiki/WikiChip.

If you want to do it the hard way, you can programmatically use the CPUID instruction
(https://stackoverflow.com/questions/1666093/cpuid-implementations-in-c/4823889#4823889) and
then consult the result with the documentation (https://c9x.me/x86/html/file_module_x86_id_45.html).
