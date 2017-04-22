#include <fstream>
#include <string>
#include <iostream>
#include "graph.h"

const char FILE_NAME[] = "CST_out.bin";

// Initialize Tree: add the default head node
void initializeTree ()
{
    std::ofstream fileIn (FILE_NAME, std::ios::out | std::ios::binary);

    Node *head = new Node;

    head -> userId = -2;
    head -> parentUserId = -1;

    head -> nodeWeight = 0;
    head -> edgeWeight = 0;

    fileIn.write ((char*)head, sizeof(Node));

    fileIn.close();
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
            return pos;
        }
    }

    fileIn.close();
    return -1;
}

// Connect: A simple write operation in the file.
void connect (Node &node1, Node &node2, double edgeWeight)
{
    // Find Node2
    // Open the file
    std::fstream fileIn (FILE_NAME, std::ios::in | std::ios::out | std::ios::binary);

    // Write the new node
    fileIn.seekg (0, std::ios::end);
    node1.parentUserId = node2.userId;
    node1.edgeWeight = edgeWeight;

    fileIn.write ((char *)&node1, sizeof(Node));

    fileIn.close();
}

void getUser (Node &user)
{
  std::fstream file (FILE_NAME, std::ios::in | std::ios::out | std::ios::binary);

  Node *readNode = new Node;

  while (!file.eof())
  {
      file.read ((char*) readNode, sizeof (Node));

      if (readNode->userId == user.userId)
      {
          file.close ();
          user.parentUserId = readNode->parentUserId;
          user.nodeWeight = readNode->nodeWeight;
          user.edgeWeight = readNode->edgeWeight;

          return;
      }
  }

  file.close ();
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
        std::fstream fileIn (FILE_NAME, std::ios::in | std::ios::out | std::ios::binary);

        Node *user = new Node;

        while (! fileIn.eof() && parentUserId != HEAD_PARENT_ID)
        {
            pos = fileIn.tellg();
            fileIn.read ((char*) user, sizeof (Node));

            if (user->userId == parentUserId)
            {
                // User found, update the edge weight.
                user->nodeWeight ++;
                fileIn.seekg(pos);
                fileIn.write ((char*) user, sizeof (Node));

                parentUserId = user->parentUserId;
                fileIn.seekg(0, std::ios::beg);
            }
        }

        fileIn.close();
        // propagateNodeWeight (*user);
    }
}

int getDepth (Node node) {
    std::fstream file (FILE_NAME, std::ios::in | std::ios::out | std::ios::binary);

    Node *readNode = new Node;

    int depth = 0, nodeUserId = node.userId;
    do
    {
        file.read ((char*) readNode, sizeof (Node));

        if(readNode->userId == nodeUserId)
        {
            nodeUserId = readNode->parentUserId;
            depth ++;
            file.seekg (0, std::ios::beg);
        }
    }while (!file.eof () && (nodeUserId != HEAD_USER_ID));

    delete readNode;
    file.close ();
    return depth;
}

void propagateEdgeWeight (Node node1, Node node2, double edgeWeight)
{
    std::fstream file (FILE_NAME, std::ios::in | std::ios::out | std::ios::binary);

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

    parentNode1 = node1.userId;
    while ((node1Depth != minDepth) && (!file.eof ()))
    {
        pos = file.tellg();
        file.read ((char*) readNode, sizeof (Node));

        if (readNode->userId == parentNode1)
        {
            //update node1
            readNode->edgeWeight += edgeWeight;
            file.seekg (pos);
            file.write ( (char*) readNode, sizeof (Node));
            parentNode1 = readNode->parentUserId;
            node1Depth --;
        }
    }

    file.seekg (std::ios::beg);
    parentNode2 = node2.userId;
    while ((node2Depth != minDepth) && (!file.eof ()))
    {
        pos = file.tellg();
        file.read ((char*) readNode, sizeof (Node));

        if (readNode->userId == parentNode2)
        {
            //update node2
            readNode->edgeWeight += edgeWeight;
            file.seekg (pos);
            file.write ( (char*) readNode, sizeof (Node));
            parentNode2 = readNode->parentUserId;
            node2Depth --;
        }
    }

    file.seekg (std::ios::beg);
    while ((parentNode2 != parentNode1) && (!file.eof ()))
    {
        pos = file.tellg();
        file.read ((char*) readNode, sizeof (Node));

        if (readNode->userId == parentNode2)
        {
            //update node1
            readNode->edgeWeight += edgeWeight;
            file.seekg (pos);
            file.write ( (char*) readNode, sizeof (Node));
            parentNode2 = readNode->parentUserId;
        }
        else if (readNode->userId == parentNode1)
        {
            //update node1
            readNode->edgeWeight += edgeWeight;
            file.seekg (pos);
            file.write ( (char*) readNode, sizeof (Node));
            parentNode1 = readNode->parentUserId;
        }
    }

    delete readNode;
    file.close ();
}

void displayData ()
{
    // Open the file
    std::ifstream fileIn (FILE_NAME, std::ios::in | std::ios::binary);

    Node *user = new Node;
    int i = 1;

    while (fileIn.read ((char*) user, sizeof (Node)))
    {
        std::cout << "Node " << i << std::endl;
        std::cout << "-------------------" << std::endl;
        std::cout << "UserId: " << user->userId << std::endl;
        std::cout << "ParentUserId: " << user->parentUserId << std::endl;
        std::cout << "NodeWeight: " << user->nodeWeight << std::endl;
        std::cout << "EdgeWeight: " << user->edgeWeight << std::endl;
        std::cout << std::endl;
        i++;
    }

    fileIn.close();
}
