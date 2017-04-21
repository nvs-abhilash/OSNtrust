#include <fstream>
#include <string>
#include <iostream>
#include "graph.h"

const char FILE_NAME[] = "CST_out.bin";

#define HEAD_USER_ID -2

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
bool presentInTree (Node node)
{
    // Open the file
    std::ifstream fileIn (FILE_NAME, std::ios::in | std::ios::binary);

    Node *user = new Node;

    while (fileIn.read ((char*) user, sizeof (Node)))
    {
        // Extract in the nodeUserId
        if (user->userId == node.userId)
        {
            fileIn.close();
            return true;
        }
    }

    fileIn.close();
    return false;
}

// Connect: A simple write operation in the file.
void connect (Node node1, Node node2, double edgeWeight)
{
    // Find Node2
    // Open the file
    std::fstream fileIn (FILE_NAME, std::ios::in | std::ios::out | std::ios::binary);
    long int pos;

    Node *user = new Node;
    bool userFound = false;

    while (! fileIn.eof())
    {
        pos = fileIn.tellg();
        fileIn.read ((char*) user, sizeof (Node));

        if (user->userId == node2.userId)
        {
            // User found, update the edge weight.
            user->edgeWeight += edgeWeight;
            fileIn.seekg(pos);
            fileIn.write ((char*) user, sizeof (Node));

            userFound = true;
            break;
        }
    }

    if (! userFound)
    {
        std::cerr << "Error: Cannot connect to unknown node in the CST \n";
    }

    // Write the new node
    fileIn.seekg (0, std::ios::end);
    node1.parentUserId = node2.userId;
    fileIn.write ((char *)&node1, sizeof(Node));

    fileIn.close();
}

// propagateNodeWeight: This function updates the weight of the parent
void propagateNodeWeight (Node node1)
{
    // Find the parent of the node
    long int parentUserId = node1.parentUserId;
    long int pos;

    if (parentUserId != -1)
    {
        // Find the parent and update the nodeWeight.
        std::fstream fileIn (FILE_NAME, std::ios::in | std::ios::out | std::ios::binary);

        Node *user = new Node;

        while (! fileIn.eof())
        {
            pos = fileIn.tellg();
            fileIn.read ((char*) user, sizeof (Node));

            if (user->userId == parentUserId)
            {
                // User found, update the edge weight.
                user->nodeWeight += node1.nodeWeight;
                fileIn.seekg(pos);
                fileIn.write ((char*) user, sizeof (Node));

                break;
            }
        }

        fileIn.close();
        propagateNodeWeight (*user);
    }
}

int getDepth (Node node) {
    std::fstream file ("data.bin", std::ios::in | std::ios::binary);

    int depth = 0, nodeUserId = node.userId;
    do
    {
        file.read ((char*) readNode, sizeof (Node));

        if(readNode->userId == nodeUserId)-
        {
            nodeUserId = readNode->parentUserId;
            depth ++;
        }
    }while (!file.eof () && (nodeUserId != HEAD_USER_ID));

    delete readNode;
    file.close ();
    return depth;
}

void edgeWeightPropagate (Node node1, Node node2, double edgeWeight)
{
    std::fstream file ("data.bin", std::ios::in | std::ios::out | std::ios::binary);

    Node *readNode = new Node ();
    long int parentNode1, parentNode2, pos, node1Depth, node2Depth, minDepth;

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

    file.tellg (std::ios::beg);
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

    file.tellg (std::ios::beg);
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
