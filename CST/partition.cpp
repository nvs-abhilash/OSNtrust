#include <fstream>
#include <string.h>
#include "graph.h"
#include "tree.h"
#include <bits/stdc++.h>
#include <vector>

#define HEAD_PARENT_ID -1

const char FILE_NAME[] = "CST_out.bin";

// Compares two intervals according to staring times.
bool compare(Node i1, Node i2)
{
    return ((i1.edgeWeight)/(i1.nodeWeight) < (i2.edgeWeight) / (i2.nodeWeight));
}

std::vector <Node> sortAmortized ()
{
    Node *user = new Node;

    std::ifstream fileIn (FILE_NAME, std::ios::in | std::ios::binary);

    std::vector<Node> allUsers;

    // Load all the users
    fileIn.seekg (0, std::ios::beg);

    while (fileIn.read ((char*) user, sizeof (Node)))
        allUsers.push_back(*user);

    // Call the sorting algorithm.
    sort(allUsers.begin(), allUsers.end(), compare);

    return allUsers;
}

/*
* @params: CST node array, node id(whose subtree contains the child node) and the child node id
* @returns: edge-cut cost
*/
void unPropagate (std::vector<Node> CST, int nodeId, int childNodeId)
{
  int parentPos = -1, childPos = -1;

  while(nodeId != HEAD_PARENT_ID)
  {
    for(int i = 0; i < (int)CST.size(); i++)
    {
      if(CST[i].userId == nodeId)
        parentPos = i;
      else if(CST[i].userId == childNodeId)
        childPos = i;

      if(parentPos >= 0 && childPos >= 0)
        break;
    }
    CST[parentPos].nodeWeight -= CST[childPos].nodeWeight;
    nodeId = CST[parentPos].parentUserId;
  }
}
