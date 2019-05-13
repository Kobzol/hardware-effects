import itertools
import os
import platform
import subprocess
import sys

import __main__
import pandas


def get_executable():
    return os.path.realpath(sys.argv[1])


def get_args(executable, parameters, pin_to_cpu):
    args = []

    if pin_to_cpu:
        if platform.system() == "Windows":
            args += ["start", "/affinity", "0x1"]
        elif platform.system() == "Linux":
            args += ["taskset", "0x1"]

    args.append(executable)
    args += (str(p) for p in parameters)

    return args


def run_repeatable(executable, repeat, parameters, pin_to_cpu):
    args = get_args(executable, parameters, pin_to_cpu)

    for i in range(repeat):
        res = subprocess.run(args,
                             stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE)
        yield res.stderr.decode().strip()


def benchmark(input_data, pin_to_cpu=False, repeat=5, y_axis="Time"):
    if len(sys.argv) < 2:
        print("Usage: python3 {} <path-to-executable>".format(__main__.__file__))
        exit(1)

    executable = get_executable()

    keys = [d[0] for d in input_data]
    inputs = itertools.product(*[d[1] for d in input_data])

    frame = pandas.DataFrame(columns=keys + [y_axis])

    for values in inputs:
        times = [float(res) for res in run_repeatable(executable, repeat, values, pin_to_cpu)]
        value = sum(times) / len(times)

        data = ["{}: {}".format(key, value) for (key, value) in zip(keys, values)]
        data.append("{}: {}".format(y_axis, value))

        print(", ".join(data))

        result = {key: values[index] for (index, key) in enumerate(keys)}
        result[y_axis] = value

        frame = frame.append(result, ignore_index=True)
    return frame
