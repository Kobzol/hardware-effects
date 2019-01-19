## Floating point
Decimal numbers are usually stored in computers using the IEEE 754 floating point standard
(https://en.wikipedia.org/wiki/IEEE_754). CPU instructions that operate on those numbers are much more complex
than those working with classic integers and that may introduce some surprising performance bottlenecks.

## Denormal numbers
In the standard form, the floating point numbers are represented as `h.s * 2^e`, where `s` is the significand (mantissa),
`e` is the exponent and `h` is the hidden bit. Most floating point numbers
(except for negative and positive zeros) have the `h` bit set to one. However there is also a set of so-called denormal
numbers that are very small (near the zero) and have the hidden bit set to zero.
Performing some arithmetic operations on such numbers can cause severe performance degradation. The Intel designers
optimized for the fast case and denormal numbers are handled by the Microcode Sequencer, which
is much slower than operating on classic floating point numbers in the FP execution units.

Usage:
```bash
$ denormals <flush> <value>
```

This program will create a large array of floating point numbers and fill them with the given `value`.
It will then go over them repeatedly and multiply them with the `value`. If `flush` is 1, the program will
instruct the CPU to flush the denormal numbers to zero and therefore it won't have to deal with them in a special way.
When `flush` is 0, the CPU should default to a mode where it keeps the denormal numbers and thus it should incur
significant overhead when multiplying them.

If you run the program with `flush` 0, you should observe similar running times for most `values`. However if you
use very small values you should observe large slowdown. With `flush` 1 the slowdown should not occur. The loop
over the numbers is repeated 10 times, so you should start seeing the effect on numbers that become denormal after
being multiplied by itself 10 times. For 32-bit binary floats you should start to see the effect around the `value`
0.0001 and smaller.

On my CPU the program runs almost six times slower when the `value` is small and thus denormal numbers are being created.

You can measure the number of times the CPU switched to microcode and also how many times
it invoked a floating point assist.
```bash
$  perf stat -e idq.ms_switches,fp_assist.any denormals 0 0
104
939 880       idq.ms_switches
      0       fp_assist.any

$  perf stat -e idq.ms_switches,fp_assist.any denormals 0 0.3
694
111 770 619   idq.ms_switches
 15 728 640   fp_assist.any
```

You can run the Python benchmark script to plot how the time depends on the `flush` mode and `value`.
```bash
$ python3 denormals.py <path-to-executable>
```

You can find more about denormal floating point numbers here:
https://randomascii.wordpress.com/2012/05/20/thats-not-normalthe-performance-of-odd-floats/

## Mixing SSE and AVX instructions
Intel CPUs have to maintain backwards compatibility for the x86-64 instruction set. Therefore they have to support
even very old instructions that are made obsolete by their newer variants. For example they have to support the old
SSE vector instructions in addition to the newer AVX instructions. SSE operates on 128-bit `XMM` registers that are
actually mapped to the lower part of the 256-bit AVX `YMM` registers.

This actually has performance implications if dirty `YMM` registers (those modified by an AVX-256 instruction) are used by
SSE instructions. Because the upper bits of those registers will be unchanged by the SSE instructions, they will carry
a partial dependency between successive instructions and the upper bits will have to be masked in the destination register.
Both things will cause the instructions to execute slower, which means that if you mix SSE and AVX instructions,
you clear the upper bits (for example with the `vzeroupper` instruction) between executing an AVX and a SSE instruction
(compiler usually does this for you).

In Intel microarchitectures up to Broadwell this slowdown only really occurs if you interleave SSE
and AVX instructions, which isn't very probable. However in Skylake and newer architectures the situation is different.
If you execute any AVX-256 instruction that leaves the registers dirty, ALL following SSE instructions will be slower
until the upper bits are cleared.

This can happen in real world code if it's compiled without AVX support and uses SSE instructions.
Even if your code doesn't directly use AVX instructions that would make the upper bits dirty,
they can get dirty for example from library functions. If that happens and `vzeroupper` or similar instructions
aren't used, the whole program will suffer from slowed SSE instructions on Skylake and newer architectures.

Usage:
```bash
$ sse-avx-mix <zero-upper>
```

This program will execute a lot of SSE instructions. If `zero-upper` is one, it will execute the `vzeroupper`
instruction at the beginning of the program. If it's zero it won't be executed. On Skylake and newer
architectures you should observe a large slowdown if you don't zero the upper bits. A single instruction executed at
the start of the program can thus make the program several times more efficient.

At the beginning of the program, the `YMM` registers are written to make sure that they are dirty, however even without
this write the registers end up dirty everytime I run the program.

The code is compiled without `-mavx` so the compiler doesn't use AVX-256 instructions and uses SSE.
However I have to use the `vzeroupper` instruction, so I "cheat" by using GCC inline assembly.

You can run the Python benchmark script to plot how the time depends on the `zero-upper` value.
```bash
$ python3 sse-avx-mix.py <path-to-executable>
```

You can find more about the SSE/AVX transitions in Intel CPUs here:
https://software.intel.com/sites/default/files/managed/9e/bc/64-ia-32-architectures-optimization-manual.pdf (page 12-7)
https://stackoverflow.com/questions/41303780/why-is-this-sse-code-6-times-slower-without-vzeroupper-on-skylake
