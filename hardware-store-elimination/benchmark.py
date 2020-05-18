import os
import sys

import matplotlib.pyplot as plt
import seaborn

ROOT = os.path.realpath(os.path.join(os.path.dirname(__file__), "../"))
sys.path.append(ROOT)
from utils import benchmark

data = [
    ("Size", [16 * 1024,
              32 * 1024,
              128 * 1024,
              256 * 1024,
              512 * 1024,
              1 * 1024 * 1024,
              2 * 1024 * 1024,
              4 * 1024 * 1024,
              8 * 1024 * 1024,
              32 * 1024 * 1024
              ]),
    ("Value", ["0", "1", "2"])
]

frame = benchmark(data, pin_to_cpu=True, y_axis="Bandwidth")
frame["Size"] = frame["Size"] / (1024 * 1024)

ax = seaborn.lineplot(data=frame, x="Size", y="Bandwidth", hue="Value")
ax.set(xlabel="Size [MiB]", ylabel="Bandwidth [MiB/s]")
plt.show()
