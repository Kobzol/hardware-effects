import itertools
import os
import platform
import subprocess
import sys

import __main__
import pandas


def get_args(executable, parameters, pin_to_cpu):
    args = []

    if pin_to_cpu:
        if platform.system() == "Windows":
            args += ["start", "/affinity", "0x1"]
        else:
            args += ["taskset", "0x1"]

    args.append(executable)
    args += (str(p) for p in parameters)

    return args


def benchmark(input_data, pin_to_cpu=False, repeat=5, y_axis="Time"):
    if len(sys.argv) < 2:
        print("Usage: python3 {} <path-to-executable>".format(__main__.__file__))
        exit(1)

    executable = os.path.realpath(sys.argv[1])

    keys = [d[0] for d in input_data]
    inputs = itertools.product(*[d[1] for d in input_data])

    frame = pandas.DataFrame(columns=keys + [y_axis])

    for values in inputs:
        args = get_args(executable, values, pin_to_cpu)
        times = []

        for i in range(repeat):
            res = subprocess.run(args,
                                 stdout=subprocess.PIPE,
                                 stderr=subprocess.PIPE)
            times.append(float(res.stderr.decode().strip()))
        value = sum(times) / len(times)

        data = ["{}: {}".format(key, value) for (key, value) in zip(keys, values)]
        data.append("{}: {}".format(y_axis, value))

        print(", ".join(data))

        result = {key: values[index] for (index, key) in enumerate(keys)}
        result[y_axis] = value

        frame = frame.append(result, ignore_index=True)
    return frame
