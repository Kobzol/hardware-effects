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
              36 * 1024,
              66 * 1024,
              128 * 1024,
              256 * 1024,
              512 * 1024,
              1 * 1024 * 1024,
              2 * 1024 * 1024,
              3 * 1024 * 1024,
              4 * 1024 * 1024,
              6 * 1024 * 1024,
              10 * 1024 * 1024,
              20 * 1024 * 1024,
              30 * 1024 * 1024,
              50 * 1024 * 1024])
]

frame = benchmark(data, pin_to_cpu=True, y_axis="Bandwidth")


seaborn.barplot(data=frame, x="Size", y="Bandwidth")
plt.show()
