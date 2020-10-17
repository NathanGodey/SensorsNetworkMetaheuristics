import matplotlib.pyplot as plt
import networkx as nx

G = nx.Graph()
f = open("result.txt")
pos = []
for k,line in enumerate(f):
    G.add_node(k)
    pos.append([float(line.split(' ')[0]), float(line.split(' ')[1])])
nx.draw_networkx_nodes(G,pos=pos, node_color=[min(i,1)*'b'+(1-min(i,1))*'r' for i in range(len(pos))])
plt.show()
