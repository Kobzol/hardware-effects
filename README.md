# Hardware effects
This repository demonstrates various hardware effects that can degrade application performance
in surprising ways and that may be very hard to explain without knowledge of the low-level CPU and OS architecture.
For each effect I try to create a proof of concept program that is as small as possible so that it can
be understood easily.

Those effects obviously depend heavily on your CPU microarchitecture and model,
so the demonstration programs may not showcase the slowdown on your CPU, but I try to make
them as general as I can. That said, the examples are targeting x86-x64 processors (Intel and AMD)
and may not make sense on other CPU architectures. I try to make them compatible
with Windows, but they are mainly tested on Linux.

Currently the following effects are demonstrated:

- branch misprediction
- branch target misprediction
- cache aliasing
- memory-bound program
- data dependencies
- false sharing
- write combining

Every example directory has a README that explains the individual effects.

All programs print their execution time to the standard error output (`stderr`).

I try to focus on effects that should be observable on commodity (desktop/notebook) hardware,
so for example I left out NUMA effects.

Isolating those hardware effects can be very tricky, so it's possible that some of the
examples are actually demonstrating something entirely else (or nothing at all :) ).
If you have a better explanation of what is happening, please let me know in the issues.
Ideally the code should be written in assembly, however that would lower it's readability.
I wrote it in C++ in a way that (hopefully) forces the compiler to emit the instructions that I want (even with -O3).

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

### License
MIT

### Resources
- http://igoro.com/archive/gallery-of-processor-cache-effects/
- https://mechanical-sympathy.blogspot.com
- https://manybutfinite.com/category/software-illustrated/
