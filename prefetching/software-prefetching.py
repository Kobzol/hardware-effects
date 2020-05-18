import os
import sys

import matplotlib.pyplot as plt
import seaborn

ROOT = os.path.realpath(os.path.join(os.path.dirname(__file__), "../"))
sys.path.append(ROOT)
from utils import benchmark

data = [
    ("Prefetch", [1]),
    ("Distance", [1, 5, 10, 15, 20]),
    ("Hint", [3, 2, 1, 0])
]

frame = benchmark(data, pin_to_cpu=True, repeat=2)


def draw_boxplot(data, **kw):
    df = data[["Time", "Distance"]]
    seaborn.lineplot(x=df["Distance"], y=df["Time"])


fg = seaborn.FacetGrid(frame, col='Hint')
ax = fg.map_dataframe(draw_boxplot)
ax.set(ylabel="Time [ms]", xlabel="Distance")
plt.show()
