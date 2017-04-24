#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>

#include "graph.h"
#include "tree.h"
#include "partition.h"

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

    long int present1 = presentInTree (node1);
    long int present2 = presentInTree (node2);

    if (present1 == -1)
    {
        if (present2 == -1)
        {
            present1 = connect (node1, head, 0, 0);
            present2 = connect (node2, node1, edgeWeight, present1);

            propagateNodeWeight (node1);
            propagateNodeWeight (node2);
        }

        else
        {
            present1 = connect (node1, node2, edgeWeight, present2);
            propagateNodeWeight (node1);
        }
    }

    else
    {
        if (present2 == -1)
        {
            connect (node2, node1, edgeWeight, present1);
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

    std::vector<Node> allUsers = sortAmortized ();

    // Print data
    std::cout << allUsers.size() << std::endl;
    for (std::vector<Node>::size_type i = 0; i != allUsers.size(); ++i)
        displayNode (allUsers[i], i);
    return 0;
}
