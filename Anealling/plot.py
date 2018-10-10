import matplotlib.pyplot as plt

array = []

with open('out') as f:
    for line in f: # read rest of lines
        array.append([float(line)])

plt.plot(array)
plt.show()
