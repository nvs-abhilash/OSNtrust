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
        }
    }

    if (! userFound)
    {
        std::cerr << "Error: Cannot connect to unknown node in the CST \n";
    }

    // Write the new node
    fileIn.seekg (0, ios::end);
    fileIn.write ((char *)user, sizeof(Node));

    fileIn.close();
}
