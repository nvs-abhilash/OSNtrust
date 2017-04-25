#ifndef GRAPH_H
#define GRAPH_H

#define HEAD_USER_ID -2
#define HEAD_PARENT_ID -1

class Edge
{
    long int u;
    long int v;
    double w;

public:
    Edge (long int u, long int v, float w)
    {
        this->u = u;
        this->v = v;
        this->w = w;
    }

    long int getU()
    {
        return u;
    }

    long int getV()
    {
        return v;
    }

    double getW()
    {
        return w;
    }
};

typedef struct Node
{
    long int userId;
    long int parentUserId;

    double nodeWeight;
    double edgeWeight;
} Node;

// typedef struct iNode
// {
//     long int userId;
//     long int parentUserId;

//     double nodeWeight;
//     double edgeWeight;
    
//     std::vector<iNode> next;
// } Node;

#endif
