import os
import sys

import matplotlib.pyplot as plt
import seaborn

ROOT = os.path.realpath(os.path.join(os.path.dirname(__file__), "../"))
sys.path.append(ROOT)
from utils import benchmark


data = [
    ("Count", [128, 512, 768, 1024, 1200, 1600, 1800]),
    ("Stride", [1, 8, 16, 64, 128, 256])
]

frame = benchmark(data, pin_to_cpu=True, repeat=2)


def draw_boxplot(**kw):
    df = kw["data"]
    seaborn.barplot(x=df["Count"], y=df["Time"])


fg = seaborn.FacetGrid(frame, col="Stride")
fg.map_dataframe(draw_boxplot)
plt.show()
