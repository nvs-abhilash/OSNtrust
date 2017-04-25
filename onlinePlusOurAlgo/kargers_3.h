#ifndef kargers_3
#define kargers_3

#include <unordered_map>
using namespace std;


typedef pair<long, float> destWeight;
typedef set<destWeight> adjList;


struct Edge
{
    long src, dest;
    float weight;
};


class graph{
    public:
    int V, E;
    unordered_map<long, adjList > myvec;
    vector<Edge> edgeList;
    graph()
    {
        this->V =0;
        this->E =0;
    }
    void clr()
    {
        this->V =0;
        this->E =0;
        edgeList.clear();
        myvec.clear();
    }

    void addEdge( long src, long dest, float weight)
    {

        Edge newEdge ;
        newEdge.src = src;
        newEdge.dest = dest;
        newEdge.weight = weight;
        edgeList.push_back(newEdge);


        adjList tempset ;
        unordered_map<long,adjList >::iterator got = myvec.find (src);
        if ( got == myvec.end() )
        {
            tempset.insert(make_pair(dest,weight));
            // E++;
            myvec.insert(make_pair(src, tempset));
            // V++;
        }
        else
        {
            set <destWeight >::const_iterator its = got->second.find(make_pair(dest, weight));
            if(its == got->second.end())
            {
                got->second.insert(make_pair(dest,weight));
                // E++;
            }
        }

        tempset.clear();

        got = myvec.find (dest);
        if ( got == myvec.end() )
        {
            tempset.insert(make_pair(src, weight));
            //E++;
            myvec.insert(make_pair(dest, tempset));
            // V++;
        }
        else
        {
            set <destWeight >::const_iterator its = got->second.find(make_pair(src, weight));
            if(its == got->second.end())
            {
                got->second.insert(make_pair(src,weight));
                //E++;
            }
        }
    }

};

extern int find(struct subset subsets[], long i);
extern void Union(struct subset subsets[], int x, int y);
extern vector <pair<long, long> > kargerMinCut(graph* newgraph);


#endif


