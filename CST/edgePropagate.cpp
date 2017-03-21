#include <iostream>
#include <fstream>
#include "graph.h"

using namespace std;

//fstream file ("data.bin", ios::in | ios::out | ios::binary | ios::app);

void edgeWeightPropagate (Node node1, Node node2, double edgeWeight)
{
    fstream file ("data.bin", ios::in | ios::out | ios::binary | ios::app);

    Node *readNode = new Node ();
    long int parentNode1, parentNode2, pos;

    do 
    {
        pos = file.tellg();
        file.read ((char*) readNode, sizeof (Node));

        if(readNode->userId == node1.userId)
        {
            //update node1
            readNode->edgeWeight += edgeWeight;
            file.seekg (pos);
            file.write ( (char*) readNode, sizeof (Node));
            parentNode1 = readNode->parentUserId;
        }
        else if(readNode->userId == node2.userId)
        {
            //update node2
            readNode->edgeWeight += edgeWeight;
            file.seekg (pos);
            file.write ( (char*) readNode, sizeof (Node));
            parentNode2 = readNode->parentUserId;
        }
    }while ((!file.eof ()) && (parentNode1 != parentNode2));

    delete readNode;
    file.close ();
}

void displayFileContent ()
{
    fstream file ("data.bin", ios::in | ios::binary);

    Node *readNode;

    while (!file.eof ())
    {
        file.read ((char*) readNode, sizeof (Node));

        cout<<readNode->userId<<" "<<readNode->parentUserId<<" "<<readNode->nodeWeight<<" "<<readNode->edgeWeight<<endl;
        //file.seekg (sizeof (Node));
    }

    //delete readNode;
    file.close ();
}

int main ()
{    
    //edgeWeightPropagate (1, 2, 5);
    fstream file ("data.bin", ios::in | ios::out | ios::binary);
    char node1[8], node2[8], nodeWeigth[8], edgeWeigth[8], parent[8];

    Node test;
    test.userId = 1;
    test.parentUserId = -1;
    test.nodeWeight = 1;
    test.edgeWeight = 0;

    file.write( (char*)&test, sizeof(Node));

    Node test2;
    test2.userId = 2;
    test2.parentUserId = -1;
    test2.nodeWeight = 1;
    test2.edgeWeight = 0;

    file.write( (char*)&test2, sizeof(Node));
    Node *readNode;

    while (!file.eof ())
    {
        file.read ((char*) readNode, sizeof (Node));

        cout<<readNode->userId<<" "<<readNode->parentUserId<<" "<<readNode->nodeWeight<<" "<<readNode->edgeWeight<<endl;
        //file.seekg (sizeof (Node));
    }

    file.close ();
    //displayFileContent ();

    //edgeWeightPropagate (test, test2, 5);

    //displayFileContent ();

    
    return 0;
}