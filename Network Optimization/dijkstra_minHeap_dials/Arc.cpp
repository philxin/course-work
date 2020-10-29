#include<iostream>
#include "Arc.h"
using namespace std;


Arc::Arc()
{
	this->cost = -100;
	this->from = -1;
	this->to = -1;
}

//use node's ID to construct
Arc::Arc(int sourceNode, int destinationNode, float cost)
{
	this->cost = cost;
	this->from = sourceNode;
	this->to = destinationNode;
}

int Arc::getFrom()
{
	return from;
}

int Arc::getTo()
{
	return to;
}

int Arc::getCost()
{
	return cost;
}

void Arc::setFrom(int FROM)
{
	this->from = FROM;
}

void Arc::setTo(int TO)
{
	this->to = TO;
}

void Arc::setCost(float COST)
{
	this->cost = COST;
}