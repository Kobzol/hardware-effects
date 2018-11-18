import os
import sys

import matplotlib.pyplot as plt
import seaborn


ROOT = os.path.realpath(os.path.join(os.path.dirname(__file__), "../"))
sys.path.append(ROOT)
from utils import benchmark


data = [
    ("Count", [6]),
    ("Increment", list(range(1, 7)))
]

frame = benchmark(data, pin_to_cpu=True)

seaborn.barplot(data=frame, x="Count", y="Time", hue="Increment")
plt.show()
