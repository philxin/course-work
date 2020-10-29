/*
This code can be compiled and run ok.

Purpose: This code is to randomly generate some numbers, copy these numbers, and sort numbers
		by counting sort, radix sort and bucket sort.
Usage: Input the number of how many numbers you want to sort out, then give the random numbers'
		upper limit and lower limit, and it will output the result. 
Compile: g++ countingSort_radixSort_BucketSort.cpp

Pseudocode: 

CS(A[], n, arrMin, arrMax)
------------------------------
use two limits to count the range.
create array Ctr[range] and B[n];
	for i = 0 ~ range-1
		Ctr[i]=0;
	for j = 0 ~ n-1
		Ctr[A[j]-arrMin]++;
	for i = range-2 ~ 0
		Ctr[i] +=  Ctr[i+1];
	for j = n-1 ~ 0
	{	
		B[Ctr[A[j]-arrMin]-1] = A[j];
		Ctr[A[j]-arrMin]--;
	}
------------------------------

RS((A[], n)
------------------------------
first get the largest digit among numbers.
And sort numbers by digit.
(start from the smallest digit)
------------------------------

BS(A[], n)
------------------------------
find the maximum, make all numbers divide maximum.
put numbers into buckets, connect them with link listed.
sort each bucket, and multiply maximum then copy to array in order.
------------------------------



*/

#include <iostream>
#include <cstdlib>
#include <time.h>
using namespace std;

struct link
{
	float value;
	link* next;
};

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


//counting sort 
void CS(int A[],int n, int arrMin, int arrMax)
{
	int k = arrMax - arrMin + 1;
	int *B = new int [n];
	int *Ctr = new int [k];
	for(int i=0; i<k; i++)
		Ctr[i]=0;
	for(int j=0; j<n; j++)
		Ctr[A[j]-arrMin]++;
	for(int i=k-2; i>=0; i--)
		Ctr[i] +=  Ctr[i+1];
	for(int j=n-1; j>=0; j--)
	{	
		B[Ctr[A[j]-arrMin]-1] = A[j];
		Ctr[A[j]-arrMin]--;
	}
	Duplicate_array(B, A, n);
	delete [] B;
	delete [] Ctr;
}

//radix sort
void RS(int A[],int n)
{
	int ds= 1;
	int g = 10;
	for(int i=0; i<n; i++)	//get the biggest digit.
	{
		while(A[i]>=g)
		{
			g *= 10;
			ds++;
		}
	}
	
	int *B = new int[n];
	int *Ctr = new int [10];
	int digit = 1;
	int p;
	for(int t=0; t<ds; t++)
	{
		for(int i=0; i<10; i++)
			Ctr[i]=0;
		for(int j=0; j<n; j++)
		{
			p = (A[j] / digit) % 10;
			Ctr[p]++;
		}
		for(int i=8; i>=0; i--)
			Ctr[i] +=  Ctr[i+1];
		for(int j=n-1; j>=0; j--)
		{	
			p = (A[j] / digit) % 10;
			B[Ctr[p]-1] = A[j];
			Ctr[p]--;
		}
		Duplicate_array(B, A, n);
		digit *= 10;
	}
	delete [] B;
	delete [] Ctr;
}

void BS(int A[], int n)
{
	link *F = new link [n];
	link *bucket = new link [11];
	for(int i=0; i<11; i++)
		bucket[i].next = NULL;
	int max = A[0];
	for(int i=1; i<n; i++)		//to find the biggest number.
	{
		if( A[i] > max )	
			max = A[i];
	};
	for(int i=0; i<n; i++)
		F[i].value = A[i]*1.0/max;
	
	for(int i=0; i<n; i++)		//put elements into buckets.
	{
		if(F[i].value<0.0)
		{
			if(bucket[0].next == NULL)
			{	
				bucket[0].next = &F[i];
				bucket[0].next->value = F[i].value;
				bucket[0].next->next = NULL;
			}
				else 
				{
					link *key = bucket[0].next;
					while(key->next != NULL)
						key = key->next;
						key->next = &F[i];
						key->next->value = F[i].value;
						key->next->next = NULL;
				}
		}
		else if(F[i].value>=0.0 && F[i].value<0.1)
		{
			if(bucket[1].next == NULL)
			{	
				bucket[1].next = &F[i];
				bucket[1].next->value = F[i].value;
				bucket[1].next->next = NULL;
			}
				else 
				{
					link *key = bucket[1].next;
					while(key->next != NULL)
						key = key->next;
						key->next = &F[i];
						key->next->value = F[i].value;
						key->next->next = NULL;
				}
		}
		else if(F[i].value>=0.1 && F[i].value<0.2)
		{
			if(bucket[2].next == NULL)
			{	
				bucket[2].next = &F[i];
				bucket[2].next->value = F[i].value;
				bucket[2].next->next = NULL;
			}
				else 
				{
					link *key = bucket[2].next;
					while(key->next != NULL)
						key = key->next;
						key->next = &F[i];
						key->next->value = F[i].value;
						key->next->next = NULL;
				}
		}
		else if(F[i].value>=0.2 && F[i].value<0.3)
		{
			if(bucket[3].next == NULL)
			{	
				bucket[3].next = &F[i];
				bucket[3].next->value = F[i].value;
				bucket[3].next->next = NULL;
			}
				else 
				{
					link *key = bucket[3].next;
					while(key->next != NULL)
						key = key->next;
						key->next = &F[i];
						key->next->value = F[i].value;
						key->next->next = NULL;
				}
		}
		else if(F[i].value>=0.3 && F[i].value<0.4)
		{
			if(bucket[4].next == NULL)
			{	
				bucket[4].next = &F[i];
				bucket[4].next->value = F[i].value;
				bucket[4].next->next = NULL;
			}
				else 
				{
					link *key = bucket[4].next;
					while(key->next != NULL)
						key = key->next;
						key->next = &F[i];
						key->next->value = F[i].value;
						key->next->next = NULL;
				}
		}
		else if(F[i].value>=0.4 && F[i].value<0.5)
		{
			if(bucket[5].next == NULL)
			{	
				bucket[5].next = &F[i];
				bucket[5].next->value = F[i].value;
				bucket[5].next->next = NULL;
			}
				else 
				{
					link *key = bucket[5].next;
					while(key->next != NULL)
						key = key->next;
						key->next = &F[i];
						key->next->value = F[i].value;
						key->next->next = NULL;
				}
		}
		else if(F[i].value>=0.5 && F[i].value<0.6)
		{
			if(bucket[6].next == NULL)
			{	
				bucket[6].next = &F[i];
				bucket[6].next->value = F[i].value;
				bucket[6].next->next = NULL;
			}
				else 
				{
					link *key = bucket[6].next;
					while(key->next != NULL)
						key = key->next;
						key->next = &F[i];
						key->next->value = F[i].value;
						key->next->next = NULL;
				}
		}
		else if(F[i].value>=0.6 && F[i].value<0.7)
		{
			if(bucket[7].next == NULL)
			{	
				bucket[7].next = &F[i];
				bucket[7].next->value = F[i].value;
				bucket[7].next->next = NULL;
			}
				else 
				{
					link *key = bucket[7].next;
					while(key->next != NULL)
						key = key->next;
						key->next = &F[i];
						key->next->value = F[i].value;
						key->next->next = NULL;
				}
		}
		else if(F[i].value>=0.7 && F[i].value<0.8)
		{
			if(bucket[8].next == NULL)
			{	
				bucket[8].next = &F[i];
				bucket[8].next->value = F[i].value;
				bucket[8].next->next = NULL;
			}
				else 
				{
					link *key = bucket[8].next;
					while(key->next != NULL)
						key = key->next;
						key->next = &F[i];
						key->next->value = F[i].value;
						key->next->next = NULL;
				}
		}
		else if(F[i].value>=0.8 && F[i].value<0.9)
		{
			if(bucket[9].next == NULL)
			{	
				bucket[9].next = &F[i];
				bucket[9].next->value = F[i].value;
				bucket[9].next->next = NULL;
			}
				else 
				{
					link *key = bucket[9].next;
					while(key->next != NULL)
						key = key->next;
						key->next = &F[i];
						key->next->value = F[i].value;
						key->next->next = NULL;
				}
		}
		else if(F[i].value>=0.9 && F[i].value<=1.0)
		{
			if(bucket[10].next == NULL)
			{	
				bucket[10].next = &F[i];
				bucket[10].next->value = F[i].value;
				bucket[10].next->next = NULL;
			}
				else 
				{
					link *key = bucket[10].next;
					while(key->next != NULL)
						key = key->next;
						key->next = &F[i];
						key->next->value = F[i].value;
						key->next->next = NULL;
				}
		}
	} 
	
	for(int t=0; t<11; t++)
	{
		int ctr = 0;
		if(bucket[t].next != NULL)
		{
		link *k = bucket[t].next;		
		while(k != NULL)			//count numbers of element in each bucket.
			{
				k = k->next;
				ctr++;
			}
		link *key = bucket[t].next;
		link *index = bucket[t].next;
		key->value = bucket[t].next->value;
		float min = key->value;
		
		for(int j=0; j<ctr; j++)
		{
			link *key = bucket[t].next;
			link *pre_index = NULL;
			link *tmp;
			float min = key->value;
			if(j==0)
			{
				while(key->next != NULL)		//to find the smallest number.
				{
					if( key->next->value < min )
					{
						pre_index = key;
						min = key->next->value;	
					}
					key = key->next;
				}
			}
			else 
			{
				while(key->next->value < key->value && key->next !=NULL)
					key = key->next;
					pre_index = key;
				if(key->next !=NULL)
					key = key->next;
					min = key->value;
				while(key->next != NULL)		//to find the smallest number.
				{
					if( key->next->value < min )
					{
						pre_index = key;
						min = key->next->value;	
					}
					key = key->next;
				}
			}
			if(pre_index != NULL)		//use minimum to sort in each bucket.
			{
				tmp = pre_index->next;
				if(pre_index->next->next == NULL)
				{
					pre_index->next = NULL;
					tmp->next = bucket[t].next;
					bucket[t].next = tmp;
				}
				else
				{
					pre_index->next = tmp->next;
					tmp->next = bucket[t].next;
					bucket[t].next = tmp;
				}
			}
		}
	}

}
	int count=0;
	for(int t=10; t>=0; t--)		//copy result to the original array.
	{
		link *x = bucket[t].next;
		while(x!=NULL)
		{
			A[count] = x->value * max;
			count++;
			x = x->next;
		}
	}
	delete [] F;
	delete [] bucket;
} 






int main()
{
	int n;
	int arrMin;
	int arrMax;
	int *arrA;
	int *arrB;
	
	//the seed of random function.
	srand(time(0));
	int ctr = 0;
	int randNum;
	
	cout<<"Please input the length of array : ";
	cin>>n;
	const int arrLength = n;
	arrA = new int [n];
	arrB = new int [n];
	
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
	
	CS(arrA, n, arrMin, arrMax);
	cout<<endl<<"By Counting Sort : ";
	Printout_arrary(arrA, n);
	
	Duplicate_array(arrB, arrA, n);
	RS(arrA, n);
	cout<<endl<<"By Radix Sort : ";
	Printout_arrary(arrA, n);
	
	Duplicate_array(arrB, arrA, n);
	BS(arrA, n);
	cout<<endl<<"By Bucket Sort : ";
	Printout_arrary(arrA, n);
	
	delete [] arrA;
	delete [] arrB;
	return 0;
}
