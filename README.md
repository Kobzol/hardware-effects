# Hardware effects
This repository demonstrates various hardware effects that may be very hard to explain
if you don't know the how the low-level CPU and OS architecture works.

Currently the following effects are demonstrated:

- cache aliasing
- false sharing 

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
