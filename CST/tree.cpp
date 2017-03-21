#include <fstream>
#include <string>
#include "graph.h"

// Normal search function to search for the userId
bool presentInTree (Node node)
{
    // Open the file
    std::ifstream fileIn = open (FILE_NAME, ios::in | ios::binary);

    Node *user = new Node;

    while (fileIn.read ((char*) user, sizeof (Node))
    {
        // Extract in the nodeUserId
        if (user.userId == node.userId)
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
    std::fstream fileIn = open (FILE_NAME, ios::in | ios::out | ios::binary);
    long int pos;

    Node *user = new Node;
    bool userFound = false;

    while (! fileIn.eof())
    {
        pos = fileIn.tellg();
        fileIn.read ((char*) user, sizeof (Node));

        if (user.userId == node.userId)
        {
            // User found, update the edge weight.
            user.edgeWeight += edgeWeight;
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
    fileIn.seekg (0, ios::end);
    node1.parentUserId = node2.userId;
    fileIn.write ((char *)node1, sizeof(Node));

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
        std::fstream fileIn = open (FILE_NAME, ios::in | ios::out | ios::binary);

        Node *user = new Node;

        while (! fileIn.eof())
        {
            pos = fileIn.tellg();
            fileIn.read ((char*) user, sizeof (Node));

            if (user.userId == parentUserId)
            {
                // User found, update the edge weight.
                user.nodeWeight += node1.nodeWeight;
                fileIn.seekg(pos);
                fileIn.write ((char*) user, sizeof (Node));

                break;
            }
        }

        fileIn.close();
        propagateNodeWeight (user);
    }
}
