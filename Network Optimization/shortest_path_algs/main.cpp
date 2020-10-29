/*
   This code can be compiled and run ok.

   This is to read a file of a network and solve all-to-all shortest path problems with 6 algorithms:
   DIKH, DIKD, BF, PAPE, FWA and FWG. 

   DIKH: Dijkstra’s Algorithm with min-heap implementation
   DIKD: Dijkstra’s Algorithm with Dial’s Implementation
   BF: Bellman-Ford Algorithm (FIFO modified Label Correcting)
   PAPE: Pape Algorithm (modified Label Correcting with dequeue implementation)
   FWA: Floyd-Warshall Algebraic algorithm
   FWG: Floyd-Warshall Graphical algorithm

   
   usage:
    ./shortest_path_algs 
    Then input 2 parameters: (# of iterations for each alg, xxx)
     where xxx is input network filename, e.g. test1.sp
	
     e.g. to run 3 times with network file test1.sp:
    	  ./shortest_path_algs
     	  3 test1.sp
  
   input file:
     xxx
	   
   output files:
     none
	   
   compile:
     g++ -o shortest_path_algs main.cpp Arc.h Arc.cpp
   
   pseudocode:

  	Read the time of iterations and the input file, then test the network with each algorithms and 
  	print out the computational time and times of nontrivial triple comparisons.


   coded by Hsin Chen,
   date: 2018.06.25
*/

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <vector>
#include "Arc.h"
#include <sys/types.h>
#include <sys/times.h>
#include <queue>


using namespace std;

float mytimer();
typedef pair<float, int> pair_SP;  //pair(distance of the node from src, index of the node)

void addArc(vector<vector<Arc>> & adjacencyList, int from, int to, float cost);
void min_heapify(vector<pair_SP> heap, int i);
void build_min_heap(vector<pair_SP> A);
void dijkstra_minHeap(int src, int n, vector<vector<Arc>> adjacencyList, 
      vector<bool>& isVisited, vector<int>& pred, vector<float>& distance, int & ctr) ; 
void dijkstra_dials(int src, int n, int C, vector<vector<Arc>> adjacencyList, 
      vector<bool> &isVisited, vector<int>& pred, vector<int>& distance, int & ctr);
void BF(int src, int n, vector<vector<Arc>> adjacencyList, 
       vector<int>& pred, vector<float>& distance, int & ctr);
void PAPE(int src, int n, vector<vector<Arc>> adjacencyList, 
       vector<int>& pred, vector<float>& distance, int & ctr) ;
void FWA(int n, vector<vector<Arc>> adjacencyList, vector<vector<float>> & dist_mtrx,
     vector<vector<int>> & pred_mtrx, int & ctr);
void FWG(int n, vector<vector<Arc>> & adjList, vector<vector<int>> & pred_mtrx, int & ctr);
int getMaxArcLength(vector<vector<Arc>> & adjacencyList);

int main()
{
	int n_it;
	string inputFile;
	string outputFile;

	cin>> n_it >> inputFile;

  ifstream ifs;
  ifs.open(inputFile.c_str());
  string line;

  string problem_name;
  string problem_type;
  int numOfNodes;
  int numOfArcs;
  int indexCtr = 1;

  vector<vector<Arc>> adjList;  //resizable containers, which can dynamically allocate themselves
  
  //for forward star representation
  vector<int> point; 
  vector<int> tail;
  vector<int> head;
  vector<int> cost;

  if (ifs.is_open())
  {
    while (getline(ifs, line))  //get each line of the file
    {
      char letter;
      stringstream ss(line);
      ss >> letter;

      switch (letter)   //use the first character to determine what to do 
      {
        case 'c':   //comment
          break;

        case 'p':   //problem type
        {
          ss >> problem_type;
          ss >> numOfNodes;
          ss >> numOfArcs;

          adjList.resize(numOfNodes);
          point.resize(numOfNodes+1);
          tail.resize(numOfArcs);
          head.resize(numOfArcs);
          cost.resize(numOfArcs);
        }
          break;

        case 't':   //problem name
          ss >> problem_name;
          break;

        case 'a':   //read each arc
        {
          int from;
          int to;
          float length;

          ss >> from;
          ss >> to;
          ss >> length;

          addArc(adjList, from, to, length);
        }
          break;
      }
    }
    ifs.close();
  }
  else 
  {
    cout << "Failed to read the file."<<endl; //if fail to read the file, terminate the program
    exit (EXIT_FAILURE);;
  }


//---------DIKH
   float dikh_t1 = 0, dikh_t2 = 0;
   dikh_t1 = mytimer();
   int dikh_ctr = 0;     //nontrivial triple comparison counter for DIKH
   for(int it = 0; it < n_it; it++)
   {
	  for(int sourceNode = 1; sourceNode <= numOfNodes; sourceNode++)
	  {
		  vector<int> predecessor;
		  vector<float> distance;
		  vector<bool> isVisited;
		  dijkstra_minHeap(sourceNode, numOfNodes, adjList, isVisited, predecessor, distance, dikh_ctr);
		}
	}

	dikh_t2 = mytimer();
	printf("%s%d%s%f%s%s%d", "\r\nDIKH ", n_it, " iterations time: ", (dikh_t2 - dikh_t1) / CLOCKS_PER_SEC, "  ",
		"NTTCs: ", dikh_ctr);

//------DIKD
	int dikd_ctr = 0;
	float dikd_t1 = 0, dikd_t2 = 0;
	dikd_t1 = mytimer();
	for(int it = 0; it < n_it; it++)
   	{
		for(int sourceNode = 1; sourceNode <= numOfNodes; sourceNode++)
		{
		  vector<int> dist_dials;
		  vector<bool> visited;
		  vector<int> preds;

		  dijkstra_dials(sourceNode, numOfNodes, getMaxArcLength(adjList), adjList, 
		                    visited, preds, dist_dials, dikd_ctr);
		}
	}
	dikd_t2 = mytimer();
	printf("%s%d%s%f%s%s%d", "\r\nDIKD ", n_it, " iterations time: ", (dikd_t2 - dikd_t1) / CLOCKS_PER_SEC, "  ",
		"NTTCs: ", dikd_ctr);

//-------BF
	float bf_t1 = 0, bf_t2 = 0;
	int bf_ctr=0;
	bf_t1 = mytimer();
	for(int it = 0; it <n_it; it++)
	{
		for(int sourceNode = 1; sourceNode <= numOfNodes; sourceNode++)
		{
			vector<int> pred;
			vector<float> dist;
			BF(sourceNode, numOfNodes, adjList, pred, dist, bf_ctr);

		}
	}
	bf_t2 = mytimer();
	printf("%s%d%s%f%s%s%d", "\r\nBF ", n_it, " iterations time: ", (bf_t2 - bf_t1) / CLOCKS_PER_SEC, "  ",
		"NTTCs: ", bf_ctr);

//----PAPE
	int pape_ctr = 0;
	float pape_t1 = 0, pape_t2 = 0;
	pape_t1 = mytimer();
  	//PAPE(sourceNode, numOfNodes, adjList, predecessor, distance, pape_ctr);
  	for(int it = 0; it <n_it; it++)
	{
		for(int sourceNode = 1; sourceNode <= numOfNodes; sourceNode++)
		{
			vector<int> pred;
			vector<float> dist;
			PAPE(sourceNode, numOfNodes, adjList, pred, dist, pape_ctr);
		}
	}
	pape_t2 = mytimer();
	printf("%s%d%s%f%s%s%d", "\r\nPAPE ", n_it, " iterations time: ", (pape_t2 - pape_t1) / CLOCKS_PER_SEC, "  ",
		"NTTCs: ", pape_ctr);


//---FWA
 	int fwa_ctr = 0;
 	float fwa_t1 = 0, fwa_t2 = 0;
	fwa_t1 = mytimer();
 	for(int it = 0; it < n_it; it++)
 	{
 	  vector<vector<float>> distanceMatrix;
	  vector<vector<int>> predMatrix;

	  FWA(numOfNodes, adjList, distanceMatrix, predMatrix, fwa_ctr);
		
 	}
  	fwa_t2 = mytimer();
  	printf("%s%d%s%f%s%s%d", "\r\nFWA ", n_it, " iterations time: ", (fwa_t2 - fwa_t1) / CLOCKS_PER_SEC, "  ",
		"NTTCs: ", fwa_ctr);

 //-------FWG
  	int fwg_ctr = 0;
  	float fwg_t1 = 0, fwg_t2 = 0;
	fwg_t1 = mytimer();

  	for(int it = 0; it <n_it; it++)
  	{
  		vector<vector<Arc>> adj_list = adjList;
		vector<vector<int>> pred_Matrix;

		FWG(numOfNodes, adj_list, pred_Matrix, fwg_ctr);
    }
  fwg_t2 = mytimer();
  printf("%s%d%s%f%s%s%d\r\n\r\n", "\r\nFWG ", n_it, " iterations time: ", (fwg_t2 - fwg_t1) / CLOCKS_PER_SEC, "  ",
		"NTTCs: ", fwg_ctr);
  
  return 0;
}


float mytimer()
{ struct tms hold;
 
 times(&hold);
 /*return  (float)(hold.tms_utime) / 60.0;*/
 return  (float)(hold.tms_utime);
}

//to save arcs in adjacency list 
void addArc(vector<vector<Arc>> &adjacencyList, int from, int to, float cost)
  {
   if (adjacencyList[from-1].empty())
    {
      adjacencyList[from-1].push_back(Arc(from, to, cost));
    }
    else if(to>adjacencyList[from-1].back().getTo())
    {
       adjacencyList[from-1].push_back(Arc(from, to, cost));
    }
    else    //let the head node in the adj list in order
    { 
      vector<Arc>  :: iterator iter ;
      for(iter = adjacencyList[from-1].begin(); iter != adjacencyList[from-1].end()  ;++iter)
        if(to < (*iter).getTo()) break;

      adjacencyList[from-1].insert(iter, Arc(from, to, cost));
    }
  }

//a procedure to build min heap
void min_heapify(vector<pair_SP> heap, int i)
{
  int l = 2*i + 1;  //index of left child of index i
  int r = 2*i + 2;  //index of right child of index i
  int min;        // index of the min element among heap[l].first, heap[r].first, heap[i].first

  if(l<= heap.size()-1 && heap[l].first < heap[i].first) 
    min = l;
  else min = i;

  if(r<= heap.size()-1 && heap[r].first < heap[min].first)
    min = r;

  if(min!=i) 
  {
    pair<float, int> a(heap[i]);
    heap[i] = pair<float, int>(heap[min]);
    heap[min] = pair<float, int>(a);

    min_heapify(heap, min);
  }
}

void build_min_heap(vector<pair_SP> A)
{
  for(int i=(A.size()/2)-1; i>=0; i--)
    min_heapify(A, i);
}

//ctr: non-trivial trple comparison counter
void dijkstra_minHeap(int src, int n, vector<vector<Arc>> adjacencyList, 
      vector<bool>& isVisited, vector<int>& pred, vector<float>& distance, int & ctr) 
{
  isVisited = vector<bool>(n, false);
  
  pred = vector<int>(n, -1);
  pred.insert(pred.begin() + src-1, src);

  distance = vector<float>(n, 1e9);
  distance[src-1] = 0;


  priority_queue< pair_SP, vector<pair_SP>, greater<pair_SP> > minHeap;
  minHeap.push(make_pair(0, src));

  while(!minHeap.empty())
  {
  	int node = minHeap.top().second;
    minHeap.pop();

    if(!isVisited[node-1])
    {
      for (unsigned int i = 0; i < adjacencyList[node-1].size(); i++)
      {
        int cost = adjacencyList[node-1][i].getCost();
        int node_2 = adjacencyList[node-1][i].getTo();

        if(!isVisited[node_2-1])
        {
        	if(distance[node_2-1] !=1e9)
            	ctr++;
            
          if(distance[node-1] + cost < distance[node_2-1])
          {

            distance[node_2-1] = distance[node-1] + cost;
	        pred[node_2-1] = node;

	        minHeap.push(make_pair(distance[node_2-1], node_2));
          }
          
        }
      }
    }

    isVisited[node-1] = true;

  }
}

void dijkstra_dials(int src, int n, int C, vector<vector<Arc>> adjacencyList, 
      vector<bool> &isVisited, vector<int>& pred, vector<int> & distance, int & ctr)
//C: the largest arc length in the network
{
  isVisited = vector<bool>(n, false);
  
  pred = vector<int>(n, -1);
  pred.insert(pred.begin() + src-1, src);

  distance = vector<int>(n, 1e9);
  distance[src-1] = 0;

  vector<vector<int>> bucket;   //the bucket for dial's implementation; 
                                //the index implis the distance from the source

  bucket.resize(n*C + 1);
  int currentIndex = 0; //record the current smallest distance label
  bucket[currentIndex].push_back(src);

  while(currentIndex < bucket.size())
  {
    int currentNode = bucket[currentIndex].back();    //the node to reach out others 
    for(int i = 0; i< adjacencyList[currentNode-1].size(); i++)
    {
      int nodeToReach = adjacencyList[currentNode-1][i].getTo();

      if(!isVisited[nodeToReach - 1]) //if the node hasn't been visited
      {
          isVisited[nodeToReach - 1] = true;  //mark it as visisted
          pred[nodeToReach - 1] = currentNode;
         
          int newDistance = currentIndex + static_cast<int>(adjacencyList[currentNode-1][i].getCost());
          distance[nodeToReach - 1] = newDistance;    //update distance
                      
          bucket[newDistance].push_back(nodeToReach);   //put the node into the bucket

      }
      else if(distance[nodeToReach - 1] > 
                currentIndex + static_cast<int>(adjacencyList[currentNode-1][i].getCost()) ) //the node is visited and
                                                                            // found shorter distance
      {
        ctr++;
        int oldDistance = distance[nodeToReach- 1];
        int newDistance = currentIndex + static_cast<int>(adjacencyList[currentNode-1][i].getCost());
        distance[nodeToReach - 1] = newDistance; //update distance
        pred[nodeToReach - 1] = currentNode;

        bucket[oldDistance].erase(std::remove( bucket[oldDistance].begin(), bucket[oldDistance].end(), 
                  nodeToReach ), bucket[oldDistance].end());

        bucket[newDistance].push_back(nodeToReach);   //put the node into the shorter distance bucket
      }
      else ctr++;
    }

    //remove the current node from the bucket
    bucket[currentIndex].erase(std::remove( bucket[currentIndex].begin(), 
            bucket[currentIndex].end(), currentNode ), bucket[currentIndex].end()); 

    if(!bucket[currentIndex].empty())
      continue;
    else
    {
      while(bucket[currentIndex].empty())
        currentIndex++;
    }
  }
}

//Bellman-Ford Algorithm (FIFO modified LC)
void BF(int src, int n, vector<vector<Arc>> adjacencyList, 
       vector<int>& pred, vector<float>& distance, int & ctr) 
{
  pred = vector<int>(n+1, -1);
  pred[src-1] = src;

  distance = vector<float>(n, 1e9);
  distance[src-1] = 0;

  deque<int> list;
  list.push_back(src);

  while(!list.empty())
  {

    int node_i = list.front();    
    list.pop_front();                 //remove an element from LIST

    for (unsigned int i = 0; i < adjacencyList[node_i-1].size(); i++)
    {
      int cost = adjacencyList[node_i-1][i].getCost();
      int node_j = adjacencyList[node_i-1][i].getTo();

      if(distance[node_j -1] != 1e9)
          ctr++;

      if(distance[node_i -1] + cost < distance[node_j -1])
      {
        distance[node_j -1] = distance[node_i -1] + cost;
        pred[node_j -1] = node_i;
       
       if (std::find(list.begin(), list.end(), node_j) == list.end())   //if node j isn't in the LIST
       {
          list.push_back(node_j);
       }
      }
    }
  }
}

//Pape Algorithm (modified LC with dequeue implementation)
void PAPE(int src, int n, vector<vector<Arc>> adjacencyList, 
       vector<int>& pred, vector<float>& distance, int & ctr) 
{
  pred = vector<int>(n+1, -1);
  pred[src-1] = src;

  distance = vector<float>(n, 1e9);
  distance[src-1] = 0;

  deque<int> dequeue;
  dequeue.push_back(src);

  while(!dequeue.empty())
  {

    int node_i = dequeue.front();    
    dequeue.pop_front();                 //remove an element from LIST

    for (unsigned int i = 0; i < adjacencyList[node_i-1].size(); i++)
    {
      int cost = adjacencyList[node_i-1][i].getCost();
      int node_j = adjacencyList[node_i-1][i].getTo();

      if(distance[node_j -1] != 1e9)    //nontrivial triple comparison counts
          ctr++;

      if(distance[node_i -1] + cost < distance[node_j -1])
      {
        distance[node_j -1] = distance[node_i -1] + cost;
        pred[node_j -1] = node_i;

        int idxInList = -1;
        for(int k = 0; k< dequeue.size(); k++)
        {
            if(dequeue[k] == node_j)
            {
              idxInList = k;
              break;
            }
        }
       
       if (idxInList == -1)   //if node j isn't in the LIST
       {
          dequeue.push_back(node_j);
       }
       else
       {
          dequeue.push_front(node_j);
       }

      }
    }
  }
}

//Floyd-Warshall Algebraic alg
void FWA(int n, vector<vector<Arc>> adjacencyList, vector<vector<float>> & dist_mtrx,
     vector<vector<int>> & pred_mtrx, int & ctr)
{
  dist_mtrx.resize(n);
  for(unsigned int x = 0; x < dist_mtrx.size(); x++)
  {
    dist_mtrx[x].resize(n);
    for(int y = 0; y < dist_mtrx[x].size(); y++)
      dist_mtrx[x][y] = 1e9;
  }

  pred_mtrx.resize(n);
  for(unsigned int x = 0; x < pred_mtrx.size(); x++)
  {
    pred_mtrx[x] = vector<int>(n, x+1);
  }

  for(int x = 0; x < adjacencyList.size(); x++)
  {
    for(int y =0; y < adjacencyList[x].size(); y++)
    {
      dist_mtrx[adjacencyList[x][y].getFrom()-1][adjacencyList[x][y].getTo()-1] = adjacencyList[x][y].getCost();
    }
  }

  for(int x=0; x<n; x++)
  {
    dist_mtrx[x][x] = 0;
  }

  for(int k = 0; k < n; k++)
  {
    for(int i = 0; i < n; i++)
    {
      for(int j= 0; j< n; j++)
      {
        if(dist_mtrx[i][j] !=1e9 && dist_mtrx[i][k] != 1e9 && dist_mtrx[k][j] != 1e9)
        {
          ctr++;
        }

        if(dist_mtrx[i][j] > dist_mtrx[i][k] + dist_mtrx[k][j])
        {
          dist_mtrx[i][j] = dist_mtrx[i][k] + dist_mtrx[k][j];
          pred_mtrx[i][j] = pred_mtrx[k][j];
        }
      }
    }
  }
}

//Floyd-Warshall Graphical alg
void FWG(int n, vector<vector<Arc>> & adjList, vector<vector<int>> & pred_mtrx, int & ctr)
{
  pred_mtrx.resize(n);
  for(unsigned int x = 0; x < pred_mtrx.size(); x++)
  {
    pred_mtrx[x] = vector<int>(n, x+1);
  }

  for(int k = 1; k <= n; k++)
  {
    vector<int> to_k;   //to store the node which goes to node k
    vector<int> to_k_indices; 
    for(int i = 1; i <= adjList.size(); i++)  //check if node i -> node k
    {
      if(i != k) 
      {
        for(int x = 1; x <= adjList[i-1].size(); x++)
        {
          if(adjList[i-1][x-1].getTo() == k)
          {
            to_k.push_back(i);
            to_k_indices.push_back(x-1);
          }
        }
      }
    }

    for(int x = 0; x < to_k.size(); x++)
    {
      for(int y = 0; y < adjList[k-1].size(); y++)
      {
        int index_i = -1;
        //int index_j = -1;

        for(int z = 0; z < adjList[to_k[x]-1].size(); z++)
        {
          if(adjList[to_k[x]-1][z].getTo() == adjList[k-1][y].getTo())
          {
            index_i = z;
            //index_j = y;
            break;
          }
        }

        if(index_i == -1)   //node i hasn't connected to node k
        {
          if(adjList[k-1][y].getTo() != to_k[x])
          {
            adjList[to_k[x]-1].push_back(Arc(to_k[x], adjList[k-1][y].getTo(), 
              adjList[to_k[x]-1][to_k_indices[x]].getCost() + adjList[k-1][y].getCost()));

            pred_mtrx[to_k[x]-1][adjList[k-1][y].getTo()-1] = pred_mtrx[k-1][adjList[k-1][y].getTo()-1];
          }
        }
        else
        {
          ctr++;

          if(adjList[to_k[x]-1][index_i].getCost() >
              adjList[to_k[x]-1][to_k_indices[x]].getCost() + adjList[k-1][y].getCost())    //triple comparison
          {
            //update distance
            adjList[to_k[x]-1][index_i].setCost
              (adjList[to_k[x]-1][to_k_indices[x]].getCost() + adjList[k-1][y].getCost());

        
            pred_mtrx[to_k[x]-1][adjList[k-1][y].getTo()-1] = pred_mtrx[k-1][adjList[k-1][y].getTo()-1];
          }
          
        }
      }
    }
  }
}


//to get C (the maximum cost of a single arc in the network)
int getMaxArcLength(vector<vector<Arc>> & adjacencyList)
{
  int maxLength = -1;
  for(int i = 0; i<adjacencyList.size(); i++)
  {
    for(int j = 0; j<adjacencyList[i].size();j++)
    {
      if(adjacencyList[i][j].getCost() > maxLength)
      {
        maxLength = adjacencyList[i][j].getCost();
      }
    }
  }
  return maxLength;
}