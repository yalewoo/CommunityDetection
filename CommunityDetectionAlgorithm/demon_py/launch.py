import networkx as nx
import Demon as D
import sys

G = nx.Graph()
file = open(sys.argv[1], "r")
for row in file:
    part = row.strip().split()
    G.add_edge(int(part[0]), int(part[1]))

# Example use of DEMON. Parameter discussed in the paper.
CD = D.Demon()
CD.execute(G)
