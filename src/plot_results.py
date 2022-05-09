import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

f = open("dp_results.txt", "r")

df = pd.read_csv("dp_results.txt", sep=" ")

time = df.iloc[:,0]
position = df.iloc[:,1]
setpoint = df.iloc[:,2]

plt.plot(time, position)
plt.plot(time,setpoint)
plt.xlabel('Time [s]')
plt.ylabel('Position [%]')
plt.title('Position')
plt.ylim(0,1)
plt.show()