import os
import sys

import matplotlib.pyplot as plt
import seaborn

ROOT = os.path.realpath(os.path.join(os.path.dirname(__file__), "../"))
sys.path.append(ROOT)
from utils import benchmark

data = [
    ("Offset", [0, 1, 60, 61, 62, 63, 64])
]

frame = benchmark(data, pin_to_cpu=True)

ax = seaborn.barplot(data=frame, x="Offset", y="Time")
ax.set(ylabel="Time [ms]")
plt.show()
