# Hardware effects
This repository demonstrates various hardware effects that can degrade application performance
in surprising ways and that may be very hard to explain without knowledge of the low-level CPU and OS architecture.
For each effect I try to create a proof of concept program that is as small as possible so that it can
be understood easily.

Related repository with GPU hardware effects: https://github.com/kobzol/hardware-effects-gpu

Those effects obviously depend heavily on your CPU microarchitecture and model,
so the demonstration programs may not showcase the slowdown on your CPU, but I try to make
them as general as I can. That said, the examples are targeting x86-64 processors (Intel and AMD)
and may not make sense on other CPU architectures. I focus on effects that should be observable on
commodity (desktop/notebook) hardware, so I don't include things like NUMA effects here
(although in a few years they might be common even in personal computers). The code is mainly tested on Linux.

Currently the following effects are demonstrated:

- 4k aliasing
- bandwidth saturation
- branch misprediction
- branch target misprediction
- cache conflicts
- cache/memory hierarchy bandwidth
- data dependencies
- denormal floating point numbers
- DRAM refresh interval
- false sharing
- hardware prefetching
- hardware store elimination
- memory-bound program
- misaligned accesses
- non-temporal stores
- software prefetching
- store buffer capacity
- write combining

Every example directory has a README that explains the individual effects.

Isolating those hardware effects can be very tricky, so it's possible that some of the
examples are actually demonstrating something entirely else (or nothing at all :) ).
If you have a better explanation of what is happening, please let me know in the issues.
Ideally the code should be written in assembly, however that would lower its readability.
I wrote it in C++ in a way that (hopefully) forces the compiler to emit the instructions that I want (even with -O3).

### Benchmarking

For all benchmarks I recommend to turn off frequency scaling, hyper-threading, Turbo mode, address space randomization and
other stuff that can increase noise. I'm using the following commands:
```bash
$ sudo bash -c "echo 0 > /proc/sys/kernel/randomize_va_space"           # address randomization
$ sudo bash -c "echo 1 > /sys/devices/system/cpu/intel_pstate/no_turbo" # Turbo mode
$ sudo bash -c "echo 0 > /sys/devices/system/cpu/cpuX/online"           # hyper-threading
$ ...                                                                   # for all hyper-threading CPUs
$ sudo cpupower frequency-set --governor performance                    # frequency scaling
```

You can find more tips [here](https://llvm.org/docs/Benchmarking.html).

### Build
```bash
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make -j
```

If you want to use the benchmark scripts (written in Python 3), you should
also install the Python dependencies:
```bash
$ pip install -r requirements.txt
```

### Docker
You can download a prebuilt image:
```bash
$ docker pull kobzol/hardware-effects
```
or build it yourself:
```bash
$ docker build -t hardware-effects .
```

Then run it:
```bash
# interactive run
$ docker run --rm -it hardware-effects

# directly launch a program
$ docker run hardware-effects build/branch-misprediction/branch-misprediction 1
```

### License
MIT

### Resources
- https://software.intel.com/en-us/download/intel-64-and-ia-32-architectures-optimization-reference-manual
- https://akkadia.org/drepper/cpumemory.pdf
- http://igoro.com/archive/gallery-of-processor-cache-effects/
- https://mechanical-sympathy.blogspot.com
- https://manybutfinite.com/category/software-illustrated/
- https://randomascii.wordpress.com/
- https://www.agner.org/optimize/
- https://software.intel.com/en-us/articles/intel-sdm#combined
