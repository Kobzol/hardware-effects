## Measuring store buffer capacity
CPU cores contain store buffers, which buffer stores until they are committed to the
cache. This allows the core to continue execution up to some point without paying the store
latency in case of a cache miss. When the buffer fills up, the core stalls before executing
further instructions.

Store buffers are one of the components affecting the processor's memory model
and their existence affects the visibility and order of stores as seen by other cores,
so it has to be taken into account when using lock-free or similar parallel programming techniques.
The draining/flushing of store buffers can be indirectly controlled using various memory
barrier/fence instructions (e.g. `mfence`).

Usage:
```bash
$ store-buffer-capacity
```

The program will repeatedly execute N stores followed by 500 NOPs. If the store buffer capacity
is larger or equal to N, the latency of the NOPs should be at least partially hidden.
However when the store buffer capacity runs out, the NOPs will be blocked until the stores complete,
which will slow the program down.

To change N, you have to change the `REP` macro surrounding the store in the code and recompile it:
```cpp
REP(0, 4, 2, sum = i;) // 42 stores
REP(0, 5, 6, sum = i;) // 56 stores
REP(0, 5, 7, sum = i;) // 57 stores
```

You can measure the number of resource stalls caused by store buffer using `perf`. Going over
the store buffer capacity should increase the number of resource stalls and also slightly increase
the runtime. On my CPU (Skylake/Kaby lake), the store buffer should have 56 entries, which is
reflected in the number of resource stalls when going over 56:

```bash
$  perf stat -e resource_stalls.sb store-buffer-capacity # 56 stores
41
   120 937  resource_stalls.sb

$  perf stat -e resource_stalls.sb store-buffer-capacity # 57 stores
48
21 281 405  resource_stalls.sb
```

On Haswell, the capacity should be 42. You can find the corresponding number on WikiChip or in the
Intel Architectures Optimization Manual.

This test was suggested by @nicknash (https://github.com/Kobzol/hardware-effects/issues/4#issuecomment-454719253).

Original blog post:
https://nicknash.me/2018/04/07/speculating-about-store-buffer-capacity/

Explanation of store buffers and memory models:
https://stackoverflow.com/a/54880249/1107768
https://preshing.com/20120930/weak-vs-strong-memory-models/
