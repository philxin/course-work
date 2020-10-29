/*
This code can be compiled and run ok.

purpose: This code is  to find the minimum, second minimum, maximum, second maximum, 
		 sum and product of input numbers.

usage: Input the number of how many numbers you want to sort out, then input every number, and
	   it will output the result. 
	   
compile: g++ findMaxMinNumber.cpp

pseudocode:
	findMax
	-------------------------
	key1 <- numbers[0];
	compare numbers[1] to numbers[n-1] with key1
	if numbers[i] > key1
		key1 <- nummbers[i]
	
	record i
	ctr1 <- i
	-------------------------
	
	findMin
	-------------------------
	key2 <- numbers[0]
	compare numbers[1] to numbers[n-1] with key2
	if numbers[i] < key2
		key2 <- nummbers[i]
		
	record i
	ctr2 <- i
	-------------------------

	find2nd_Max
	-------------------------
	as findMax
	but skip the maximum( numbers[ctr1] )
	-------------------------
	
	find2nd_Min
	-------------------------
	as findMin
	but skip the minimum( numbers[ctr2] )
	-------------------------
	
	coded by Hsin Chen, ID: H34021204, email: chenshin0606@gmail.com
	2014/09/27
*/ 
#include <iostream>
using namespace std;

int main()
{
	int n;		//the number of numbers going to be sorted. 
	
	//function to deal with the problems that input number is not larger than 4.  
	do{			
	
	cout<<"Please input a nonnegative integer number that is larger than 4 : ";
	cin>>n;
	
	if (n<=4)
	cout<<"!!Warning!! "<<n<<" is NOT larger than 4! "<<endl;
	
	}while(n<=4);
	
	int *numbers = new int[n];			// dynamically allocate memory for n.
	cout<<"Please input "<<n<<" integers : "<<endl;
	
	for(int i=0; i<n; i++)				//input every number.
	cin>>numbers[i];
	
	cout<<"Among the "<<n<<" integers : ";		//print all numbers.
	for(int i=0; i<n; i++)
	cout<<numbers[i]<<"  ";
	
	cout<<endl;
	
	int a = numbers[0];			//assign the first element of array to a(key 1) and b(key 2) 
	int b = numbers[0];			//because sorting will be from numbers[0] to numbers[n].
	
	int ctr1 = 0;				//counter to record the biggest element.
	int ctr2 = 0;				//counter to record the smallest element.
	
	for(int i=1; i<n; i++)		//to find the biggest number.
	{
		if( numbers[i] > a )
		{	
		a = numbers[i];
		ctr1 = i;				//record the biggest number.
		};
	};
	
	for(int i=1; i<n; i++)		//to find the smallest number.
	{
		if( numbers[i] < b )
		{
		b = numbers[i];
		ctr2 = i;				//record the smallest number.
		};
	};
	
	int max = a;				//now a is the biggest number, b is the smallest number,
	int min = b;				//assign a to max, and b to min. 
	
	a = b = numbers[0];			//initialize a and b again.
	
	//to find the second minimum, and second maximum.
	if(ctr1 == 0)				//prevent the condtion that numbers[0] is the biggest.
	a = numbers[1];
	
	if(ctr2 == 0)				//prevent the condtion that numbers[0] is the smallest.
	b = numbers[1];
	
	for(int i=0; i<n; i++)
	{
		if(i != ctr1)			//if numbers[i] is the biggest, skip it.
		{
			if( numbers[i] > a )
			a = numbers[i];
		};
	};
	
	for(int i=0; i<n; i++)
	{
		if(i != ctr2)			//if numbers[i] is the smallest, skip it.
		{
			if( numbers[i] < b )
			b = numbers[i];
		};
	};
	
	int max2 = a;				//assign value.
	int min2 = b;
	
	int sum = 0;				//count numbers' sum.
	for(int i=0; i<n; i++)
	{
		sum += numbers[i];
	};
	
	int product = 1;			//count numbers' product.
	for(int i=0; i<n; i++)
	{
		product = numbers[i] * product;
	};
	
	//print the result.
	cout<<"min = "<<min<<" ; 2nd_min = "<<min2<<" ; max = "<<max<<" ; 2nd_max = "<<max2
		<<" ;"<<endl<<"sum = "<<sum<<" ; product = "<<product<<endl;
		
	delete [] numbers;			//return memory space.
	return 0;
}
