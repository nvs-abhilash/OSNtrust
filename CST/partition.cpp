#include <iostream>
#include <fstream>
#include <string.h>
#include "graph.h"
#include "tree.h"

#include <bits/stdc++.h>
#include <vector>

const char FILE_NAME[] = "CST_out.bin";

// Compares two intervals according to staring times.
bool compare(Node i1, Node i2)
{
    return ((i1.edgeWeight)/(i1.nodeWeight) < (i2.edgeWeight) / (i2.nodeWeight));
}

std::vector <Node> sortAmortized ()
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

    return allUsers;
}
