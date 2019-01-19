## 4k aliasing
When a store to a memory address is followed by a load of that same address, the CPU
can directly forward the value to be stored to the load to avoid an expensive memory access.
This is called Store-to-load forwarding. However in order to make this faster, Intel CPUs do not
compare the whole memory address, but only the lowest 12 bits. Therefore if there's a write to an address
followed by a load from an address that has the same value of the lowest 12 bits, the CPU
will try to forward it. If it later finds out that the address is not actually the same, it will
incur a few cycles of penalty. Because the addresses have to be a multiple of
2^12 (4096) bytes apart for this to happen, this is called 4k aliasing.

Usage:
```bash
$ 4k-aliasing <increment>
```

This program creates two array of floats and repeatedly adds the second one to the first:
```cpp
for (int i = 0; i < SIZE; i++)
{
    a[i] += b[i];
}
```

The `a` and `b` arrays are `increment` bytes apart. When the increment is a bit smaller than a multiple of
`4096` (for example `4092`), the loop will store to an address and right after that load from a different
address that is 4096 bytes apart, therefore causing 4k aliasing.

Store-to-load forwarding stalls can be measured using the `ld_blocks.store_forward` performance counter:
```bash
$ perf stat -e ld_blocks.store_forward 4k-aliasing 4000
310
    3 378     ld_blocks.store_forward

$ perf stat -e ld_blocks.store_forward 4k-aliasing 4092
379
2 100 223     ld_blocks.store_forward
```

You can run the Python benchmark script to plot how the time depends on various increments.
```bash
$ python3 benchmark.py <path-to-executable>
```

You can find more about 4k aliasing here:
https://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-optimization-manual.pdf
