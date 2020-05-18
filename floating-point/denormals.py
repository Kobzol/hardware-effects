import os
import sys

import matplotlib.pyplot as plt
import seaborn

ROOT = os.path.realpath(os.path.join(os.path.dirname(__file__), "../"))
sys.path.append(ROOT)
from utils import benchmark

data = [
    ("Flush", [0, 1]),
    ("Value", ["0.3", "0.2", "0.1", "0", "1", "10"])
]

frame = benchmark(data, pin_to_cpu=True)

ax = seaborn.barplot(data=frame, x="Value", y="Time", hue="Flush")
ax.set(ylabel="Time [ms]")
plt.show()
