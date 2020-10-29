/*
This code can be compiled and run ok, but the output of ploynomial isn't in order.
This program is to sum up polynomials.

Purpose: To add ploynomails and output the result.

Usage:
./a.exe

Compile:
g++ polynomial.cpp


Coded by Hsin Chen,
2014/11/20

*/

#include <iostream>
#include <fstream>
using namespace std;

struct term
{
	double coe;
	double exp;
	term* next;
};



int main()
{
	ifstream file("input.txt");
	int ploy_n;
	file>>ploy_n;
	term **ploys;		//to save all data.
	term **ploy_temp;	//to temporarily save data.
	term *head;
	int *ploy_terms;	//to meomorize how many terms each ploynomial has. 
	ploy_terms = new int [ploy_n];
	
	ploy_temp = new term* [ploy_n];	//because we don't know how big the terms are, 
	for(int i=0; i<ploy_n; i++)	//so guess that they will not bigger than 100.
		ploy_temp[i] = new term [100];
	
	
 	ploys = new term*[ploy_n];			//after getting the longest ploynomials
										//can allocate a two dimension array.
	
	for(int i=0; i<ploy_n; i++)			//save input data in temporary array.
	{
		file>>ploy_terms[i];
		for(int j=0; j<ploy_terms[i]; j++)
		{
			file>>ploy_temp[i][j].coe;
			file>>ploy_temp[i][j].exp;
		}
	}
	
	int key_big = ploy_terms[0]; 
	for(int i=1; i<ploy_n; i++)		//to find the biggest number of 
	{								//ploynomial's terms.
		if( ploy_terms[i] > key_big )
		{	
			key_big = ploy_terms[i];				
		}
	}
	
	for(int k=0; k<ploy_n; k++)			//use the biggest number of ploynomial's terms
		ploys[k] = new term [key_big];	//to allocate a two dimension array.
	
	for(int i=0; i<ploy_n; i++)		//copy data from temporary array to another array.
	{
		for(int j=0; j<ploy_terms[i]; j++)
		{
			ploys[i][j].coe = ploy_temp[i][j].coe;
			ploys[i][j].exp = ploy_temp[i][j].exp;
		}
	}
	
	for(int j=0; j< ploy_terms[0]-1; j++)		//use linked list to connect datas.
			ploys[0][j].next = &ploys[0][j+1];
	
	ploys[0][ploy_terms[0]-1].next =NULL;	//make the last element point to NULL.
	
	for(int i=0; i<ploy_n; i++)			//delete temporary array.
		delete [] ploy_temp[i];
	delete [] ploy_temp;
	
	head = &ploys[0][0];				
	term *key;
	int ctr = 0;						//to count how many terms are add to the
	for(int i=1; i<ploy_n; i++)			//final ploynomial.
	{
		for(int j=0; j<ploy_terms[i]; j++)//addition of terms.
		{
			key = head;
			while(key->exp!=ploys[i][j].exp && key->next != NULL)
			{ 
				key = key->next;
			}
			if(key->exp == ploys[i][j].exp)
				key->coe += ploys[i][j].coe;
				else
				{
					ploys[i][j].next = head;	
					head = &ploys[i][j];						
					ctr++;
				}
		}
	} 
	
	term *init_head = head;
	for(int j=0; j<ploy_terms[0]+ctr; j++)	//sort exp in descending order
	{											//by move the smallest exp to the
		term *sort_key = init_head;				//head everytime.
		term *smallest = init_head;
		term *small_prev = NULL;
		while(sort_key->next != NULL)
		{
			if(sort_key->exp > sort_key->next->exp)
			{
				smallest = sort_key->next;
				small_prev = sort_key;
			}
			if(sort_key->next != NULL)
				sort_key = sort_key->next;
		}
		
		if(small_prev != NULL)
		{
			small_prev->next = smallest->next;
			smallest->next = head;
			head = smallest;
		}
		else
		{
			term *prev_key = head;
			while(prev_key->next!=smallest)
			{
				prev_key = prev_key->next;
			}
			init_head = init_head->next;
			prev_key->next = smallest->next;
			smallest->next = head;
			head = smallest;
		} 
	} 
	
	term *print_key = head;
	for(int i=0; i<ploy_terms[0]+ctr; i++)	//print the result.
	{
		cout<< print_key->coe<<"x^"<<print_key->exp;
		if(print_key->next!=NULL)
		{
			if(print_key->next->coe >= 0)
				cout<<"+";
				print_key = print_key->next;
		}
	} 
	 
	for(int i=0; i<ploy_n; i++)		//delete the two dimension array.
		delete [] ploys[i];
	delete [] ploys;
	
	return 0;
}
	
