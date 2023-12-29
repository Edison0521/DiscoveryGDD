import re

point = {}
edge = {}

def load(path):
    with open(path) as f:
        curNum = int(1)
        for line in f:
            line = line.replace('\n', '').replace('\r', '')
            s = line.split(' ')
            if len(s) == 0:
                continue
            s1 = s[0]
            if(s1 == str(curNum) + ":"):
                if(curNum == 1):
                    curNum = curNum + 1
                    continue
                with open("queryGraph/purePattern" + str(curNum-2) + ".txt", "w") as pattern:
                    for (curEdge, rel) in edge.items():
                        edgeNode = curEdge.split(' ')
                        writeline = "(" + edgeNode[0] + ":" + point[
                            edgeNode[0]] + ")" + "-" + "[" + rel + "]" + "->" + "(" + \
                                    edgeNode[1] + ":" + point[edgeNode[1]] + ")"
                        pattern.write(writeline)
                        pattern.write("\n")

                pattern.close()
                point.clear()
                edge.clear()

                curNum = curNum + 1
                continue


            if(len(s) <= 1):
                continue
            if(s1 == "#"):
                continue

            if(s1 == "v"):
                point[s[1]] = s[2]
            else:
                edges = s[1] + " " +  s[2]
                edge[edges] = s[3]

    with open("queryGraph/purePattern" + str(curNum-2) + ".txt", "w") as pattern:
        for (curEdge, rel) in edge.items():
            edgeNode = curEdge.split(' ')
            writeline = "(" + edgeNode[0] + ":" + point[edgeNode[0]] + ")" + "-" + "[" + rel + "]" + "->" + "(" + \
                        edgeNode[1] + ":" + point[edgeNode[1]] + ")"
            pattern.write(writeline)
            pattern.write("\n")

if __name__ == '__main__':
    load('Output.txt')
