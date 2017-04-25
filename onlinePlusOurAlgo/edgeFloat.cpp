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


// set<long> firstGraphNodeList;
// set<long> secondGraphNodeList;

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
    int dest_cluster=0, src_cluster=0, src_cluster_size=0, dest_cluster_size=0;
    string folder = "cluster/";
    string s_fileaddress = "";
    const char *address = NULL;
    queue<int> emptyClusterQueue;
    graph* tempGraph = NULL;

    // graph arrOfCluster[513]; 
    graph* arrOfCluster = new graph[513];

    ifstream fin;
    fin.open("lac10_random_singleEdge_sg_weight.dat");

	ofstream fout;
	fout.open("deleted_cluster_num.dat");

	ofstream fout_edges;
	fout_edges.open("leftout_edges.dat");

	ofstream fout_Ecut;
	fout_Ecut.open("Ecut.dat");

    

    int progress = 0;
    nodeHash.insert(make_pair(0,0));

    while (fin >> src)
    {
		progress++;
		cout << progress << " / 10,00,000" << endl;

		fin >> dest;
		fin >> Eweight;

        if(nodeHash.find(src)==nodeHash.end() && nodeHash.find(dest)==nodeHash.end())
        {
            if(N_of_cluster < 256)
            {
                // cout << " -------------------------------1.1 " << endl;
                N_of_cluster++;
                arrOfCluster[N_of_cluster].addEdge(src, dest, Eweight);
                arrOfCluster[N_of_cluster].V +=2;
                arrOfCluster[N_of_cluster].E++;

                s_fileaddress = folder + itos(N_of_cluster) + ".bin";
				address = s_fileaddress.c_str();
                 
                //save out cluster
                ofstream fileOut;
                fileOut.open(address, ios::out | ios::binary);
                tempGraph = &arrOfCluster[N_of_cluster];
                fileOut.write ((char*)tempGraph, sizeof(graph));
                fileOut.close();

                nodeHash.insert(make_pair(src, N_of_cluster));
                nodeHash.insert(make_pair(dest, N_of_cluster));

                clusterSizeHash.insert(make_pair(N_of_cluster, 2)) ;
            }
            else if(!emptyClusterQueue.empty())
            {
                // cout << " -------------------------------1.2 " << endl;
                int clusterNum = emptyClusterQueue.front();
                emptyClusterQueue.pop();

                arrOfCluster[clusterNum].addEdge(src, dest, Eweight);
                arrOfCluster[clusterNum].V +=2;
                arrOfCluster[clusterNum].E++;

                s_fileaddress = folder + itos(clusterNum) + ".bin";
				address = s_fileaddress.c_str();
                 
                //save out cluster
                ofstream fileOut;
                fileOut.open(address, ios::out | ios::binary);
                tempGraph = &arrOfCluster[clusterNum];
                fileOut.write ((char*)tempGraph, sizeof(graph));
                fileOut.close();

                nodeHash.insert(make_pair(src, clusterNum));
                nodeHash.insert(make_pair(dest, clusterNum));

                // clusterSizeHash.insert(make_pair(clusterNum, 2)) ;
                clusterSizeHash.find(clusterNum)->second = 2;
            }
            else
            {
                fout_edges << src << " " << dest << " " << Eweight << endl;
            }
        }
        else if( (nodeHash.find(src)!=nodeHash.end())  &&(nodeHash.find(dest)!=nodeHash.end())  && (nodeHash.find(src)->second == nodeHash.find(dest)->second) )
        {
            // cout << " -------------------------------2 " << endl;
            src_cluster = nodeHash.find(src)->second;
            s_fileaddress = folder + itos(src_cluster) + ".bin";
			address = s_fileaddress.c_str();
            
            // read in cluster
            ifstream fileIn ;
            fileIn.open(address, ios::in | ios::binary);
            tempGraph = &arrOfCluster[src_cluster];
            fileIn.read ((char*)tempGraph, sizeof(graph));
            fileIn.close();

            arrOfCluster[src_cluster].addEdge(src, dest, Eweight);
            arrOfCluster[src_cluster].E++;

            // write out cluster
            ofstream fileOut;
            fileOut.open(address, ios::out | ios::binary);
            tempGraph = &arrOfCluster[src_cluster];
            fileOut.write ((char*)tempGraph, sizeof(graph));
            fileOut.close();

        }
        else if( (nodeHash.find(src)==nodeHash.end())  &&   (nodeHash.find(dest)!=nodeHash.end()) )
        {
                // cout << " -------------------------------3 " << endl;
				dest_cluster = nodeHash.find(dest)->second;
				//cout << dest_cluster << "----" << dest_cluster << endl;
				s_fileaddress = folder+itos(dest_cluster)+".bin";
				address = s_fileaddress.c_str();

                //  read in cluster
                ifstream fileIn ;
                fileIn.open(address, ios::in | ios::binary);
                tempGraph = &arrOfCluster[dest_cluster];
                fileIn.read ((char*)tempGraph, sizeof(graph));
                fileIn.close();

				arrOfCluster[dest_cluster].addEdge(src, dest, Eweight);
                arrOfCluster[dest_cluster].E++;
                arrOfCluster[dest_cluster].V++; 
				nodeHash.insert(make_pair(src, dest_cluster));

                // write out cluster
                ofstream fileOut;
                fileOut.open(address, ios::out | ios::binary);
                tempGraph = &arrOfCluster[dest_cluster];
                fileOut.write ((char*)tempGraph, sizeof(graph));
                fileOut.close();


				clusterSizeHash.find(dest_cluster)->second += 1;

                //check size ..........than ...apply kernigham lin min cut ....
                if(clusterSizeHash.find(dest_cluster)->second > cluster_thresold_size)
                {
                    cout << "in here 1-------------------------------------------------------------";
                    vector <pair<long, long> > temp = kargerMinCut(&arrOfCluster[dest_cluster]);
                    cout << temp.size()<<endl;
                    vector <pair<long, long> >:: iterator it_temp;
                    for(it_temp = temp.begin(); it_temp !=temp.end(); it_temp++)
                        cout << it_temp->first << "====="<< it_temp->second << endl;
                }
        }
        else if( nodeHash.find(src)!=nodeHash.end() && nodeHash.find(dest)==nodeHash.end() )
        {
                // cout << " -------------------------------4 " << endl;
				src_cluster = nodeHash.find(src)->second;
				//cout << src_cluster << "----" << src_cluster << endl;
				s_fileaddress = folder+itos(src_cluster)+".bin";
				address = s_fileaddress.c_str();

                // read in cluster
                ifstream fileIn ;
                fileIn.open(address, ios::in | ios::binary);
                tempGraph = &arrOfCluster[src_cluster];
                fileIn.read ((char*)tempGraph, sizeof(graph));
                fileIn.close();

				arrOfCluster[src_cluster].addEdge(src, dest, Eweight);
                arrOfCluster[src_cluster].E++;
                arrOfCluster[src_cluster].V++; 
				nodeHash.insert(make_pair(dest, src_cluster));

                //write out cluster
                ofstream fileOut;
                fileOut.open(address, ios::out | ios::binary);
                tempGraph = &arrOfCluster[src_cluster];
                fileOut.write ((char*)tempGraph, sizeof(graph));
                fileOut.close();

				clusterSizeHash.find(src_cluster)->second += 1;

				//check size ..........than ...apply kernigham lin min cut ....
                if(clusterSizeHash.find(src_cluster)->second > cluster_thresold_size)
                {
                    cout << "in here -------------------------------------------------------------";
                    vector <pair<long, long> > temp = kargerMinCut(&arrOfCluster[src_cluster]);
                    // set<long>::iterator it;
                    // for(it = firstGraphNodeList.begin(); it!=firstGraphNodeList.end(); it++)
                    //     cout << *it <<"-";
                    // cout <<"mmmm  " << firstGraphNodeList.size()<< endl;
                
                    // for(it = secondGraphNodeList.begin(); it!=secondGraphNodeList.end(); it++)
                    //     cout << *it <<"-";
                    // cout << endl;
                }
        }
        else
        {
                // cout << " -------------------------------5 " << endl;
				src_cluster = nodeHash.find(src)->second;
				dest_cluster = nodeHash.find(dest)->second;

				src_cluster_size = clusterSizeHash.find(src_cluster)->second;
				dest_cluster_size = clusterSizeHash.find(dest_cluster)->second;
				//cout << endl << src_cluster_size <<"-------------------------------"<<dest_cluster_size << endl;

				int EId, SrcNId, DstNId, EdgeWeight;

				if(src_cluster_size + dest_cluster_size < cluster_thresold_size)
				{
                    // cout << " -------------------------------if " << endl;
                    // cout << " ------------------------------- " << src_cluster << "--" << dest_cluster << endl;
                    // cout << " ------------------------------- " << src_cluster_size << "--" << dest_cluster_size << endl;
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
						//cout << "dest less than source";
						//make one cluster src_cluster+dest_cluster and delete the dest_cluster
						s_fileaddress = folder+itos(src_cluster)+".bin";
						address = s_fileaddress.c_str();

                        ifstream fileIn ;
                        fileIn.open(address, ios::in | ios::binary);
                        tempGraph = &arrOfCluster[src_cluster];
                        fileIn.read ((char*)tempGraph, sizeof(graph));
                        fileIn.close();

						s_fileaddress = folder+itos(dest_cluster)+".bin";
						address = s_fileaddress.c_str();

                        ifstream fileIn2 ;
                        fileIn2.open(address, ios::in | ios::binary);
                        tempGraph = &arrOfCluster[dest_cluster];
                        fileIn2.read ((char*)tempGraph, sizeof(graph));
                        fileIn2.close();

                        vector<Edge> ::iterator EI ;
  						for ( EI = arrOfCluster[smallerCluster].edgeList.begin(); EI != arrOfCluster[smallerCluster].edgeList.end(); EI++) 
						{
    						SrcNId = EI->src;
    						DstNId = EI->dest;
    						EdgeWeight = EI->weight;

							if(nodeHash.find(SrcNId)->second != biggerCluster)
							{
								nodeHash.find(SrcNId)->second = biggerCluster;
                                arrOfCluster[biggerCluster].V++;
							}

							if(nodeHash.find(DstNId)->second != biggerCluster)
							{
								nodeHash.find(DstNId)->second = biggerCluster;
                                arrOfCluster[biggerCluster].V++;
							}

							arrOfCluster[biggerCluster].addEdge(SrcNId, DstNId, EdgeWeight);
                            arrOfCluster[biggerCluster].E++;


  						}
						clusterSizeHash.find(biggerCluster)->second = clusterSizeHash.find(biggerCluster)->second + clusterSizeHash.find(smallerCluster)->second;
						clusterSizeHash.find(smallerCluster)->second = 0;
						//cout << endl << clusterSizeHash.GetDat(src_cluster)  <<"-----------"<<clusterSizeHash.GetDat(dest_cluster) << endl;

                        arrOfCluster[smallerCluster].clr();
						emptyClusterQueue.push(smallerCluster);
						fout << smallerCluster <<endl;

						s_fileaddress = folder+itos(src_cluster)+".bin";
						address = s_fileaddress.c_str();

                        ofstream fileOut;
                        fileOut.open(address, ios::out | ios::binary);
                        tempGraph = &arrOfCluster[src_cluster];
                        fileOut.write ((char*)tempGraph, sizeof(graph));
                        fileOut.close();

						s_fileaddress = folder+itos(dest_cluster)+".bin";
						address = s_fileaddress.c_str();

                        ofstream fileOut2;
                        fileOut2.open(address, ios::out | ios::binary);
                        tempGraph = &arrOfCluster[dest_cluster];
                        fileOut2.write ((char*)tempGraph, sizeof(graph));
                        fileOut2.close();
					
				}
				else
				{
					fout_Ecut << src << " " << dest << " " << Eweight <<  "    "<<src_cluster_size << "---"<< dest_cluster_size<< endl;
				}
        }

    }

    ofstream fout_HMnodesC;
	fout_HMnodesC.open("cluster_NofNodes.dat");
	int Key, Value;
	unordered_map<long, int>::iterator itt;
  	for ( itt = clusterSizeHash.begin(); itt != clusterSizeHash.end(); itt++) {
		fout_HMnodesC << itt->first << "----> "<<itt->second<<"    -    "<< arrOfCluster[itt->first].V << "---" << arrOfCluster[itt->first].E <<  endl;
  	}
    //   fout_HMnodesC << arrOfCluster[255].V << "---" << arrOfCluster[255].E << ",  "<< arrOfCluster[256].V << "---" << arrOfCluster[256].E <<endl;
	fin.close();
	fout.close();
    fout_edges.close();
	fout_HMnodesC.close();
	fout_Ecut.close();
    // graph* graph1 = createGraph();
    // addEdge(graph1, 2, 1, 0.5);
    // addEdge(graph1, 2, 3, 0.5);
    // // addEdge(graph1, 1, 2);
    // addEdge(graph1, 2, 4, 0.5);
    // addEdge(graph1, 3, 0, 0.5);
    // addEdge(graph1, 4, 1, 0.5);
    // printGraph(graph1);

    // cout << endl<<endl;

    // srand(time(NULL));
 
    // printf("\nCut found by Karger's randomized algo is %d\n",
    //        kargerMinCut(graph1));

}