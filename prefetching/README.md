## Prefetching
CPUs contain hardware prefetchers that try to prefetch cache lines into the cache when they detect that
cache misses and the program is accessing the memory predictably. Predictably usually means that the accesses are
either sequential (stream prefetching) or strided (stride prefetching, the stride size must be reasonable).

If you know which memory will be accessed beforehand, you can help the hardware prefetcher with software prefetching,
using specialised instructions. The instructions specify which address should be prefetched and where should it be
stored (just L1 cache, all caches etc.).

Software prefetching can however also hurt performance and it's hard to find the right prefetch distance. It shouldn't
be too soon (otherwise the cache line might get evicted before it's accesses) nor too late (otherwise it won't help).

### Random accesses
Usage:
```bash
$ random-accesses <shuffle> <size>
```
This example demonstrates the performance difference between sequential and random accesses to memory.
In the sequential case, the hardware prefetcher can hide most of the latency by prefetching the required cache lines
beforehand.

The program will allocated an array of integers of `size` bytes. Then it will create an array of pointers to each individual
elements. If `shuffle` is 1, the pointers will be randomly shuffled. If `shuffle` is 0, the first pointer will point to the first
element, the second pointer to the second one etc. The pointer array is then repeatedly iterated from the beginning
to the end and the pointed elements are accumulated into a variable.

When the `size` stays small, there shouldn't be many differences between the shuffled and sequential accesses, because
the array will fit into the L1 cache. However when the `size` goes up, the cache won't be able to help much by itself.
That's where the hardware prefetcher effect should be observable - the shuffled version should be much slower with
bigger array size.

You can use the provided `random-accesses.py` script to plot the various shuffle/size combinations and their
speeds.

```bash
$ python3 random-accesses.py <path-to-executable>
```


### Software prefetching
Usage:
```bash
$ prefetching <prefetch> <distance> <hint>
```

The program will create a large array of 64 byte structures. Then it will create another array with pointers to each
of the structures and shuffle the pointers, so that accesses to the structures in memory will essentially be random.
In this case the hardware prefetcher will be hopelessly ineffective.

The program will repeatedly go over the pointers and perform several arithmetic operations per each element.
This is necessary for the prefetching to help, a simple read or write would take too little time and require the next
cache line too soon for the prefetching to help. That's also the reason why the structures are so large (64 bytes),
it makes it easier to observe the prefetching speedup.

If `prefetch` is 0, no prefetching will be performed. If it is 1, the memory address that is `distance` pointers
away will be prefetched using the `hint`. The various hint affect where will the prefetched cache lines stored
(https://stackoverflow.com/a/46525326/1107768). Hint 3 means store it in all cache levels, hint 2 means store it in L2
and above, hint 1 means store it in L3 and above and hint 0 means store it close to the processor without polluting the
cache. However those are just hints and are implementation specific.

On my CPU I can see significant (2x) speedup when using prefetch distance 10/15/20.

You can use the provided `benchmark.py` script to plot the various prefetch/distance/hint combinations and their
speeds.

```bash
$ python3 software-prefetching.py <path-to-executable>
```

More about hardware and software prefetching can be found here:
https://www.cc.gatech.edu/~hyesoon/lee_taco12.pdf
