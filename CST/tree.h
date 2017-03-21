#ifndef TREE_H
#define TREE_H

extern bool presentInTree (Node node);

extern void connect (Node node1, Node node2, double edgeWeight);

extern void propagateNodeWeight (Node node1);

#endif
