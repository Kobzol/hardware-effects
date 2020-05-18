import multiprocessing
import os
import sys

import matplotlib.pyplot as plt
import seaborn

ROOT = os.path.realpath(os.path.join(os.path.dirname(__file__), "../"))
sys.path.append(ROOT)
from utils import benchmark

data = [
    ("NonTemporal", [0, 1]),
    ("Threads", list(range(1, multiprocessing.cpu_count() + 1)))
]

frame = benchmark(data)

ax = seaborn.barplot(data=frame, x="Threads", y="Time", hue="NonTemporal")
ax.set(ylabel="Time [ms]")
plt.show()
