import matplotlib.pyplot as plt
import networkx as nx
import numpy as np

G = nx.Graph()
f = open("result.txt")
pos = []
sensors = []
targets = []
neighbours_com = []
weights = []
for k,line in enumerate(f):
    G.add_node(k)
    attributes = line.split(' ')
    pos.append([float(attributes[1]), float(attributes[2])])
    is_sensor = int(attributes[3])
    id = int(attributes[0])
    current_neighbours = [int(s) for s in attributes[4][1:-1].split(',') if len(s)>0]
    weights.append(float(attributes[5][:-2]))
    for n in current_neighbours:
        G.add_edge(k,n)
    if is_sensor and id!=0:
        sensors.append(k)
    else:
        targets.append(k)

def draw_weights(pos, weights):
    max_weights = np.max(weights)
    min_weights = np.min(weights)
    weights_adapted = [int((w-min_weights+1)*255/(max_weights-min_weights+1)) for w in weights]
    nx.draw_networkx_nodes(G,nodelist = range(1, len(pos)),pos = pos, node_color=weights_adapted[1:], node_size = 50, node_shape = 'o', cmap="coolwarm")

#draw_weights(pos, weights)
#plt.show()


nx.draw_networkx_nodes(G,nodelist = targets,pos = pos, node_color='g', node_size = 15, node_shape = 'h')
nx.draw_networkx_nodes(G,nodelist = sensors,pos = pos, node_color='r', node_size = 30, node_shape = 'o')
nx.draw_networkx_nodes(G,nodelist = [0],pos=[pos[0]], node_color='b', node_size = 60, node_shape = 'v')

nx.draw_networkx_edges(G, pos, width = 1.5, edge_color = 'r')

plt.show()
