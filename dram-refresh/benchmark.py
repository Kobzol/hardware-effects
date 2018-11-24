import os
import pandas
import sys

import matplotlib.pyplot as plt
import seaborn

ROOT = os.path.realpath(os.path.join(os.path.dirname(__file__), "../"))
sys.path.append(ROOT)
from utils import run_repeatable, get_executable


data = []
for res in run_repeatable(get_executable(), 1, [], True):
    data += [float(time) for time in res.split("\n")]

# remove outliers (https://stackoverflow.com/a/43279492/1107768)
frame = pandas.DataFrame({'duration': data})
q1 = frame['duration'].quantile(0.25)
q3 = frame['duration'].quantile(0.75)
iqr = q3 - q1

q1 = q1 - 1.5 * iqr
q3 = q3 + 1.5 * iqr

mask = frame['duration'].between(q1, q3, inclusive=True)
filtered = frame.loc[mask, 'duration']

g = seaborn.distplot(filtered, kde=False)
g.set(xlabel="Duration between slow iterations (ns)")
plt.show()
