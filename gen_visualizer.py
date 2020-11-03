import matplotlib.pyplot as plt
import networkx as nx
import numpy as np

f = open('result_evol.txt')
avg_fitness = []
best_fitness = []
best_ind = []
line = f.readline()
while line:
    if "###" in line:
        line = f.readline()
        _,  best_fit, av, best_sco = line.split(' ')
        avg_fitness.append(float(av))
        best_fitness.append(int(best_fit.rstrip()))
        best_ind.append(int(best_sco))
    line = f.readline()

gen_range = range(1, len(avg_fitness)+1)
plt.plot(gen_range, avg_fitness, c='b', label = 'Average fitness')
plt.plot(gen_range, best_fitness, c='r', label = 'Best fitness')
plt.legend()
plt.xlabel("Number of generations")
plt.ylabel("Fitness")
plt.show()
