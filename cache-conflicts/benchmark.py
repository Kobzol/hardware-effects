import os
import sys

import matplotlib.pyplot as plt
import seaborn

ROOT = os.path.realpath(os.path.join(os.path.dirname(__file__), "../"))
sys.path.append(ROOT)
from utils import benchmark


data = [
    ("Count", [1, 2, 3, 4, 6, 7, 8, 9, 10, 15, 16, 17, 18]),
    ("Increment", [int(x, 16) for x in ["0x4", "0x40", "0x800", "0x1000"]])
]

frame = benchmark(data, pin_to_cpu=True)


def draw_boxplot(**kw):
    df = kw["data"][["Time", "Count"]]
    seaborn.lineplot(x=df["Count"], y=df["Time"])


fg = seaborn.FacetGrid(frame, col='Increment')
fg.map_dataframe(draw_boxplot)
plt.show()
