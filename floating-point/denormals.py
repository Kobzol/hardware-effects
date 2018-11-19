import os
import sys

import matplotlib.pyplot as plt
import seaborn

ROOT = os.path.realpath(os.path.join(os.path.dirname(__file__), "../"))
sys.path.append(ROOT)
from utils import benchmark


data = [
    ("Flush", [0, 1]),
    ("Value", ["0.00000001", "0.0000001", "0.000001", "0.00001", "0.0001", "0.001", "0.01", "0.1", "0", "1", "5",
               "10000000"])
]

frame = benchmark(data, pin_to_cpu=True)

g = seaborn.barplot(data=frame, x="Value", y="Time", hue="Flush")
plt.show()
