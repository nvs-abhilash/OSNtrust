#ifndef TREE_H
#define TREE_H

#include <vector>

extern void initializeTree (std::vector<Node> &CST);

extern std::vector<Node>::iterator presentInTree (std::vector<Node> &CST, Node node);

extern void connect (std::vector<Node> &CST, Node &node1, Node &node2, double edgeWeight);

extern std::vector<Node>::iterator getNode (std::vector<Node> &CST, int nodeId);

extern void propagateNodeWeight (std::vector<Node> &CST, Node node);

extern void propagateEdgeWeight (std::vector<Node> &CST, Node node1, Node node2, double edgeWeight);

extern long int getDepth (std::vector<Node> &CST, Node node);

extern void displayNode (Node user);

void displayData (std::vector<Node> &CST);

#endif
