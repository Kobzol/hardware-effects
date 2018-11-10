import os
import platform
import subprocess
import sys
import itertools

import pandas
import seaborn
import matplotlib.pyplot as plt

if len(sys.argv) < 2:
    print("Usage: python3 benchmark.py <path-to-executable>")
    exit(1)

EXECUTABLE = os.path.realpath(sys.argv[1])

COUNTS = [1, 2, 3, 4, 6, 7, 8, 9, 10, 15, 16, 17, 18]
INCREMENTS = ["0x4", "0x40", "0x800", "0x1000"]
INPUTS = itertools.product(COUNTS, INCREMENTS)

frame = pandas.DataFrame(columns=["Count", "Increment", "Time"])

last_count = 1
for (count, increment) in INPUTS:
    if last_count != count:
        last_count = count
        print()

    if platform.system() == "Windows":
        args = ["start", "/affinity", "0x1"]
    else:
        args = ["taskset", "0x1"]

    args += [EXECUTABLE, str(count), str(int(increment, 16))]
    times = []

    for i in range(4):
        res = subprocess.run(args,
                             stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE)
        times.append(float(res.stderr.decode().strip()))
    output = sum(times) / len(times)

    intro = "Count: {}, increment: {}".format(count, increment)
    print("{:<30} {:>16.1f} us".format(intro, output))
    frame = frame.append({
        "Count": count,
        "Increment": increment,
        "Time": output
    }, ignore_index=True)


def draw_boxplot(**kw):
    df = kw["data"][["Time", "Count"]]
    seaborn.lineplot(x=df["Count"], y=df["Time"])


fg = seaborn.FacetGrid(frame, col='Increment')
fg.map_dataframe(draw_boxplot)
plt.show()
