#ifndef PARTITION_H
#define PARTITION_H

extern void sortAmortized (std::vector<Node> &CST, int k);

extern void unPropagate (std::vector <Node> &CST, int nodeId, int childNodeId);

extern int getSuitableNode (std::vector<Node> &CST, int partSize);

extern void assignPart (std::vector<Node> &CST, int nodeIdx, int currPart);

extern void partitionGraph (std::vector<Node> &CST, int k);

extern void writePartition (std::vector<Node> &CST, int k);

#endif
