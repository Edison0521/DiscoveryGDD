import os
import time
import warnings
from collections import defaultdict

import click
import numpy as np
from sklearn import metrics
import pandas as pd

from btw.graph import Graph, JunctionTree
from btw.inference import BPModel, JuncTreeModel
from btw.sampling import sample



edges = '../data/IMDB_2m/e1.csv'
nodes = '../data/IMDB_2m/nodes.csv'


graph = Graph()
graph.from_csv(edges)
#print('The given graph has {} edges.'.format(graph.num_edges()))
t1 = graph.get_neighbors(0)
print(t1)
t = graph.get_neighbors(1).difference(t1)
print(t)

