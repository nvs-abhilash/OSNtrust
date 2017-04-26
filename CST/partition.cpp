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

void sortAmortized (std::vector<Node> &CST, int k)
{
    sort(CST.begin(), CST.end(), compare);

    //Hash partition
    partHash = new int[(int)CST.size()] ();
    part = new int[k] ();
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
  partHash[nodeIdx] = currPart;

  if(CST[nodeIdx].nodeWeight == 1)
    return;

  for(std::vector<int>::size_type i = 0; i != CST.size(); i++)
  {
    if ((CST[i].parentUserId == CST[nodeIdx].userId) && partHash[i] == 0)
    {
      assignPart (CST, i, currPart);
    }
  }
}

void partitionGraph (std::vector<Node> &CST, int k)
{
  int currPart = 1;
  int partSize = ceil ((CST.size() - 1) / (float) k);
  int threshold = ceil (partSize / (float) 2);
  int nodeIdx = -1, edgeCut = 0;

  while(true)
  {
    currPart = 1;
    nodeIdx = -1;
    while(nodeIdx == -1)
    {
      if(part[currPart - 1] >= threshold)
        nodeIdx = -1;
      else
        nodeIdx = getSuitableNode (CST, partSize - part[currPart - 1]);

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
        std::cout << "\n\nEdge Cut = " << edgeCut << std::endl;
        return;
      }
    }
    part[currPart - 1] += CST[nodeIdx].nodeWeight;
    edgeCut += CST[nodeIdx].edgeWeight;
    assignPart (CST, nodeIdx, currPart);
    unPropagate (CST, CST[nodeIdx].nodeWeight, CST[nodeIdx].parentUserId);
  }
}

void writePartition (std::vector<Node> &CST, int k)
{
  std::ofstream *fPtr = new std::ofstream[k];

  char **fileName = new char *[k];
  for (int i = 0; i < k; i++)
    fileName[i] = new char[50];

  for (int i = 0; i < k; i++)
  {
    sprintf (fileName[i], "part_%d.txt", i + 1);
    fPtr[i].open (fileName[i], std::ios::out);
  }

  for (std::vector<int>::size_type i = 0; i != CST.size(); i++)
  {
    if (partHash[i] >= 1 && partHash[i] <= k)
      fPtr[partHash[i] - 1] << CST[i].userId << std::endl;
  }

  for (int i = 0; i < k; i++)
    fPtr[i].close();

  for (int i = 0; i < k; i++)
    delete[] fileName[i];
  delete[] fileName;

  delete[] fPtr;
}
