import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

array = []

with open('out') as f:
    for line in f: # read rest of lines
        array.append([float(line)])

plt.plot(array)
plt.savefig('t2.png')
