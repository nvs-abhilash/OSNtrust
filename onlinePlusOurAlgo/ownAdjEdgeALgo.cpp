#include <iostream>
#include <unordered_map>
#include <set>
#include <vector>
#include <utility>
// #include <queue>

// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>

// #include <fstream>
// #include <string>
// #include <sstream>
#include "kargers_3.h"

using namespace std;

graph* ownAlgorithm(graph *newgraph)
{
    long src, dest;
    float weight;

    unordered_map<long, adjList>::iterator it;
    for (it = newgraph->myvec.begin(); it != newgraph->myvec.end(); it++)
    {
        src = it->first;
        set<pair<long, float> >::iterator it_list_slow_mover;
        set<pair<long, float> >::iterator it_list_fast_mover;
        
        
        for(it_list_slow_mover = it->second.begin(); it_list_slow_mover!=it->second.end(); it_list_slow_mover++)
        {
            
            for(it_list_fast_mover = it_list_slow_mover; it_list_fast_mover!=it->second.end(); )
            {
                it_list_fast_mover++;
                if(it_list_fast_mover==it->second.end())
                    break;
                cout << it->first << " : " ;
                cout<< it_list_slow_mover->first << "  " << it_list_fast_mover->first << endl;

                if(it_list_slow_mover->second > 0.75 && it_list_fast_mover->second > 0.75)
                {
                    cout << " addedge \n";
                    newgraph->addEdge(it_list_slow_mover->first, it_list_fast_mover->first
                                    ,(it_list_slow_mover->second + it_list_fast_mover->second )/2.0);
                    newgraph->E++;
                    cout << newgraph->E <<endl;
                }

            }
        }
    }
    return newgraph;
}

int main()
{
    graph *mygraph = new graph();
    mygraph->addEdge(1, 5, 0.8);
    mygraph->addEdge(1, 2, 0.8);
    mygraph->addEdge(2, 7, 0.8);
    mygraph->addEdge(2, 3, 0.7);
    mygraph->addEdge(3, 6, 0.756);
    mygraph->addEdge(3, 4, 0.756);
    mygraph->V = 7;
    mygraph->E = 6;

    graph *modifiedGraph = ownAlgorithm(mygraph);
    cout << " NoOfEdges----->" << modifiedGraph->E << endl;
}
