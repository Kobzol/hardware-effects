## Floating point
Decimal numbers are usually stored in computers using the IEEE 754 floating point standard
(https://en.wikipedia.org/wiki/IEEE_754). CPU instructions that operate on those numbers are much more complex
than those working with classic integers and that may introduce some surprising performance bottlenecks.

In the standard form, the floating point numbers are represented as `h.s * 2^e`, where `s` is the significand (mantissa),
`e` is the exponent and `h` is the hidden bit. Most floating point numbers
(except for negative and positive zeros) have the `h` bit set to one. However there is also a set of so-called denormal
numbers that are very small (near the zero) and have the hidden bit set to zero.
Performing some arithmetic operations on such numbers can cause severe performance degradation because they either
have to be handled in software (the operating system) or they take the hardware slow path.

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

You can run the Python benchmark script to plot how the time depends on the `flush` mode and `value`.
```bash
$ python3 denormals.py <path-to-executable>
```

You can find more about denormal floating point numbers here:
https://randomascii.wordpress.com/2012/05/20/thats-not-normalthe-performance-of-odd-floats/
