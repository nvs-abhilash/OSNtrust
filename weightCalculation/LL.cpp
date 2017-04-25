//g++ -std=c++11 LL.cpp
//./a.out
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

#define FRIEND_BIAS 0.5

using namespace std;

float findSimilarity(int user_1, int user_2, unordered_map<int, vector<pair<int, int> > > &dataStruct )
{
        auto p = dataStruct.find(user_1);
        auto q = dataStruct.find(user_2);
        
        if(p!=dataStruct.end() && q!=dataStruct.end())
        {
                float union_loc = p->second.size() + q->second.size();
                float intersect =0;
                int simi=0, non_simi=0;
                float CLR =0, CoR=0, weight=0.0;
                for(int i=0,j=0; i < p->second.size(), j < q->second.size(); )
                {
                 
                    if(q->second[j].first == p->second[i].first )
                    {
                        intersect++;
                        union_loc--;
                        if(q->second[j].second >= 4 && p->second[i].second >= 4)
                            simi++;
                        else if(q->second[j].second < 4 && p->second[i].second < 4)
                            simi++;
                        else
                            non_simi++;
                        i++;
                        j++;
                    }
                    else if(q->second[j].first > p->second[i].first)
                        i++;
                    else
                        j++;
                }
                CLR = intersect/union_loc;
                if(CLR!=0)
                    CoR = (simi - non_simi)/intersect;
                weight = (CLR*CoR + 1.0)/4.0 + FRIEND_BIAS;

				if(weight > 1.0)
					weight = 1.0;
                return weight;
            
        }
        else if(p!=dataStruct.end())
        {
            // cout << "user 2 not found";
            return 0.75;
        }
        else{
            // cout << "user 1 not found";
            return 0.75;
        }
}

int main()
{
    ifstream fin;
    int user = 0, locID = 0, rating = 0;
    unordered_map<int, vector<pair<int, int> > > dataStruct;
    fin.open("ratings.dat");
    
    while (fin >> user)
    {
        fin >> locID;
        fin >> rating;
       
        auto p = dataStruct.find(user);
        if(p!=dataStruct.end())
        {
            // cout << "found";
            p->second.push_back(make_pair(locID,rating));
        }
        else
        {
            // cout << "not";
            vector<pair<int, int> > tempVec;
            tempVec.push_back(make_pair(locID, rating));
            dataStruct.insert({user, tempVec});

        }
            
    }
 

    ifstream fi;
    fi.open("socialgraph.dat");

    ofstream fout;
    fout.open("socialgraph_edgeWeight.dat");

    int user_1=0, user_2=0;
    float weight =0;
	int progress=0;
    while(fi >> user_1 )
    {
		progress++;
		cout << progress <<" / 135,49,236" << endl;
        fi >> user_2;
        weight = findSimilarity(user_1, user_2, dataStruct);
        fout << user_1<<" " << user_2 << " "<<  weight << endl;
    }
    
    fin.close();
    fi.close();
    fout.close();
}
