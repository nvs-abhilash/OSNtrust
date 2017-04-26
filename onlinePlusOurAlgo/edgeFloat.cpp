#include <iostream>
#include <unordered_map>
#include <set>
#include <vector>
#include <utility>
#include <queue>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <fstream>
#include <string>
#include <sstream>
#include "kargers_3.h"

using namespace std;

#define cluster_thresold_size 40
unordered_map<long,long> nodeHash;
unordered_map<long,int> clusterSizeHash;

//--------------------------------------------------------------------------------------------------------------
//Karger in other file

//--------------------------------------------------------------------------------------------------------------

string itos(int i) // convert int to string
{
    stringstream s;
    s << i;
    return s.str();
}

//-------------------------------------------------------------------------------------------------------------
int main()
{
    long long src = 0, dest = 0;
    double Eweight = 0;

    int N_of_cluster = 0;
    int dest_cluster = 0, src_cluster = 0, src_cluster_size = 0, dest_cluster_size = 0;

    queue<int> emptyClusterQueue;
    graph* tempGraph = NULL;

    // graph arrOfCluster[513];
    graph* arrOfCluster = new graph[513];

    ifstream fin;
    fin.open("lac10_random_singleEdge_sg_weight.dat");

	ofstream deletedCluster;
	deletedCluster.open("deleted_cluster_num.dat");

	ofstream leftOutEdges;
	leftOutEdges.open("leftout_edges.dat");

	ofstream fout_Ecut;
	fout_Ecut.open("Ecut.dat");



    int progress = 0;
    nodeHash.insert (make_pair(0, 0));

    // While loop for each streaming edge
    while (fin >> src)
    {
        // To keep track of the number of edges encountered.
		progress++;
		cout << progress << " / 10,00,000" << endl;

		fin >> dest;
		fin >> Eweight;

        // Case 1: A and B both not present in any cluster.
        if (nodeHash.find(src) == nodeHash.end() && nodeHash.find(dest) == nodeHash.end())
        {
            // If we can create new cluster or not.
            if (N_of_cluster < 256)
            {
                N_of_cluster++;

                arrOfCluster[N_of_cluster].addEdge (src, dest, Eweight);
                arrOfCluster[N_of_cluster].V += 2;
                arrOfCluster[N_of_cluster].E++;

                nodeHash.insert (make_pair (src, N_of_cluster));
                nodeHash.insert (make_pair (dest, N_of_cluster));

                clusterSizeHash.insert (make_pair(N_of_cluster, 2)) ;
            }

            // Reusing the empty clusters.
            else if (! emptyClusterQueue.empty ())
            {
                int clusterNum = emptyClusterQueue.front ();
                emptyClusterQueue.pop ();

                arrOfCluster[clusterNum].addEdge (src, dest, Eweight);
                arrOfCluster[clusterNum].V += 2;
                arrOfCluster[clusterNum].E++;

                nodeHash.insert (make_pair (src, clusterNum));
                nodeHash.insert (make_pair (dest, clusterNum));

                clusterSizeHash.find (clusterNum)->second = 2;
            }

            else
            {
                leftOutEdges << src << " " << dest << " " << Eweight << endl;
            }
        }

        // Case 2: A and B are present and are in same cluster.
        else if ((nodeHash.find (src) != nodeHash.end())  && (nodeHash.find(dest) != nodeHash.end()) &&
                (nodeHash.find (src)->second == nodeHash.find(dest)->second))
        {
            src_cluster = nodeHash.find(src)->second;

            arrOfCluster[src_cluster].addEdge(src, dest, Eweight);
            arrOfCluster[src_cluster].E++;

        }

        // Case 3: A not present and B present in cluster.
        else if ((nodeHash.find(src) == nodeHash.end()) && (nodeHash.find (dest) != nodeHash.end()))
        {
                dest_cluster = nodeHash.find (dest)->second;

				arrOfCluster[dest_cluster].addEdge (src, dest, Eweight);
                arrOfCluster[dest_cluster].E++;
                arrOfCluster[dest_cluster].V++;
				nodeHash.insert (make_pair (src, dest_cluster));

				clusterSizeHash.find (dest_cluster)->second += 1;

                // Check if size > cluster threshold size and partion the cluster
                if (clusterSizeHash.find (dest_cluster)->second > cluster_thresold_size)
                {
                    vector <pair<long, long>> temp = kargerMinCut (&arrOfCluster[dest_cluster]);
                    cout << temp.size() <<endl;
                    vector <pair<long, long> >::iterator it_temp;
                    for (it_temp = temp.begin(); it_temp != temp.end(); it_temp++)
                        cout << it_temp->first << "====="<< it_temp->second << endl;
                }
        }

        // Case 4: A is present and B is not present.
        else if (nodeHash.find(src) != nodeHash.end() && nodeHash.find(dest) == nodeHash.end())
        {
            src_cluster = nodeHash.find(src)->second;

            arrOfCluster[src_cluster].addEdge(src, dest, Eweight);
            arrOfCluster[src_cluster].E++;
            arrOfCluster[src_cluster].V++;
            nodeHash.insert(make_pair(dest, src_cluster));

            clusterSizeHash.find (src_cluster)->second += 1;

            // Check if size > cluster threshold size and partion the cluster
            if(clusterSizeHash.find(src_cluster)->second > cluster_thresold_size)
            {
                vector <pair<long, long> > temp = kargerMinCut(&arrOfCluster[src_cluster]);
                set<long>::iterator it;
            }
        }

        // Case 5: A and B both present in different cluster.
        else
        {
            src_cluster = nodeHash.find (src)->second;
            dest_cluster = nodeHash.find (dest)->second;

            src_cluster_size = clusterSizeHash.find (src_cluster)->second;
            dest_cluster_size = clusterSizeHash.find (dest_cluster)->second;

            int EId, SrcNId, DstNId, EdgeWeight;

            // If size (1 + 2) < cluster_threshold_size then Merge them.
            if(src_cluster_size + dest_cluster_size < cluster_thresold_size)
            {
                int biggerCluster, smallerCluster;

                if(src_cluster_size >= dest_cluster_size)
                {
                    biggerCluster = src_cluster;
                    smallerCluster = dest_cluster;
                }

                else
                {
                    biggerCluster = dest_cluster;
                    smallerCluster = src_cluster;
                }

                // Make one cluster src_cluster + dest_cluster and delete the dest_cluster

                vector<Edge>::iterator EI;
                for (EI = arrOfCluster[smallerCluster].edgeList.begin(); EI != arrOfCluster[smallerCluster].edgeList.end(); EI++)
                {
                    SrcNId = EI->src;
                    DstNId = EI->dest;
                    EdgeWeight = EI->weight;

                    nodeHash.find (SrcNId)->second = biggerCluster;
                    arrOfCluster[biggerCluster].V++;


                    nodeHash.find(DstNId)->second = biggerCluster;
                    arrOfCluster[biggerCluster].V++;

                    arrOfCluster[biggerCluster].addEdge (SrcNId, DstNId, EdgeWeight);
                    arrOfCluster[biggerCluster].E++;
                }

                clusterSizeHash.find(biggerCluster)->second = clusterSizeHash.find(biggerCluster)->second + clusterSizeHash.find(smallerCluster)->second;
                clusterSizeHash.find(smallerCluster)->second = 0;
                
                arrOfCluster[smallerCluster].clr();
                emptyClusterQueue.push (smallerCluster);

                deletedCluster << smallerCluster << endl;

            }

            else
            {
                fout_Ecut << src << " " << dest << " " << Eweight <<  "    "<< src_cluster_size << "---" << dest_cluster_size<< endl;
            }
        }

    }

    ofstream fout_HMnodesC;
	fout_HMnodesC.open ("cluster_NofNodes.dat");
	int Key, Value;
	unordered_map<long, int>::iterator itt;
  	for ( itt = clusterSizeHash.begin(); itt != clusterSizeHash.end(); itt++) {
		fout_HMnodesC << itt->first << "----> "<<itt->second<<"    -    "<< arrOfCluster[itt->first].V << "---" << arrOfCluster[itt->first].E <<  endl;
  	}

    //   fout_HMnodesC << arrOfCluster[255].V << "---" << arrOfCluster[255].E << ",  "<< arrOfCluster[256].V << "---" << arrOfCluster[256].E <<endl;
	fin.close();
	deletedCluster.close();
    leftOutEdges.close();
	fout_HMnodesC.close();
	fout_Ecut.close();
}