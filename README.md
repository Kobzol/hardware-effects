# Hardware effects
This repository demonstrates various hardware effects that may be very hard to explain
if you don't know the how the low-level CPU and OS architecture works. I try to create proof of concept
programs that are as small as possible for each effect.

Currently the following effects are demonstrated:

- branch misprediction
- branch target misprediction
- cache aliasing
- false sharing
- write combining

Every example directory has a README that explains the individual effect.

All programs print the duration of their execution to their standard error output (`stderr`).

I try to focus on effects that should be observable on all commodity (desktop/notebook) hardware,
so for example I left out the NUMA effect.

### Build
```bash
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make -j
```

If you want to use the Python benchmark scripts, you should also install the Python dependencies:
```bash
$ pip install -r requirements.txt
```
