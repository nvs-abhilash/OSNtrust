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
            return true;
    }

    return false;
}
