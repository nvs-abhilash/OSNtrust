#include <iostream>
#include <vector>
#include <utility>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <stdlib.h>

#include "kargers_3.h"

using namespace std;

int main()
{
    graph *mygraph = new graph();
    mygraph->addEdge(1, 5, 0.8);
    mygraph->addEdge(1, 2, 0.8);
    mygraph->addEdge(2, 7, 0.8);
    mygraph->addEdge(2, 3, 0.7);
    mygraph->addEdge(3, 6, 0.756);
    mygraph->addEdge(3, 4, 0.756);
    mygraph->addEdge(3, 5, 0.756);
    mygraph->V = 7;
    mygraph->E = 6;



    set< pair<pair<int, int >, int> > cutEdges;
    cutEdges.insert(make_pair(make_pair(1,2),0.75) );
    cutEdges.insert(make_pair(make_pair(3,5),0.75) );

    graph *graph_1 = new graph();
    graph *graph_2 = new graph();

    unordered_map<int, int> firstGraph_nodeHash;
    unordered_map<int, int> secondGraph_nodeHash;

    bool firstEdge = true;
    bool edgeAdded = true;
    int src, dest, weight;
    vector<Edge> :: iterator it;

    while(edgeAdded)
    {
        edgeAdded = false;
        for(it = mygraph->edgeList.begin() ; it!=mygraph->edgeList.end(); it++)
        {
            src = it->src;
            dest = it->dest;
            weight = it->weight;

            if( (cutEdges.find(make_pair(make_pair(src,dest),weight)) != cutEdges.end()) 
                || (cutEdges.find(make_pair(make_pair(dest,src),weight)) != cutEdges.end()) )
                {
                    continue;
                }
            else 
            {
                if(firstEdge)
                {
                    graph_1->addEdge(src, dest, weight);
                    firstGraph_nodeHash.insert(make_pair(src,1));
                    firstGraph_nodeHash.insert(make_pair(dest,1));
                    graph_1->V+=2;
                    graph_1->E++;
                    firstEdge = false;
                }
                else
                {
                    if( (firstGraph_nodeHash.find(src)!=firstGraph_nodeHash.end() && firstGraph_nodeHash.find(dest)==firstGraph_nodeHash.end() ) )
                    {
                        graph_1->addEdge(src, dest, weight);
                        firstGraph_nodeHash.insert(make_pair(dest,1));
                        graph_1->V++;
                        graph_1->E++;
                        edgeAdded = true;
                    }
                    else if( (firstGraph_nodeHash.find(dest)!=firstGraph_nodeHash.end() && firstGraph_nodeHash.find(src)==firstGraph_nodeHash.end() ) )
                    {
                        graph_1->addEdge(src, dest, weight);
                        firstGraph_nodeHash.insert(make_pair(src,1));
                        graph_1->V++;
                        graph_1->E++;
                        edgeAdded = true;
                    }
                    else if(firstGraph_nodeHash.find(src)!=firstGraph_nodeHash.end() && firstGraph_nodeHash.find(dest)!=firstGraph_nodeHash.end())
                    {
                        graph_1->addEdge(src, dest, weight);
                        graph_1->E++;
                        edgeAdded = true;
                    }
                    else
                    {
                        continue;
                    }
                }
            }

        }
    }

    for(it = mygraph->edgeList.begin() ; it!=mygraph->edgeList.end(); it++)
    {
        src = it->src;
        dest = it->dest;
        weight = it->weight;
        if( (cutEdges.find(make_pair(make_pair(src,dest),weight)) != cutEdges.end()) 
                || (cutEdges.find(make_pair(make_pair(dest,src),weight)) != cutEdges.end()) )
            {
                continue;
            }
                

        if(!(firstGraph_nodeHash.find(src)!=firstGraph_nodeHash.end() || firstGraph_nodeHash.find(dest)!=firstGraph_nodeHash.end()) )
        {
            
                    if( (secondGraph_nodeHash.find(src)!=secondGraph_nodeHash.end() && secondGraph_nodeHash.find(dest)==secondGraph_nodeHash.end() ) 
                        || (secondGraph_nodeHash.find(dest)!=secondGraph_nodeHash.end() && secondGraph_nodeHash.find(src)==secondGraph_nodeHash.end() ) )
                    {
                        graph_2->addEdge(src, dest, weight);
                        secondGraph_nodeHash.insert(make_pair(src,2));
                        secondGraph_nodeHash.insert(make_pair(dest,2));
                        graph_2->V++;
                        graph_2->E++;
                    }
                    else if(secondGraph_nodeHash.find(src)!=secondGraph_nodeHash.end() || secondGraph_nodeHash.find(dest)!=secondGraph_nodeHash.end())
                    {
                        graph_2->addEdge(src, dest, weight);
                        graph_2->E++;
                        edgeAdded = true;
                    }
                    else
                    {
                        graph_2->addEdge(src, dest, weight);
                        secondGraph_nodeHash.insert(make_pair(src,2));
                        secondGraph_nodeHash.insert(make_pair(dest,2));
                        graph_2->V+=2;
                        graph_2->E++;
                    }
        }
    }

    cout << graph_1->V << "- V     E --> " << graph_1->E << endl ;
    cout << graph_2->V << "- V     E --> " << graph_2->E << endl ;

}