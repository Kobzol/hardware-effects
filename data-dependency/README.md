## Data dependency
This example demonstrates how data dependencies between instructions can affect program performance.
Modern CPUs are pipelined to enable more parallelism inside the core itself and to hide memory latency.
Instruction execution is divided into tens of small steps in a so-called pipeline (fetch, decode, execute, write-back etc.).
However this means that when an instruction depends on the result of a previous one, the pipeline must stall for a
while to wait for the computed result.

Data dependencies are one of the hazards that can affect pipeline execution (there are also structural and control hazards).

Usage:
```bash
$ data-dependency <version>
```

The program will repeatedly accumulate elements from a large array either to a single number (`version=0`)
or to four different numbers that are then summed at the end(`version=1`).

Even though the CPU does the same amount of work with the same result, you should observe that version 1 is much faster.
When you want to add a number to the same memory location repeatedly, the CPU has to wait until the
previous operation has completed before starting the next addition. However when you write to more memory locations at once,
it can be done in parallel, because there are less data dependencies.

I recommend to use `float` or `double` as the data type, because the floating point model has restricted associativity because
of the limited precision and that should prevent the compiler from doing funky stuff like optimizing away the additions
into a single operation (that happens here with `gcc -O3` if you use `int`).

You can use the provided `benchmark.py` script to test the speeds of the two versions.

```bash
$ python3 benchmark.py <path-to-executable>
```

Data hazards explained:
https://en.wikipedia.org/wiki/Hazard_(computer_architecture)
