
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

int Is_arr_desc(int A[], int n)
{
	int k=0;
	for(int i=0; i<n; i++)
	{
		k = i;
		if(A[i]<A[i+1])
			break;
	}
	if(k==n-1)
		return 1;
	else return 0;
	
}


void Min_heapify(int A[], int i, int n)	//: size of A
{
	int l = 2*i + 1;	//left child of i
	int r = 2*i + 2;	//right child of i
	int min;

	if(l<=n-1 && A[l]<A[i]) 
		min = l;
	else min = i;

	if(r<=n-1 && A[r]<A[min])
		min = r;

	if(min!=i) 
	{
		int a = A[i];
		A[i] = A[min];
		A[min] = a;
		Min_heapify(A, min, n);
	}
}

void Build_min_heap(int A[], int n)
{
	for(int i=(n/2)-1; i>=0; i--)
		Min_heapify(A, i, n);
}

//Heap sort function, to sort numbers in DESCENDING order.
void HS(int A[], int n)
{
	Build_min_heap(A,n);
	for(int j=n-1; j>0; j--)
	{
		int b = A[0];
		A[0] = A[j];
		A[j] = b;
		Min_heapify(A, 0, j);
	}
}

int Partition(int A[],int p ,int r)
{
	int x = A[r-1];
	int i = p-1;
	for(int j = p; j<r-1; j++)
	{
		if(A[j] >= x)
		{
			i++;
			int b = A[i];
			A[i] = A[j];
			A[j] = b;
		}
	}
	int c = A[i+1];
	A[i+1] = A[r-1];
	A[r-1] = c;
	return i+1;
}

void QS(int A[], int p, int r)
{
	int q;
	if(p<r-1)
	{
		q = Partition(A, p, r);
		QS(A, p, q);
		QS(A, q+1, r);	
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
	int *arrA = new int[n];
	int *arrB = new int[n];
	
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
	
	cout<<endl<<"By heap sort : "<<endl;
	HS(arrA, n);
	Printout_arrary(arrA, n);
	cout<<endl<<"If array is descending, print 1 ; if not, print 0 : ";
	cout<<Is_arr_desc(arrA, n);
	
	cout<<endl<<endl<<"By quick sort : "<<endl;
	QS(arrB, 0, n);
	Printout_arrary(arrB, n);
	cout<<endl<<"If array is descending, print 1 ; if not, print 0 : ";
	cout<<Is_arr_desc(arrB, n);
	
	delete [] arrA;
	delete [] arrB;
	return 0;
}

