## TLB Aliasing

This example demonstrates how the TLB (Translation Lookaside Buffer) works as an
N-way associative cache.  The TLB stores physical-to-virtual address mappings
from the currently active page table.  This information is needed by the CPU for
any memory access, including cache access.  So when there is a miss (the program
tries to access a virtual memory address that currently has no physical
translation loaded in the TLB), the CPU has to wait for the relevant part of the
page table to be loaded.  This causes a stall similar to a cache miss.

``` bash
$ tlb-aliasing <count> <page_stride>
```

The program will write to `count` addresses which are each `page_stride *
PAGE_SIZE` apart (as well as a small offset to help prevent extra cache misses).
The TLB stores address translations at page-level granularity, so each different
page that is written to corresponds to a different TLB entry.

Most recent Intel x86-64 processors have a two-level TLB for 4K pages, the
second level of which has 1024 entries and is 8-way associative.  That means
there are 128 entries per set.  So here are some illustrative examples:

``` bash
$ ./tlb-aliasing 2000 1
1629.48 misses per repetition (162947773 total)
1648843 us
```
This asks for more pages than there are TLB entries, which causes repeated
capacity evictions.

``` bash
$ ./tlb-aliasing 8 128
0.04 misses per repetition (3564 total)
6031 us
```
This fills a single set of the TLB which shouldn't cause any evictions and
subsequent misses on its own, but because the main loop isn't really the only
thing in use (e.g. the stack, page table pages, etc.) a few misses still occur.

``` bash
$ ./tlb-aliasing 12 128
7.10 misses per repetition (710057 total)
9387 us
```
This fills a single set of the TLB and then causes contention for half of the
set (8-11 compete with 0-3).

``` bash
$ ./tlb-aliasing 16 128
11.97 misses per repetition (1197021 total)
16550 us
```
Finally, this fills a single set of the TLB and then causes contention for the
entire set (0-7 compete with 8-15).  This can be shown to be worse than previous
example because the miss probability per access is higher: `11.97 / 16 = .75`
vs. `7.10 / 12 = .59`.

### Getting Your TLB Layout Info

For x86 machines, tlb-info.cpp will give you a dump of descriptors with
information about your particular CPU's TLB hierarchy.  You can look up what
each descriptor means in the [Intel Software Developer's
Manual](https://software.intel.com/en-us/articles/intel-sdm) or try this [html
excerpt](https://c9x.me/x86/html/file_module_x86_id_45.html)
