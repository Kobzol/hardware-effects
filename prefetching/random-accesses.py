import os
import sys

import matplotlib.pyplot as plt
import seaborn

ROOT = os.path.realpath(os.path.join(os.path.dirname(__file__), "../"))
sys.path.append(ROOT)
from utils import benchmark

KiB = 1024
MiB = KiB ** 2

data = [
    ("Shuffle", [0, 1]),
    ("Size", [4 * KiB, 16 * KiB, 32 * KiB, 64 * KiB, 128 * KiB, 256 * KiB, 512 * KiB,
              1 * MiB, 2 * MiB, 4 * MiB, 16 * MiB, 32 * MiB, 64 * MiB])
]

frame = benchmark(data, pin_to_cpu=True)

ax = seaborn.lineplot(x="Size", y="Time", hue="Shuffle", data=frame)
ax.set(ylabel="Time [µs]")
plt.show()
