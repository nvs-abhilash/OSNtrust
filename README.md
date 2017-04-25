# R&D Project Report

Algorithms for Graph Based Trust Models in 
Online Social Networks
(OSNs)



Table of Contents :
Summary,
Problem Statement,
Methodology,
Experimental Results,
Interpretation of Results,
Challenges Faced.




1)	Summary :
Online Social Networks (OSNs) are becoming a popular method of meeting people and keeping in touch with friends. OSNs resort to trust evaluation models and algorithms to improve service quality and enhance user experiences. Much research has been done to evaluate trust and predict the trustworthiness of a target, usually from the view of a source. Graph-based approaches make up a major portion of the existing works, in which the trust value is calculated through a trusted graph (or trusted network, web of trust, or multiple trust chains).The goal of this project is to design and implement algorithms for such trust evaluation.


2)	Problem Statement :
All around the world people are using Social Media to create a big online social network and the numbers just keep increasing. It is estimated that by the year 2018, 2.44 billion people will be using social networks. The growing desire and demand of social networks is not just removing our communication barrier, it is also changing the way we think and do things.
The people we “trust” or “are close to” in our social network continuously shape our views about  different things. Like for example, if there two mobile phones of different brands having same features are available in the market. And one of the phone is recommended to you by your friend. So, which one would you prefer to buy ? Obviously the one recommended to you by your friend. This is because you would trust your friend more than anyone else.  This trust based relationship is the one that majority of the online websites use to recommend items and is also referred to as online recommender system. Recommender Systems produce a list of recommendations for a user using two techniques- Collaborative filtering and Content based filtering
In our research, we will be focusing on Collaborative filtering. 
But there are too many website users hence a lot of data for the recommender systems to process which makes the system less accurate. Thus the objective here is to solve the scalability issue in Collaborative Filtering by making coherent partitions.

3)	Methodology :
These days social graphs are changing every seconds, which should be updated. Graphs are so large that we can not load the entire graph in the main memory to process. So we need to devise a Heuristic to smartly partition graph with least loss of information. Recommendation algorithm will be applied on each partition.
Complexity:   O(n2/k2), and for k partition. Overall time complexity becomes: O(n2/k).
 

Without partition Computation complexity
	  NC2	     =   O( N*N )
Let say we divide Graph in ‘K’ parts, Graph contains total ‘N’ nodes(USERS), so each partition contains around  ‘N/K’ nodes.
With partition Computation complexity:
        	   N/KC2       = O( N*N/K*K ) for each partition   
So for ‘K’ partitions complexity becomes: O( N*N/K )
The traditional partitioning algorithm needs to load the entire graph in memory in order to make partitions. So to resolve this problem we would be using Online Algorithms.

To solve the scalability issue, we are using the Foursquare dataset to get best results for recommendation. There are three main areas in the process of solving it.
1)	Internal Processing of Data set
2)	Edge weight calculation between users
3)	Floating of edges and clustering (Online Algorithms)

Internal Processing of Data set :-
	FourSquare is basically an application which recommends hotels to their customers based on their friends and location. Foursquare data set that we will be using to prove our algorithm contains the following information : It has five files called Users, Social graph, Venue, Checkins and Ratings. Thus foursquare dataset contains the information of its users, their location, friends as well as their ratings for various hotels. It also contains the location of hotels. Users.dat file has UserId, Longitude and Latitude which consists of the location of all the users registered in foursquare. Socialgraph.dat contains firstUserId and secondUserID which determines the friendship of the users using the application. Venue.dat has Id, Longtitude and Latitude describing the location of all the hotels. Checkins.dat has Id, UserId, VenueId, Longitude, Latitude, CreatedAt which gives the information as to which user visited which venue(hotel in this case). Ratings contains UserId, VenueId, Ratings which are basically the ratings given by users to the various hotels. Below is the tabular presentation of the above data.

Users :-
UserId	Longitude	Latitude

Social Graph :-
FirstUserId	SecondUserId

Venue :-
Id	Longitude	Latitude

Checkins :-
Id	UserId	VenueId	Longitude	Latitude	CreatedAt

Ratings :-
UserId	VenueId	Ratings


This data set contains 2,153,471 users, 1,143,092 venue, 1,021,970 check-ins, 27,098,490 social connections, and 2,809,581 ratings that users assigned to venues; all extracted from the Foursquare application through the public API.
Now for edge weight calculation, ratings(for the similar rating information) and social graph(for their friendship information) files are used.

Edge Weight Calculation Between Users :-
	Before beginning the clustering of the groups, weights are assigned between all the users which would give the idea of how much these users are related to each other. For this, Common location of users as well as their ratings are considered. Hence the formula to calculate the edge weights between users is :-

Weight = Common Location Ratio(CLR) x Correlation between ratings of common locations(CoR) + Friendship Bias(FB)

Here, Common Location Ratio is the measure which quantifies how closely the two users are located from each other.
Common Location Ratio α(u i , u j ), measuring the degree of overlap in visited locations, is defined as α(u i , u j ) =   |  Li   INTERSECT    Lj|
		                                                                                                                       |  Li        UNION        Lj|


Correlation between Users is the measure which quantifies how similar are the ratings of the common hotels visited among the users.
Correlation (Cor) = ( Similarity – NonSimilarity) 
                     |  Li   INTERSECT    Lj|

Friendship Bias is the constant value which denotes if User A is a friend of UserB.




Floating of edges and clustering (Online Algorithms) :-
	Clustering of Graph will be done by either of the below mentioned Algorithms :-
1)	Condensed Spanning Tree (CST)
2)	Our Algorithm
Here, we are going to apply both these algorithms separately on our graph and conclude which one is providing with better results. We will continue further research with the one with better, efficient and accurate results.

Condensed Spanning Tree (CST) :-
Considering the graph with nodes and edge weights between them,
Each node u has a weight S, where S is the sum of weights of nodes in the subtree starting from node u.
The parent edge of each node u has a weight equal to E, where E is the sum of weights of the edges that emanate from the subtree to a node not in the subtree.
Also there are two methods to propogate across the graph, 
Node Weight Propagation - To update size of subtree.
Edge Weight Propagation - To update edge-cut cost.

In context of a streaming model where edges arrive in arbitrary order.There are three case: 
If both nodes u and v do not appear in the tree
From an incoming edge only one of its adjacent nodes exists in the CST
If both incoming edge nodes have already been placed in the structure by previous insertions
 
On-Demand Partitioning :-
The Graph Partitioning Algorithm is transformed into a Multiple Knapsack Problem by considering nodes as “items”.
The edge weight of each node E corresponds to the edge cut cost.
The subtree weight S corresponds to the size of each item.
Size of each partition/knapsack = |V| / k.

Amortized Cost = E / S
               = edge-cut cost / size of subtree
The algorithm sorts nodes in ascending order based on the amortized cost.
Select nodes from the list that do not violate the balance (bin capacity) criterion.
When k-1 partitions have been formed, the algorithm assigns the remaining nodes to the last, kth partition.


Our Algorithm :-
	Partitioning :::
•	Stream the edges of the graph one by one.
•	Check for the 3 cases mentioned below.
•	Load the necessary clusters.
•	Assign node to the cluster.
•	Apply Link Prediction Algorithm in the cluster at particular node count in cluster.
•	Break the cluster if size > threshold.
•	Update the cluster accordingly.

Data Structures And Heuristics :::
●	Hash table to store the User’s cluster hashed with the UserId.
●	Store the cluster as an adjacency list.
●	Load only one cluster at a time.
●	At most two clusters will be present in the memory at any point of time
●	Divide the cluster if size > Max_Cluster_Size 

Forming the Clusters :::
We will take the streaming edges as input and each edge will be the part of one of the three cases mentioned below.

CASE 1 : A ∉ K and B ∉ K
K: Set of all the clusters formed till now.
Here both the nodes are not part of any cluster.
In this case, we make a new cluster C and add both the nodes to this cluster. 

CASE 2 : A ∈ C1 and B ∉ K
K: Set of all the clusters formed till now.
Here A belongs to some cluster C1 and B does not belong to any cluster.
We add node B to the same cluster C1.

CASE 3 : A ∈ C1 and B ∈ C2
In this case A belongs to a cluster C1 and B belongs to some other cluster C2
We will compare the sizes of cluster C1 and C2.
1.	If one of clusters is smaller than the other, smaller cluster will join the bigger one.
2.	If both clusters have comparatively same sizes and suppose A have more connections than B, we will add B to cluster C1

Special CASE :
Users who belong to this case are users who visited common locations (hotels) and rated the hotels in the similar pattern yet they are NOT friends
These users might belong to a common geographic location (inferred implicitly).
So we don’t want to lose recommendations by such users which is why we will be applying Link prediction algorithm.

Link prediction algorithm :-
●	If node A and node B have similarity of 0.77 (> 0.75)
●	If node B and C have similarity of 0.76 (> 0.75)
●	Then there is a high chance of similarity between node A and node C
●	So the weight of edge AC can be derived with the same formula, which can result in following three cases : 

1.	w(AC) > 0.75 
2.	w(AC) = 0.75
3.	w(AC) < 0.75

Now if new edge CD comes and it’s weight is 0.80 (> 0.75): 
As derived w(AC) = 0.76 (previous slide case 1).
We can run the same algorithm to derive w(AD)
So now user A is connected to user B, user C, and user D.
Result: The partitions will become more cohesive
	
Stopping condition :- 
Here, As w(ED) <= 0.75, So edge won’t be formed.
The one discussed is our own algorithm.
Other link prediction algorithms are also being considered


4)	Experimental Results :-
		//Code’s Screenshot :::

//Edge Weight File Screenshot :::
//grep -w "1"  socialgraph_weight.dat     		grep -w "0.5"  socialgraph_weight.dat
 
 

5)	Interpretation of Results :-
		Preprocessing the data :::
●	Incomplete data points in the Venues (no location data) removed. 
●	Corresponding check ins, and ratings for these venues were removed.
●	Multiple ratings were averaged(Ceiling function used as well)
Calculating the weights :::
●	Successfully completed the implementation of calculating the weights of the edges in the social graph.
Edge Weight = Common Location Ratio x Correlation b/w Users + Friendship Bias
1.	Common Location Ratio Range: [0,1]
2.	Correlation b/w users Range: [(-1),1]
3.	Friendship Bias: 0.5
4.	4.1  Edge weight is between the total range of [0.5,1]                                    
4.2  Ranges: 4.2.1 [0.5,0.75) = (-ve) Correlation  
                        4.2.2 [0.75] = Zero Correlation

6)	Challenges Encountered :-
	In the data set :::
1.	Check-ins table dataset had missing latitude and longitude values.
2.	Presence of multiple ratings of the same hotel by the same user without timestamp.
3.	 Discrepancy between data in check-in and rating dataset.
In edge weight calculation :::
1.	Hard to visualise data due to presence of large data set. 
2.	Challenge in scaling down the range of edge weight from [-1,1] to the acceptable range of [0.5,1].
3.	Decision of data storage structure to use during processing of social graph table and ratings table(Array of linked List) 
