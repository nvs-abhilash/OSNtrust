#include <iostream>
#include <fstream>
#include <string.h>
#include "graph.h"
#include "tree.h"

void updateCST (Edge e)
{
    Node node1, node2, head;

    head.userId = -2;
    head.parentUserId = -1;

    head.nodeWeight = 0;
    head.edgeWeight = 0;

    node1.userId = e.getU();
    node1.parentUserId = -1;
    node1.nodeWeight = 1;
    node1.edgeWeight = 0;

    node2.userId = e.getV();
    node2.parentUserId = -1;
    node2.nodeWeight = 1;
    node2.edgeWeight = 0;

    double edgeWeight = e.getW();

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
            // propagateEdgeWeight (node2, node1, edgeWeight);
        }
    }
}

int main (int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./main <file_name.dat>\n";
        return -1;
    }

    // Open the edges file
    char fileName[50];
    strcpy (fileName, argv[1]);

    std::fstream f (fileName);
    // std::string line;

    while (! f.eof())
    {
        // Extract the edge

        // 1. Extract node1
        long int node1 = 0;
        f >> node1;

        // 2. Extract node2
        long int node2 = 0;
        f >> node2;

        // 3. Extract the weight
        double weight = 0;
        f >> weight;

        // Create the edge
        Edge e (node1, node2, weight);

        // Call the updateCST
        updateCST (e);

    }

    return 0;
}
