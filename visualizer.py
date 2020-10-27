import matplotlib.pyplot as plt
import networkx as nx

G = nx.Graph()
f = open("result.txt")
pos = []
sensors = []
targets = []
neighbours_com = []
for k,line in enumerate(f):
    G.add_node(k)
    pos.append([float(line.split(' ')[0]), float(line.split(' ')[1])])
    is_sensor = int(line.split(' ')[2])
    current_neighbours = [int(s) for s in line.split(' ')[3][1:-2].split(',') if len(s)>0]
    for n in current_neighbours:
        G.add_edge(k,n)
    if is_sensor and k!=0:
        sensors.append(k)
    else:
        targets.append(k)



nx.draw_networkx_nodes(G,nodelist = targets,pos = pos, node_color='g', node_size = 15, node_shape = 'h')
nx.draw_networkx_nodes(G,nodelist = sensors,pos = pos, node_color='r', node_size = 30, node_shape = 'o')
nx.draw_networkx_nodes(G,nodelist = [0],pos=[pos[0]], node_color='b', node_size = 60, node_shape = 'v')

nx.draw_networkx_edges(G, pos, width = 1.5, edge_color = 'r')

plt.show()
