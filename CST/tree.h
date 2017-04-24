#ifndef TREE_H
#define TREE_H

extern long int presentInTree (Node node);

extern long int connect (Node &node1, Node &node2, double edgeWeight, long int node2Pos);

extern void propagateNodeWeight (Node node1);

extern void propagateEdgeWeight (Node node1, Node node2, double edgeWeight);

extern void initializeTree ();

extern void getUser (Node &user);

extern void displayData ();

extern void displayNode (Node user, int i);

#endif
