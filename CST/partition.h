#ifndef PARTITION_H
#define PARTITION_H

extern std::vector <Node> sortAmortized (int k);

extern void unPropagate (std::vector <Node>, int nodeId, int childNodeId);

extern int getSuitableNode (std::vector<Node> CST, int partSize);

extern void assignPart (std::vector<Node> CST, int nodeIdx, int currPart);

extern void partitionGraph (std::vector<Node> CST, int k);

#endif
