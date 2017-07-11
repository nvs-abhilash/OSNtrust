// Authors : Karanjit Singh Gill & NVS Abhilash

#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <time.h>

#include "graph.h"
#include "tree.h"
#include "partition.h"

#define K 256

void updateCST (std::vector<Node> &CST, Edge e)
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

    std::vector<Node>::iterator present1 = presentInTree (CST, node1);
    std::vector<Node>::iterator present2 = presentInTree (CST, node2);

    if (present1 == CST.end ())
    {
        if (present2 == CST.end ())
        {
            connect (CST, node1, head, 0);
            connect (CST, node2, node1, edgeWeight);

            propagateNodeWeight (CST, node1);
            propagateNodeWeight (CST, node2);
        }
        else
        {
            connect (CST, node1, node2, edgeWeight);
            propagateNodeWeight (CST, node1);
        }
    }

    else
    {
        if (present2 == CST.end())
        {
            connect (CST, node2, node1, edgeWeight);
            propagateNodeWeight (CST, node2);
        }

        else
        {
            propagateEdgeWeight (CST, node2, node1, edgeWeight);
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

    clock_t t = clock();

    // Open the edges file
    char fileName[50];
    strcpy (fileName, argv[1]);

    std::fstream f (fileName);

    std::vector<Node> CST;
    // std::vector<int> userIdHash;
    initializeTree (CST);
    long int node1 = 0;
    long int node2 = 0;
    double weight = 0;

    long int i = 0;
    while (true)
    {
        std::cout << "\r" << i++;
        
        // Extract the edge
        f >> node1;
        // 2. Extract node2
        f >> node2;

        // 3. Extract the weight
        f >> weight;

        // Create the edge
        Edge e (node1, node2, weight);

        // Call the updateCST
        updateCST (CST, e);

        if (f.eof())
          break;
    }

    displayData (CST);

    f.close ();


    sortAmortized (CST, K);

    // Print data
    // std::cout << allUsers.size() << std::endl;
    // for (std::vector<Node>::size_type i = 0; i != allUsers.size(); ++i)
    //     displayNode (allUsers[i], i);

    partitionGraph (CST, K);

    writePartition (CST, K);

    t = clock() - t;

    std::cerr << "\nTime: " << (double)t / CLOCKS_PER_SEC << std::endl;

    return 0;
}
