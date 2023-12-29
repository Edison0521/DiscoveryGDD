import pandas as pd



class Counter(dict):
    def __missing__(self, key):
        return None


def read_labels(file: str) -> dict:
    """Read labels from a CSV file."""
    labels = {}
    fn = pd.read_csv(file,header=0)
    df = pd.DataFrame(fn)
    for i in range(len(df)):
        n1 = df['id'][i]
        n2 = df['entity_type'][i]
        labels[n1] = n2
    #print(labels)
    return labels

def edge_from_csv(file: str):
    edges = {}
    fn = pd.read_csv(file,header=0)
    df = pd.DataFrame(fn)
    for i in range(len(df)):
        e1 = int(df['start_id'][i])
        e2 = int(df['target_id'][i])
        e3 = int(df['relationship_type'][i])
        edges[(e1,e2)] = e3
    return edges


edgefile = 'edges.csv'
nodefile = 'nodes.csv'


e = edge_from_csv(edgefile)
n = read_labels(nodefile)


ndf = pd.read_csv(nodefile)
ndf = pd.DataFrame(ndf)

edf = pd.read_csv(edgefile)
edf = pd.DataFrame(edf)


edges = []
nodes = []


for i in range(len(n)):
    n_node = []
    n_node.append('v')
    n_node.append(ndf['id'][i])
    n_node.append(ndf['entity_type'][i])
    nodes.append(n_node)

for i in range(len(e)):
    n_edge = []
    n_edge.append("e")
    n_edge.append(int(edf['start_id'][i]))
    n_edge.append(int(edf['target_id'][i]))
    n_edge.append(e[(int(edf['start_id'][i]),int(edf['target_id'][i]))])
    edges.append(n_edge)

#print(e[(17,226331)])   
ne = pd.DataFrame(edges)
nn = pd.DataFrame(nodes)
#nn.to_csv('yago_o.lg',header=['#','t',1],index=0,sep=' ')
#ne.to_csv('yago_o.lg',header=False,index=0,mode='a',sep=' ')
e = Counter(e)
#print(e[(17,226331)])
#print(e[(0,355223)])

ns = []
subgraphfile = 'subgraph.csv'
subgraph = pd.read_csv(subgraphfile,header=None)
s = pd.DataFrame(subgraph)
for i in range(len(s)):
    #nes = []
    ex = 'e'
    e1 = int(s[0][i])
    e2 = int(s[1][i])
    es = e[(e1,e2)]
    if es != None:
        ns.append([ex,e1,e2,es])
ns = pd.DataFrame(ns)
nn.to_csv('subgraph.lg',header=['#','t',1],index=0,sep=' ')
ns.to_csv('subgraph.lg',header=False,index=0,mode='a',sep=' ')
