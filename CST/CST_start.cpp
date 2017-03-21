#include <iostream>
#include <fstream>
#include "graph.h"
#include "tree.h"

updateCST (Edge e)
{
    Node node1.userId = e.getU();
    Node node2.userId = e.getV();

    bool present1 = presentInTree (node1);
    bool present2 = presentInTree (node2);

    if (! present1)
    {
        if (! present2)
        {
            connect (node1, head, 0);
            connect (node2, node1, edgeWeight);

            propagateNodeWeight (node1);
            propagateNodeWeight (node2);
        }

        else
        {
            connect (node1, node2, edgeWeight);
            propagateNodeWeight (node1);
        }
    }

    else
    {
        if (! present2)
        {
            connect (node2, node1, edgeWeight);
            propagateNodeWeight (node2);
        }

        else
        {
            propagateEdgeWeight (node2, node1, edgeWeight);
        }
    }
}


int main ()
{
    // Open the edges file
    fstream f = open ("filename.dat", "r");

}
