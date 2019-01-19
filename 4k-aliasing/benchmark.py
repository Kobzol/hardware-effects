import os
import sys

import matplotlib.pyplot as plt
import seaborn

ROOT = os.path.realpath(os.path.join(os.path.dirname(__file__), "../"))
sys.path.append(ROOT)
from utils import benchmark


data = [

    ("Increment", [4000, 4080, 4084, 4088, 4092, 4096, 4100, 5000])
]

frame = benchmark(data, pin_to_cpu=True)

g = seaborn.barplot(data=frame, x="Increment", y="Time")
plt.show()
