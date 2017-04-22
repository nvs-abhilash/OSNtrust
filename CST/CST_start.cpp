#include <iostream>
#include <fstream>
#include <string.h>
#include "graph.h"
#include "tree.h"

void updateCST (Edge e)
{
    Node node1, node2, head;

    head.userId = HEAD_USER_ID;
    head.parentUserId = HEAD_PARENT_ID;

    head.nodeWeight = 0;
    head.edgeWeight = 0;

    node1.userId = e.getU();
    node1.parentUserId = HEAD_PARENT_ID;
    node1.nodeWeight = 1;
    node1.edgeWeight = 0;

    node2.userId = e.getV();
    node2.parentUserId = HEAD_PARENT_ID;
    node2.nodeWeight = 1;
    node2.edgeWeight = 0;

    double edgeWeight = e.getW();

    bool present1 = (presentInTree (node1) != -1) ? true : false;
    bool present2 = (presentInTree (node2) != -1) ? true : false;

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

    initializeTree ();
    long int node1 = 0;
    long int node2 = 0;
    double weight = 0;

    while (true)
    {
        // Extract the edge
        f >> node1;
        // 2. Extract node2
        f >> node2;

        // 3. Extract the weight
        f >> weight;

        // Create the edge
        Edge e (node1, node2, weight);

        // Call the updateCST
        updateCST (e);

        if (f.eof())
          break;
    }
    displayData ();

    f.close ();
    return 0;
}
