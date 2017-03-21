#ifndef GRAPH_H
#define GRAPH_H

class Edge
{
    long int u;
    long int v;
    float w;

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

    long int getW()
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

#endif
