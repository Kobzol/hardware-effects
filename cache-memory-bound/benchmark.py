import os
import sys

import matplotlib.pyplot as plt
import seaborn

ROOT = os.path.realpath(os.path.join(os.path.dirname(__file__), "../"))
sys.path.append(ROOT)
from utils import benchmark


data = [
    ("Increment", list(range(1, 20)) + [28, 32, 64, 128, 256, 512, 1024])
]

frame = benchmark(data, pin_to_cpu=True)

g = seaborn.barplot(data=frame, x="Increment", y="Time")
plt.show()
