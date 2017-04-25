#include <iostream>
#include <fstream>
#include <string.h>
#include "graph.h"
#include "tree.h"
#include <bits/stdc++.h>
#include <vector>

#define HEAD_PARENT_ID -1

const char FILE_NAME[] = "CST_out.bin";

int *partHash;
int *part;

// Compares two intervals according to staring times.
bool compare(Node i1, Node i2)
{
    return ((i1.edgeWeight)/(i1.nodeWeight) < (i2.edgeWeight) / (i2.nodeWeight));
}

std::vector <Node> sortAmortized (int k)
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

    // Make a tree

    //Hash partition
    partHash = new int[(int)allUsers.size()] ();
    part = new int[k] (); 

    delete user;
    return allUsers;
}


/*
* @params: CST node array, node id(whose subtree contains the child node) and the child node id
* @returns: edge-cut cost
*/
void unPropagate (std::vector<Node> CST, int childNodeId, int nodeId)
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

int getSuitableNode (std::vector<Node> CST, int partSize)
{
  for(std::vector<int>::size_type i = 0; i != CST.size(); i++)
      if (partHash[i] == 0 && CST[i].nodeWeight <= partSize)
        return i;

  return -1;
}

void assignPart (std::vector<Node> CST, int nodeIdx, int currPart)
{
  part[currPart] += CST[nodeIdx].nodeWeight;
  partHash[nodeIdx] = currPart;

  if(CST[nodeIdx].nodeWeight == 1)
    return;
  
  for(std::vector<int>::size_type i = 0; i != CST.size(); i++) 
  {
    if (CST[i].parentUserId == CST[nodeIdx].userId)
    {
      assignPart (CST, i, currPart);
    }
  }
}

void partitionGraph (std::vector<Node> CST, int k)
{
  int currPart = 1;
  int partSize = ((int) CST.size() / k);
  int nodeIdx;

  while (currPart <= k)
  {
    nodeIdx = getSuitableNode (CST, partSize - part[currPart]);
    if (nodeIdx != -1)
    {
      assignPart (CST, nodeIdx, currPart);
      unPropagate (CST, CST[nodeIdx].userId, CST[nodeIdx].parentUserId);
    }

    if (((partSize - part[currPart]) <= (partSize / 2)) && nodeIdx == -1)
      currPart ++;
  }
}