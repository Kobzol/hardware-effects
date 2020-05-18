import os
import sys

import matplotlib.pyplot as plt
import seaborn

ROOT = os.path.realpath(os.path.join(os.path.dirname(__file__), "../"))
sys.path.append(ROOT)
from utils import benchmark

data = [
    ("Threads", [1, 2, 3, 4]),
    ("Increment", [1, 8])
]

frame = benchmark(data)

ax = seaborn.barplot(data=frame, x="Threads", y="Time", hue="Increment")
ax.set(ylabel="Time [ms]")
plt.show()
