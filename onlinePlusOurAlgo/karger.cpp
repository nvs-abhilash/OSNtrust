// Karger's algorithm to find Minimum Cut in an
// undirected, unweighted and connected graph.
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <utility>
#include <set>
#include <unordered_map>
#include <algorithm>
#include "kargers_3.h"

 
using namespace std;

// set<long> firstGraphNodeList;
// set<long> secondGraphNodeList;

// Function prototypes for union-find (These functions are defined
// after kargerMinCut() )
long find(unordered_map <long,pair<long,long> > &subset, long i);
void Union(unordered_map <long,pair<long,long> > &subset, long x, long y);
 
// A very basic implementation of Karger's randomized
// algorithm for finding the minimum cut. Please note
// that Karger's algorithm is a Monte Carlo Randomized algo
// and the cut returned by the algorithm may not be
// minimum always
vector <pair<long, long> > kargerMinCut(graph* newgraph)
{
    // cout << " In karger \n";
    // Get data of given graph

    unordered_map <long,pair<long,long> > subset;


    int V = newgraph->V, E = newgraph->E;
    cout << "V:" << V << "  E: " << E << endl;
    vector<Edge> edge(newgraph->edgeList);

    set<long> vertices_set;
    vector<Edge> :: iterator it_edgeList;
    long src_edge, dest_edge;

    // Creating the vertex set
    for(it_edgeList = edge.begin(); it_edgeList!=edge.end(); it_edgeList++)
    {
        // cout << "in for \n";
        src_edge = it_edgeList->src;
        dest_edge = it_edgeList->dest;

        if(vertices_set.find(src_edge)==vertices_set.end())
        {
            vertices_set.insert(src_edge);
        }            

        if(vertices_set.find(dest_edge)==vertices_set.end())
        {
            vertices_set.insert(dest_edge);
        }
            
    }
 
    // Create V subsets with single elements
    set<long>::iterator it_vertices_set;
    for(it_vertices_set = vertices_set.begin(); it_vertices_set!=vertices_set.end(); it_vertices_set++)
    {
        subset.insert(make_pair(*it_vertices_set,make_pair(*it_vertices_set,0)));
    }

    // Initially there are V vertices in
    // contracted graph
    int vertices = V;
 
    // Keep contracting vertices until there are
    // 2 vertices.
    vector<int> randomVec;
    for(int i=0; i < E; i++)
    {
        randomVec.push_back(i);
    }
    // random_shuffle ( randomVec.begin(), randomVec.end() );
    int kk=0;
    int cnt=1;

    while (vertices > 2)
    {
        // kk++;
        // cout <<kk;
        // cout << "in while " << kk << endl;
        // Pick a random edge
        int i = rand() % E;
        // kk = rand() % (randomVec.size());
        // int i = randomVec[kk];
        // Find vertices (or sets) of two corners
        // of current edge
        // cout << "i = " << i << "   kk: "<<kk<<endl;
        int subset1 = find(subset, edge[i].src);
        int subset2 = find(subset, edge[i].dest);

        // cout << "Subset1: " << subset1 << " \t Subset2 = " << subset2 << endl;
       // If two corners belong to same subset,
       // then no point considering this edge
    
       if (subset1 == subset2)
       {
            // cout << "--------------------------"<<cnt<<endl;
            cnt++;
            // randomVec.erase (randomVec.begin() + kk);
            continue;
       }
       // Else contract the edge (or combine the
       // corners of edge into one vertex)
       else
       {
        //   printf("Contracting edge %ld-%ld\n",
        //          edge[i].src, edge[i].dest);
          vertices--;
          cout <<"v left - " << vertices << endl;
          Union(subset, subset1, subset2);
        //   randomVec.erase (randomVec.begin() + kk);
       }
    }
 
    // Now we have two vertices (or subsets) left in
    // the contracted graph, so count the edges between
    // two components and return the count.
    int cutedges = 0;
    vector<pair<long, long> > cutEdgeVec;
    for (int i=0; i<E; i++)
    {
        int subset1 = find(subset, edge[i].src);
        int subset2 = find(subset, edge[i].dest);
        if (subset1 != subset2)
        {
            cutEdgeVec.push_back(make_pair(edge[i].src, edge[i].dest));
            cutedges++;
        }
          
    }
    cout << "rrrrrrrrrrrrr   "<<cutEdgeVec.size()<< endl;
    return cutEdgeVec;
}
 
// A utility function to find set of an element i
// (uses path compression technique)

long find(unordered_map <long,pair<long,long>> &subset, long i)
{
    // find root and make root as parent of i
    // (path compression)
    unordered_map <long,pair<long,long> >::iterator it_sub;
    it_sub = subset.find(i);
    if(it_sub->second.first != it_sub->first)
    {
        it_sub->second.first = find(subset, it_sub->second.first);
    }
 
    return it_sub->second.first;
}
 
// A function that does union of two sets of x and y
// (uses union by rank)
void Union(unordered_map <long,pair<long,long> > &subset, long x, long y)
{
    long xroot = find(subset, x);
    long yroot = find(subset, y);
 
    // Attach smaller rank tree under root of high
    // rank tree (Union by Rank)
    unordered_map <long,pair<long,long> >::iterator  it_x;
    unordered_map <long,pair<long,long> >::iterator it_y;
    it_x = subset.find(x);
    it_y = subset.find(y);
    if (it_x->second.second < it_y->second.second)
        it_x->second.first = yroot;
    else if (it_x->second.second > it_y->second.second)
        it_y->second.first = xroot;
 
    // If ranks are same, then make one as root and
    // increment its rank by one
    else
    {
        it_y->second.first = xroot;
        it_x->second.second++;
    }
}
 
// // Driver program to test above functions
// int main()
// {
//     /* Let us create following unweighted graph
//         0------1------4
//         | \    |
//         |   \  |
//         |     \|
//         2------3------5   */
//     // int V = 4;  // Number of vertices in graph
//     // int E = 5;  // Number of edges in graph
//     // struct Graph* graph = createGraph(V, E);

//     // graph *mygraph = new graph();
//     graph mygraph;
//     // add edge 0-1
//     // graph->edge[0].src = 0;
//     // graph->edge[0].dest = 1;
//     // mygraph->addEdge(0,1,0.5);
//     mygraph.addEdge(0,1,0.5);
//      mygraph.addEdge(1,4,0.5);
//       mygraph.addEdge(3,5,0.5);
 
//     // add edge 0-2
//     // graph->edge[1].src = 0;
//     // graph->edge[1].dest = 2;
//     // mygraph->addEdge(0,2,0.5);
//     mygraph.addEdge(0,2,0.5);
//     // add edge 0-3
//     // graph->edge[2].src = 0;
//     // graph->edge[2].dest = 3;
//     // mygraph->addEdge(0,3,0.5);
//     mygraph.addEdge(0,3,0.5);

//     // add edge 1-3
//     // graph->edge[3].src = 1;
//     // graph->edge[3].dest = 3;
//     // mygraph->addEdge(1,3,0.5);
//     mygraph.addEdge(1,3,0.5);
 
//     // add edge 2-3
//     // graph->edge[4].src = 2;
//     // graph->edge[4].dest = 3;
//     // mygraph->addEdge(2,3,0.5);
//     mygraph.addEdge(2,3,0.5);
//     // mygraph->V = 4;
//     // mygraph->E = 5;
    
//     mygraph.V = 6;
//     mygraph.E = 7;
 
//     // Use a different seed value for every run.
//     srand(time(NULL));
 
    
//     vector <pair<long, long> > temp = kargerMinCut(&mygraph);
//     cout << temp.size() << endl;
//     vector <pair<long, long> >::iterator it;
//     // cout << "ans is \n";
//     for(it = temp.begin(); it!=temp.end(); it++)
//         cout << it->first << "---" << it->second << "\n";
//     // printf("\nCut found by Karger's randomized algo is \n");
 
//     return 0;
// }