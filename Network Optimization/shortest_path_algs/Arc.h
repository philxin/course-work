#include<iostream>
#include<vector>

using namespace std;

#ifndef Arc_h
#define Arc_h

class Arc
{
	
	
private:	
	int from; //tail's ID
	int to; //head's ID
	float cost;
	//int capacity;


public:
    Arc();
	Arc(int, int, float);
	int getFrom();
	int getTo();
	int getCost();
	void setFrom(int);
	void setTo(int);
	void setCost(float);

};


#endif // !Arc_h
