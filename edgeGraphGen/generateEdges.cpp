#include <iostream>
#include "graph.h"
#include <unordered_map>
#include <fstream>
#include <utility>

using namespace std;

const string NODE_HASH_FILE = "nodeHash.txt";
const string INPUT_EDGES_FILE = "test.txt";

// Program to generate the required edges in the partition files, and calculate the 
// edge cut

unordered_map<long long int, long long int> genNodeHash()
{
    unordered_map<long long int, long long int> temp;

    ifstream fin(NODE_HASH_FILE);
    long long int nodeNumber, partNumber;

    while(fin >> nodeNumber >> partNumber)
        temp.insert(make_pair(nodeNumber, partNumber));
    
    fin.close();
    return temp;
}

void writeEdge(Edge *e, long long int partNumber)
{
    char fName[30];
    sprintf (fName, "edgePart_%lld.txt", partNumber);

    ofstream fout(fName, ios::app);

    fout << e->getU() << " " << e->getV() << " " << e->getW() << endl;

    fout.close();
}

double generateEdges()
{
    double edgeCut = 0;
    Edge *e;
    long long int part1, part2;
    long long int u, v;
    double w;

    // Load the nodeHash
    unordered_map<long long int, long long int> nodeHash = genNodeHash();
    unordered_map<long long int, long long int>::iterator got;

    ifstream fin(INPUT_EDGES_FILE);
    while(fin >> u >> v >> w)
    {
        e = new Edge(u, v, w);
        part1 = -1;
        part2 = -1;

        // Find part1
        got = nodeHash.find(e->getU());
        if (got != nodeHash.end())
            part1 = got->second;
        
        got = nodeHash.find(e->getV());
        if (got != nodeHash.end())
            part2 = got->second;
        
        // Write the edge in that partition file
        if (part1 == part2 && part1 != -1)
            writeEdge(e, part1);
        else
            edgeCut += e->getW();
        delete e;
    }

    fin.close();
    return edgeCut;
}

int main ()
{
    double edgeCut = generateEdges();

    cout << "Edge cut: " << edgeCut << endl;

    return 0;
}