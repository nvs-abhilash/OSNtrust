#include <string>
#include <iostream>
#include <vector>
#include "graph.h"
#include <algorithm>

// Initialize Tree: add the default head node
void initializeTree (std::vector<Node> &CST)
{
    Node *head = new Node;

    head -> userId = -2;
    head -> parentUserId = -1;
    head -> nodeWeight = 0;
    head -> edgeWeight = 0;

    CST.push_back (*head);

    delete head;
}

// Normal search function to search for the userId
std::vector<Node>::iterator presentInTree (std::vector<Node> &CST, Node node)
{
    for (std::vector<Node>::iterator it = CST.begin(); it != CST.end(); it++)
      if (it->userId == node.userId)
        return it;

    return CST.end();
}

// Connect: A simple write operation in the file.
void connect (std::vector<Node> &CST, Node &node1, Node &node2, double edgeWeight)
{
    node1.parentUserId = node2.userId;
    node1.edgeWeight = edgeWeight;

    CST.push_back (node1);
}

std::vector<Node>::iterator getNode (std::vector<Node> &CST, int nodeId)
{
  //Try to optimize this
  std::vector<Node>::iterator node;
  for(node = CST.begin (); node != CST.end (); node ++)
  {
    if(node->userId == nodeId)
      return node;
  }

  return CST.end();
}

// propagateNodeWeight: This function updates the weight of the parent
void propagateNodeWeight (std::vector<Node> &CST, Node node)
{
    // Find the parent of the node
    std::vector<Node>::iterator parent = getNode (CST, node.parentUserId);

    // while (parent->userId != HEAD_PARENT_ID)
    while (parent != CST.end ())
    {
      // Find the parent and update the nodeWeight.
      parent->nodeWeight ++;
      // parent = user->parentUserId;
      parent = getNode (CST, parent->parentUserId);
    }
}

long int getDepth (std::vector<Node> &CST, Node node)
{
    std::vector<Node>::iterator treeNode;
    long int depth = 0;
    int nodeUserId = node.userId;

    while (nodeUserId != HEAD_USER_ID)
    {
        treeNode = getNode (CST, nodeUserId);
        nodeUserId = treeNode->parentUserId;
        depth ++;
    }

    return depth;
}

void propagateEdgeWeight (std::vector<Node> &CST, Node node1, Node node2, double edgeWeight)
{
    long int node1Depth, node2Depth, minDepth;

    std::vector<Node>::iterator parentNode1 = getNode (CST, node1.userId);
    std::vector<Node>::iterator parentNode2 = getNode (CST, node2.userId);

    node1Depth = getDepth (CST, node1);
    node2Depth = getDepth (CST, node2);

    if(node1Depth == node2Depth)
      minDepth = node1Depth;
    else
      minDepth = (node1Depth < node2Depth) ? node1Depth : node2Depth;

    while (node1Depth != minDepth)
    {
        //update node1's parent
        parentNode1->edgeWeight += edgeWeight;
        parentNode1 = getNode (CST, parentNode1->parentUserId);
        node1Depth --;
    }

    while (node2Depth != minDepth)
    {
        //update node2's parent
        parentNode2->edgeWeight += edgeWeight;
        parentNode2 = getNode (CST, parentNode2->parentUserId);
        node2Depth --;
    }

    while (true)
    {
        if (parentNode2->userId == parentNode1->userId)
        {
            parentNode1->edgeWeight += 2*edgeWeight;
            break;
        }
        else
        {
          parentNode1->edgeWeight += edgeWeight;
          parentNode1 = getNode (CST, parentNode1->parentUserId);

          parentNode2->edgeWeight += edgeWeight;
          parentNode2 = getNode (CST, parentNode2->parentUserId);
        }
    }
}

void displayNode (Node user)
{
    std::cout << "-------------------" << std::endl;
    std::cout << "UserId: " << user.userId << std::endl;
    std::cout << "ParentUserId: " << user.parentUserId << std::endl;
    std::cout << "NodeWeight: " << user.nodeWeight << std::endl;
    std::cout << "EdgeWeight: " << user.edgeWeight << std::endl;
    std::cout << std::endl;
}

void displayData (std::vector<Node> &CST)
{
    // Open the file
    std::vector<Node>::iterator node;
    for (node = CST.begin (); node != CST.end (); node ++)
    {
      displayNode (*node);
    }
}
