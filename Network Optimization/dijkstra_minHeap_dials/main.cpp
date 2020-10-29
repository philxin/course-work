/*
   This code can be compiled and run ok.

   This is to read a file of a network and find 1-all shortest path, with min-heap and Dial's implementations,
    with the given source node.
   
   usage:
    dijkstra_minHeap_dials xxx
     where xxx is input network filename, e.g. test1.sp
  
   input file:
     xxx
	   
   output files:
     none
	   
   compile:
     g++ -o dijkstra_minHeap_dials main.cpp Arc.h Arc.cpp
   
   pseudocode:

  	Read the input file, and deal with lines in the file based on each's first letter.
  	For each arc, it will be saved in a adjancency list and forward star form.

		Ask user to input a source node to run Dijkstra algorithm with two kinds of implementation, and print out the result.
    
    min-heap implementation:
  
    Initialize;
    dist[source] = 0;
    put the sourceNode into min_heap;

    while(min_heap isn't empty)
    {
      let i = the root of the min_heap, which is the node that has the smallest distance from source;
      remove i from min_heap;

      for every node j belongs to arc (i, j)
      {
        if(dist[j] > dist[i] + cost(i, j))
        {
          dist[j] = dist[i] + cost(i, j);
          predecessor[j] = i;
          put j into min_heap;
          build_min_heap;
        }
      }
    }
    
    Dials implementation:
    
    Similar to the min-heap implementation, but this uses buckets with distance label 
      instead of a heap as the data structure.

    Whenever dist(j) is updated, update the buckets, which is move j from 
      the original bucket to the bucket with the new and smaller distance label,
      so that the simple bucket scheme remains true.

    Then look for the minimum non-empty bucket.
    To find the minimum non-empty bucket, start where you last left off, and repeatedly scan buckets with higher numbers.
 
   coded by Hsin Chen,
   date: 2018.04.29
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
#include <queue>

using namespace std;

typedef pair<float, int> pair_SP;  //pair(distance of the node from src, index of the node)

void addArc(vector<vector<Arc>> & adjacencyList, int from, int to, float cost);
void min_heapify(vector<pair_SP> heap, int i);
void build_min_heap(vector<pair_SP> A);
void dijkstra_minHeap(int src, int n, vector<vector<Arc>> & adjacencyList, 
      vector<bool> & isVisited, vector<int>& pred, vector<float>& distance);
void dijkstra_dials(int src, int n, int C, vector<vector<Arc>> & adjacencyList, 
      vector<bool> &isVisited, vector<int>& pred, vector<int>& distance);

int getMaxArcLength(vector<vector<Arc>> & adjacencyList);

int main()
{

  string filePath = "";
  //filePath = "test1.sp";
  cout<<"Please input network filename: ";
  cin>> filePath;


  ifstream ifs;
  ifs.open(filePath.c_str());
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



  int sourceNode;
  int sinkNode;
  cout<<endl<<"Input source node: ";
  cin>> sourceNode;

  vector<int> predecessor;
  vector<float> distance;
  vector<bool> isVisited;
  dijkstra_minHeap(sourceNode, numOfNodes, adjList, isVisited, predecessor, distance);

  cout<<"\n\nmin-heap implementation:"<<endl;

  for(sinkNode = 1; sinkNode <= numOfNodes; sinkNode++)  
  {
    if(sourceNode!=sinkNode)
    {
      int trace_node = sinkNode;
      
      if(isVisited[sinkNode-1])
      {
        cout<<endl<<sourceNode<<"->"<<sinkNode<<":"<<"["<<distance[sinkNode-1]<<"] ";
        while(trace_node!=sourceNode)
        {
          cout<<trace_node<<"<-";
          int temp = predecessor[trace_node-1];
          trace_node = temp;
        }
        cout<<sourceNode;
      }
      else cout<<endl<<sourceNode<<"->"<<sinkNode<<":"<<"[can not reach] ";
    }
  }

  cout<<"\n\n\n"<<"Dial's implementation:"<<endl;

  vector<int> dist_dials;
  vector<bool> visited;
  vector<int> preds;

  dijkstra_dials(sourceNode, numOfNodes, getMaxArcLength(adjList), adjList, 
                    visited, preds, dist_dials);

  for(sinkNode = 1; sinkNode <= numOfNodes; sinkNode++) 
  {
    if(sourceNode!=sinkNode)
    {
      int trace_node = sinkNode;
      
      if(visited[sinkNode-1])
      {
        cout<<endl<<sourceNode<<"->"<<sinkNode<<":"<<"["<<dist_dials[sinkNode-1]<<"] ";
        while(trace_node!=sourceNode)
        {
          cout<<trace_node<<"<-";
          int temp = preds[trace_node-1];
          trace_node = temp;
        }
        cout<<sourceNode;
      }
      else cout<<endl<<sourceNode<<"->"<<sinkNode<<":"<<"[can not reach] ";
    }
  }
  return 0;
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


void dijkstra_minHeap(int src, int n, vector<vector<Arc>> & adjacencyList, 
      vector<bool> &isVisited, vector<int>& pred, vector<float>& distance) 
{
  isVisited = vector<bool>(n, false);
  
  pred = vector<int>(n, -1);
  pred.insert(pred.begin() + src-1, src);

  distance = vector<float>(n, 1e9);
  distance[src-1] = 0;

  //vector<pair_SP> minHeap;
  //minHeap.push_back(make_pair(0, src));
  priority_queue< pair_SP, vector<pair_SP>, greater<pair_SP> > minHeap;
  minHeap.push(make_pair(0, src));

 // cout<<"enter while loop\n";

  while(!minHeap.empty())
  {
    //int node = minHeap[0].second;
   // minHeap[0] = minHeap.back();
   //minHeap.pop_back();
    int node = minHeap.top().second;
    minHeap.pop();

    //build_min_heap(minHeap);
    if(!isVisited[node-1])
    {
      for (unsigned int i = 0; i < adjacencyList[node-1].size(); i++)
      {
        int cost = adjacencyList[node-1][i].getCost();
        int node_2 = adjacencyList[node-1][i].getTo();

        if(!isVisited[node_2-1])
        {
          if(distance[node-1] + cost < distance[node_2-1])
          {
            if(distance[node_2-1] == 1e9)

          }
          distance[node_2-1] = distance[node-1] + cost;
          pred[node_2-1] = node;
          //minHeap.push_back(make_pair(distance[node_2-1], node_2));
          //build_min_heap(minHeap);
          minHeap.push(make_pair(distance[node_2-1], node_2));
        }
      }
    }

    isVisited[node-1] = true;
  }
}

void dijkstra_dials(int src, int n, int C, vector<vector<Arc>> & adjacencyList, 
      vector<bool> &isVisited, vector<int>& pred, vector<int>& distance)
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
      else if( isVisited[nodeToReach - 1] && 
                (distance[nodeToReach - 1] > 
                currentIndex + static_cast<int>(adjacencyList[currentNode-1][i].getCost())) ) //the node is visited and
                                                                            // found shorter distance
      {
        int oldDistance = distance[nodeToReach- 1];
        int newDistance = currentIndex + static_cast<int>(adjacencyList[currentNode-1][i].getCost());
        distance[nodeToReach - 1] = newDistance; //update distance
        pred[nodeToReach - 1] = currentNode;

        bucket[oldDistance].erase(std::remove( bucket[oldDistance].begin(), bucket[oldDistance].end(), 
                  nodeToReach ), bucket[oldDistance].end());
        //remove the node from the longer distance bucket
        // v.erase( std::remove( v.begin(), v.end(), 5 ), v.end() ); 

        bucket[newDistance].push_back(nodeToReach);   //put the node into the shorter distance bucket

      }
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