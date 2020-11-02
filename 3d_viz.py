import pandas as pd
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt

table = pd.read_csv('results.csv')

table['Rating'] = table[' SensorsLeft']/table[' LowerBound']

table = table.groupby(cols=[' K', ' R_comm', ' R_capt']).mean()

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

ax.scatter(table[' K'], table[' R_comm'], table[' R_capt'], c=table['Rating'], cmap = 'coolwarm', depthshade=True)

ax.set_xlabel('K')
ax.set_ylabel('Rcom')
ax.set_zlabel('Rcapt')
plt.show()
