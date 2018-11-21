## Misaligned accesses
Processors are most happy if you access memory that is aligned to a multiple of their natural word width.
For example accessing a 4-byte number should be done at an address that is 4-byte aligned (address divisible by 4).
Alignment is actually required for some (mostly vector) instructions.

This example focuses on classic scalar instructions, where the effect is largely artificial, because they are usually
aligned just fine. It's still an interesting effect that might be observable on your CPU.

Some architectures outright ban misaligned accesses (older ARMs), some simply execute them slower.
While older x86 (Intel) CPUs had lower performance for misaligned accesses, modern versions seem to have removed
this penalty (https://lemire.me/blog/2012/05/31/data-alignment-for-speed-myth-or-reality/). However there is still
a situation where misalignment can cause slowdown - if you manage to access a number across a cache boundary.

Usage:
```bash
$ misaligned-access <offset>
```

This program will create an array with 4-byte integers aligned to a cache boundary. It will then repeatedly write
an integer to an address that is `offset` bytes from the start of the array. So with `offset 0` it will write to the
first array element, `offset 4` will write to the second element etc. If you use an offset that will write to an address
spanning both the first and the second cache line in the array, you should observe (a slight) slowdown. For example
`offset 62` will put the first two bytes of the written integer into the first cache line and the following two bytes
into the second cache line (assuming you have 64 byte cache lines).

If you have a modern Intel CPU, you should observe similar times for aligned and misaligned accesses,
except for offsets `61, 62` and `63` that cross a cache boundary, where you should see a slight slowdown.

The program writes to the same memory all the time so that it is cached and the misalignment cost can be observed better.

You can run the Python benchmark script to plot how the time depends on the offset.
```bash
$ python3 benchmark.py <path-to-executable>
```

You can find more about memory alignment here:
https://en.wikipedia.org/wiki/Data_structure_alignment
