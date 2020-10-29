/*
   This code can be compiled and run ok.

   This is to read a file of a network and print out arcs connecting with the given node.

   Format in test2.sp:
   	"c" as the start letter: comment
   	"p" as the start letter: problem type
   	"t" as the start letter: problem name
   	"a" as the start letter: an arc in the network
   
   usage:
    connect_network xxx
     where xxx is input network filename, e.g. test2.sp
  
   input file:
     xxx
	   
   output files:
     none
	   
   compile:
     g++ -o connect_network connect_network.cpp
   
   pseudocode:

  	Read the input file, and deal with lines in the file based on each's first letter.
  	For each arc, it will be saved in a dynamically allocated adjancency matrix.

	Ask user to input a source node index (or input -1 to end the program), 
	scan the row and column of that index in the adjancency matrix,
	and print out every arc connects with the source node.
 
   coded by Hsin Chen,
   date: 2018.03.14
*/

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

struct arc
{
	int index;
	int from;
	int to;
	float length;
};

int main()
{

	string filePath = "";

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

	arc ** adjacencyMatrix;  //dynamically allocated memory for 2d array

	if (ifs.is_open())
	{
		while (getline(ifs, line))	//get each line of the file
		{
			char letter;
			stringstream ss(line);
			ss >> letter;

			switch (letter)		//use the first character to determine what to do 
			{
				case 'c':		//comment
					break;

				case 'p':		//problem type
				{
					ss >> problem_type;
					ss >> numOfNodes;
					ss >> numOfArcs;

					adjacencyMatrix = new arc*[numOfNodes];		//initialize adjacency matrix

					for (int i=0; i<numOfNodes; i++)
						adjacencyMatrix[i] = new arc[numOfNodes];

					float M = 10000;
					for(int i = 0; i< numOfNodes; i++)
					{
						for(int j = 0; j<numOfNodes; j++)
						{
							arc tempArc;
							tempArc.index = 0;
							tempArc.from = -1;
							tempArc.to = -1;
							tempArc.length = M;
							adjacencyMatrix[i][j] = tempArc;
						}
					}
				}
					break;

				case 't':		//problem name
					ss >> problem_name;
					break;

				case 'a':		//read each arc
				{
					int from;
					int to;
					float length;
					arc tempArc;

					ss >> from;
					ss >> to;
					ss >> length;

					tempArc.index = indexCtr;
					tempArc.from = from;
					tempArc.to = to;
					tempArc.length = length;

					adjacencyMatrix[from-1][to-1] = tempArc;
					indexCtr++;
				}
					break;
			}
		}
		ifs.close();
	}
	else 
	{
		cout << "Failed to read the file."<<endl;	//if fail to read the file, terminate the program
		exit (EXIT_FAILURE);;
	}

	
	int sourceNode = 0;
	while(sourceNode != -1)		//print arcs by inputting a source node or leave
	{
		cout<<"Please input a source node or -1 to leave: ";
		cin>>sourceNode;
		
		if((sourceNode >= numOfNodes || sourceNode < 1) && sourceNode != -1) 	//avoid invalid input
			cout<<"!!Warning!!: node "<< sourceNode <<" does not exist."<<endl;
		else if(sourceNode != -1)
		{
			//scan row and column of the source node index in the adjancency matrix,
			//and print out the arcs connect to the source node
			for(int i = 0; i < numOfNodes; i++)
			{
				if(adjacencyMatrix[i][sourceNode-1].length != 10000)
					cout<<"arc[" << adjacencyMatrix[i][sourceNode-1].index << "]: " 
						<< adjacencyMatrix[i][sourceNode-1].from <<"-" << adjacencyMatrix[i][sourceNode-1].to << " : "
						<< adjacencyMatrix[i][sourceNode-1].length <<endl;
			}

			for(int i = 0; i < numOfNodes; i++)
			{
				if(adjacencyMatrix[sourceNode-1][i].length != 10000)
					cout<<"arc[" << adjacencyMatrix[sourceNode-1][i].index << "]: " 
						<< adjacencyMatrix[sourceNode-1][i].from <<"-" << adjacencyMatrix[sourceNode-1][i].to << " : "
						 << adjacencyMatrix[sourceNode-1][i].length <<endl;
			}
			
		}
		else break;

	}
	

	for(int i=0; i<numOfNodes; i++)		//release the memory space of adjacency matrix
		delete [] adjacencyMatrix[i];

	delete [] adjacencyMatrix;
	
	return 0;
}
