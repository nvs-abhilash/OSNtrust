#include <iostream>
#include <fstream>
#include <string.h>
#include "graph.h"
#include "tree.h"
#include <bits/stdc++.h>
#include <vector>

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
void unPropagate (std::vector<Node> &CST, int nodeWeight, int nodeId)
{
  int i;

  while(nodeId != HEAD_PARENT_ID)
  {
    for(i = 0; i < (int)CST.size(); i++)
    {
      if(CST[i].userId == nodeId)
      {
        CST[i].nodeWeight -= nodeWeight;
        nodeId = CST[i].parentUserId;
        break;
      }
    }
  }
}

int getSuitableNode (std::vector<Node> &CST, int partSize)
{
  for(std::vector<int>::size_type i = 0; i != CST.size(); i++)
      if (partHash[i] == 0 && CST[i].nodeWeight <= partSize && CST[i].userId != HEAD_USER_ID)
        return i;

  return -1;
}

void assignPart (std::vector<Node> &CST, int nodeIdx, int currPart)
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

void partitionGraph (std::vector<Node> &CST, int k)
{
  int currPart = 1;
  int partSize = ceil ((CST.size() - 1) / (float) k);
  int threshold = partSize / 2;
  int nodeIdx = -1;

  // while (currPart <= k)
  // {
  //   nodeIdx = getSuitableNode (CST, partSize - part[currPart]);
  //   if (nodeIdx != -1)
  //   {
  //     assignPart (CST, nodeIdx, currPart);
  //     unPropagate (CST, CST[nodeIdx].nodeWeight, CST[nodeIdx].parentUserId);
  //   }
  //
  //   if (((partSize - part[currPart]) < (partSize / 2)) && nodeIdx == -1)
  //     currPart ++;
  // }

  while(true)
  {
    currPart = 1;
    nodeIdx = -1;
    while(nodeIdx == -1)
    {
      if(part[currPart] >= threshold)
        nodeIdx = -1;
      else
        nodeIdx = getSuitableNode (CST, partSize - part[currPart]);

      if(nodeIdx != -1)
        break;
      currPart ++;

      if(currPart > k)
      {
        //Place rem nodes
        for (std::vector<int>::size_type i = 0; i != CST.size(); i++)
        {
          if (partHash[i] == 0 && CST[i].userId != HEAD_USER_ID)
          {
            partHash[i] = k;//last partition
          }
        }
        return;
      }
    }
    assignPart (CST, nodeIdx, currPart);
    unPropagate (CST, CST[nodeIdx].nodeWeight, CST[nodeIdx].parentUserId);
  }
}

void writePartition (std::vector<Node> &CST, int k)
{
  // // Open all the partition files
  // char **fNames = new char *[k];
  // for (int i = 0; i < k; i++)
  // {
  //   fNames[i] = new char[50];
  //   if (fNames[i] == NULL)
  //   {
  //     std::cerr << "Error allocating memory";
  //     return;
  //   }

  //   sprintf(fNames[i], "part_%d.txt", i + 1);
  // }

  // std::ofstream *fPtr = new std::ofstream [k];
  // for (int i = 0; i < k; i++)
  //   fPtr[i].open (fNames[i], std::ios::out | std::ios::binary);

  std::ofstream fPtr1, fPtr2, fPtr3;

  fPtr1.open ("part_1.bin", std::ios::out | std::ios::binary);
  fPtr2.open ("part_2.bin", std::ios::out | std::ios::binary);
  fPtr3.open ("part_3.bin", std::ios::out | std::ios::binary);

  for (std::vector<int>::size_type i = 0; i != CST.size(); i++)
  {
    if (partHash[i] == 1)
    {
      std::cout << "Part 1:" << std::endl;
      displayNode (CST[i], i);
      fPtr1.write ((char *) &CST[i], sizeof (CST[i]));
    }
    else if(partHash[i] == 2)
    {
      std::cout << "Part 2: " << std::endl;
      displayNode (CST[i], i);
      fPtr2.write ((char *) &CST[i], sizeof (CST[i]));
    }
    else if(partHash[i] == 3)
    {
      std::cout << "Part 3: " << std::endl;
      displayNode (CST[i], i);
      fPtr3.write ((char *) &CST[i], sizeof (CST[i]));
    }
  }


  // for (int i = 0; i < k; i++)
  // {
  //   fPtr[i].close();
  //   delete fNames[i];
  // }
  // delete fPtr;

}
