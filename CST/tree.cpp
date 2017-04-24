#include <fstream>
#include <string>
#include <iostream>
#include "graph.h"

const char FILE_NAME[] = "CST_out.bin";

// Initialize Tree: add the default head node
void initializeTree ()
{
    std::ofstream fileOut (FILE_NAME, std::ios::out | std::ios::binary);

    Node *head = new Node;

    head -> userId = -2;
    head -> parentUserId = -1;

    head -> nodeWeight = 0;
    head -> edgeWeight = 0;

    fileOut.write ((char*)head, sizeof(Node));

    delete head;
    fileOut.close();
}

// Normal search function to search for the userId
long int presentInTree (Node node)
{
    // Open the file
    std::ifstream fileIn (FILE_NAME, std::ios::in | std::ios::binary);

    Node *user = new Node;

    while (fileIn.read ((char*) user, sizeof (Node)))
    {
        // Extract in the nodeUserId
        if (user->userId == node.userId)
        {
            long int pos = fileIn.tellg ();
            fileIn.close();
            delete user;
            return pos;
        }
    }
    delete user;
    fileIn.close();
    return -1;
}

// Connect: A simple write operation in the file.
long int connect (Node &node1, Node &node2, double edgeWeight, long int node2Pos)
{
    // Find Node2
    // Open the file
    std::fstream fileInOut (FILE_NAME, std::ios::in | std::ios::out | std::ios::binary);

    if (node2.userId != HEAD_USER_ID)
    {
        fileInOut.seekg (node2Pos);
        Node *readNode = new Node;

        fileInOut.read ((char*) readNode, sizeof (Node));
        if (node2.userId == readNode->userId)
        {
            readNode->edgeWeight += edgeWeight;
            fileInOut.seekp (node2Pos);

            fileInOut.write ((char*) readNode, sizeof (Node));
        }
        delete readNode;
    }

    // Write the new node
    fileInOut.seekp (0, std::ios::end);
    node2Pos = fileInOut.tellp ();
    node1.parentUserId = node2.userId;
    node1.edgeWeight = 0;

    fileInOut.write ((char *)&node1, sizeof(Node));

    fileInOut.close();
    return node2Pos;
}

void getUser (Node &user)
{
  std::ifstream fileIn (FILE_NAME, std::ios::in | std::ios::binary);

  Node *readNode = new Node;

  while (!fileIn.eof())
  {
      fileIn.read ((char*) readNode, sizeof (Node));

      if (readNode->userId == user.userId)
      {
          user.parentUserId = readNode->parentUserId;
          user.nodeWeight = readNode->nodeWeight;
          user.edgeWeight = readNode->edgeWeight;

          delete readNode;
          fileIn.close ();
          return;
      }
  }
  delete readNode;
  fileIn.close ();
}


// propagateNodeWeight: This function updates the weight of the parent
void propagateNodeWeight (Node node1)
{
    getUser (node1);

    // Find the parent of the node
    long int parentUserId = node1.parentUserId;
    long int pos;

    if (parentUserId != HEAD_PARENT_ID)
    {
        // Find the parent and update the nodeWeight.
        std::fstream fileInOut (FILE_NAME, std::ios::in | std::ios::out | std::ios::binary);

        Node *user = new Node;

        pos = fileInOut.tellg();
        while ((fileInOut.read ((char*) user, sizeof (Node))) && parentUserId != HEAD_PARENT_ID)
        {
            if (user->userId == parentUserId)
            {
                // User found, update the edge weight.
                user->nodeWeight ++;
                fileInOut.seekp(pos);
                fileInOut.write ((char*) user, sizeof (Node));

                parentUserId = user->parentUserId;
                fileInOut.seekg(0, std::ios::beg);
            }
            pos = fileInOut.tellg();
        }
        delete user;
        fileInOut.close();
    }
}

int getDepth (Node node) {
    std::ifstream fileIn (FILE_NAME, std::ios::in | std::ios::binary);

    Node *readNode = new Node;

    int depth = 0, nodeUserId = node.userId;
    while ((fileIn.read ((char*) readNode, sizeof (Node))) && (nodeUserId != HEAD_USER_ID))
    {
        if(readNode->userId == nodeUserId)
        {
            nodeUserId = readNode->parentUserId;
            depth ++;
            fileIn.seekg (0, std::ios::beg);
        }
    }

    delete readNode;
    fileIn.close ();
    return depth;
}

void propagateEdgeWeight (Node node1, Node node2, double edgeWeight)
{
    std::fstream fileInOut (FILE_NAME, std::ios::in | std::ios::out | std::ios::binary);

    Node *readNode = new Node ();
    long int parentNode1, parentNode2, pos, node1Depth, node2Depth, minDepth;

    getUser (node1);
    getUser (node2);

    node1Depth = getDepth (node1);
    node2Depth = getDepth (node2);

    if(node1Depth == node2Depth)
      minDepth = node1Depth;
    else
      minDepth = (node1Depth < node2Depth) ? node1Depth : node2Depth;

    parentNode1 = node1.parentUserId;
    pos = fileInOut.tellg();
    while ((node1Depth != minDepth) && (fileInOut.read ((char*) readNode, sizeof (Node))))
    {
        if (readNode->userId == parentNode1)
        {
            //update node1
            readNode->edgeWeight += edgeWeight;
            fileInOut.seekp (pos);
            fileInOut.write ((char*) readNode, sizeof (Node));
            parentNode1 = readNode->parentUserId;
            node1Depth --;
            fileInOut.seekg(0, std::ios::beg);
        }
        pos = fileInOut.tellg();
    }

    fileInOut.seekg (0, std::ios::beg);
    parentNode2 = node2.parentUserId;
    pos = fileInOut.tellg();
    while ((node2Depth != minDepth) && (fileInOut.read ((char*) readNode, sizeof (Node))))
    {
        if (readNode->userId == parentNode2)
        {
            //update node2
            readNode->edgeWeight += edgeWeight;
            fileInOut.seekp (pos);
            fileInOut.write ((char*) readNode, sizeof (Node));
            parentNode2 = readNode->parentUserId;
            node2Depth --;
            fileInOut.seekg(std::ios::beg);
        }
        pos = fileInOut.tellg();
    }

    fileInOut.seekg (0, std::ios::beg);
    pos = fileInOut.tellg();
    while ((parentNode2 != parentNode1) && (fileInOut.read ((char*) readNode, sizeof (Node))))
    {
        if (readNode->userId == parentNode2)
        {
            //update node1
            readNode->edgeWeight += edgeWeight;
            fileInOut.seekp (pos);
            fileInOut.write ((char*) readNode, sizeof (Node));
            parentNode2 = readNode->parentUserId;
            fileInOut.seekg(std::ios::beg);
        }
        else if (readNode->userId == parentNode1)
        {
            //update node1
            readNode->edgeWeight += edgeWeight;
            fileInOut.seekp (pos);
            fileInOut.write ((char*) readNode, sizeof (Node));
            parentNode1 = readNode->parentUserId;
            fileInOut.seekg(std::ios::beg);
        }
        pos = fileInOut.tellg();
    }

    delete readNode;
    fileInOut.close ();
}

void displayNode (Node user, int i)
{
    std::cout << "Node " << i << std::endl;
    std::cout << "-------------------" << std::endl;
    std::cout << "UserId: " << user.userId << std::endl;
    std::cout << "ParentUserId: " << user.parentUserId << std::endl;
    std::cout << "NodeWeight: " << user.nodeWeight << std::endl;
    std::cout << "EdgeWeight: " << user.edgeWeight << std::endl;
    std::cout << std::endl;
}

void displayData ()
{
    // Open the file
    std::ifstream fileIn (FILE_NAME, std::ios::in | std::ios::binary);

    Node *user = new Node;
    int i = 1;

    while (fileIn.read ((char*) user, sizeof (Node)))
    {
        displayNode (*user, i);
        i++;
    }
    delete user;
    fileIn.close();
}
