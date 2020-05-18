import os
import sys

import matplotlib.pyplot as plt
import seaborn

ROOT = os.path.realpath(os.path.join(os.path.dirname(__file__), "../"))
sys.path.append(ROOT)
from utils import benchmark

data = [
    ("Version", [0, 1]),
]

frame = benchmark(data, pin_to_cpu=True)

ax = seaborn.barplot(x=frame["Version"], y=frame["Time"])
ax.set(ylabel="Time [ms]")
plt.show()
