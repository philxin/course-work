/*
This code can be compiled and run ok.

Purpose: This code is to randomly generate some numbers, copy these numbers, and sort numbers
		by insertion sort and merge sort.
Usage: Input the number of how many numbers you want to sort out, then give the random numbers'
		upper limit and lower limit, and it will output the result. 
Compile: g++ insertionSort_mergeSort.cpp

Pseudocode: 

Duplicate_array(A[], B[], length)
	for i= 1 -> length
		assign A[i] to B[i]
------------------------------
Printout_arrary()
	print every element by for loop
------------------------------
Ins_sort(A[], n)
	int key;	
	for i= n-2 -> 0
		key = A[i]
		for j= i+1 -> n-1
			if(key < A[j])
				//A[j] move forwards, key keep comparing
			else break
------------------------------
Mg_sort(B[], p, r)
	if(p<r)	
		int q = (p+r)/2;
		Mg_sort(B, p, q);
		Mg_sort(B,q+1, r);
		Merge(B, p, q, r);
------------------------------
Merge(B[], p, q, r)
	int n1 = q-p+1;
	int n2 = r-q;
	double L[n1+1], R[n2+1];
	for i=0 -> n1-1
		L[i] = B[p+i];
	for j=0 -> n2-1
		R[j] = B[q+j+1];
	L[n1] = infinitesimal;
	R[n2] = infinitesimal;
	
	int k=t=0;
	for x= p -> r
		if(L[k] >= R[t])
			B[x] = L[k];
			k++;
		else
			B[x] = R[t];
			t++;
------------------------------
	coded by Hsin Chen, ID: H34021204, email: chenshin0606@gmail.com
	2014/10/27
*/
#include <iostream>
#include <cstdlib>
#include <time.h>
using namespace std;

//copy array A to array B.
void Duplicate_array(int A[],int B[],int n)
{
	for(int i=0; i<n; i++)
			B[i] = A[i];
}

//print all elements of array.  
void Printout_arrary(int A[],int n)
{
	for(int j=0; j<n; j++)
		cout<<A[j]<<" ";
} 

//insertion sort function, to sort numbers in DESCENDING order.
void Ins_sort(int A[], int n)
{
	int key;	
		for(int i=n-2; i>=0; i--)
		{
			key = A[i];
			for(int j=i+1; j<n; j++)
			{
				if(key<A[j]) //move A[j] forward, key keep comparing next numbers.
				{
					A[j-1] = A[j];
					A[j] = key;
				}
				else break;	//don't move A[j], key keep comparing next numbers.
			}
		}
}

//an important function in Mg_sort.
void Merge(int B[], int p, int q, int r)
{
	//divide array to two parts.
	int n1 = q-p+1;
	int n2 = r-q;
	double L[n1+1], R[n2+1];
	for(int i=0; i<n1; i++)
		L[i] = B[p+i];
	for(int j=0; j<n2; j++)
		R[j] = B[q+j+1];
	
	//make the last element infinitesimal, otherwise  
	//some element may not to be sort, because no numbers can compare with it
	//when there is only one element left in arrays.
	L[n1] = -1.0/0.0;
	R[n2] = -1.0/0.0;
	
	int k=0;
	int t=0;
	for(int x=p; x< r+1 ; x++)
	{
		if(L[k] >= R[t])
		{
			B[x] = L[k];
			k++;
		}
		else
		{
			B[x] = R[t];
			t++;
		}
	}		
}

//merge sort function, to sort numbers in DESCENDING order.
void Mg_sort(int B[], int p, int r)
{
	if(p<r)
	{	
		int q = (p+r)/2;
		Mg_sort(B, p, q);
		Mg_sort(B,q+1, r);
		Merge(B, p, q, r);
	}
}

int main()
{
	int n;
	int arrMin;
	int arrMax;
	
	//the seed of random function.
	srand(time(0));
	int ctr = 0;
	int randNum;
	
	cout<<"Please input the length of array : ";
	cin>>n;
	const int arrLength = n;
	int arrA[arrLength];
	int arrB[arrLength];
	
	do{
	cout<<"Please input the lower bound of array : ";
	cin>>arrMin;
	cout<<"Please input the upper bound of array : ";
	cin>>arrMax;
	
	if(arrMax<arrMin)
	cout<<"Lower bound is greater than upper bound, please input again."<<endl;
	}while(arrMax<arrMin);
	
	while(ctr<n)	//generate n random numbers.
	{
		//make random number between upper limit and lower limit.
		randNum = (rand()%(arrMax-arrMin+1)) + arrMin;
			arrA[ctr] = randNum;
			ctr++;
	}
	
	Duplicate_array(arrA, arrB, n);
	Printout_arrary(arrA, n);
	
	cout<<endl<<"By insertion sort : "<<endl;
	Ins_sort(arrA, n);
	Printout_arrary(arrA, n);
	
	cout<<endl<<endl<<"By merge sort : "<<endl;
	Mg_sort(arrB, 0, n-1);
	Printout_arrary(arrB, n);
	
	return 0;
}


