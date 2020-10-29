#include <iostream>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <vector>
using namespace std;

struct bucketList{
    bucketList(int i=0):data(i),next(NULL){
    }
	bucketList* next;
	int data;
};

void printout(int [], int);
int findMax(int [], int);
bucketList* BsMerge(bucketList *,bucketList *);
bucketList* BsInsert(bucketList* ,int);
void BucketSort(int [], int, int);

int main()
{
    int n, a_min, a_max, k;
    cout << "Please input [a_min, a_max, n]:";
    cin >> a_min >> a_max >> n;
    srand (time(NULL));

    int* A = new int[n+1];
    for(int j=1; j<=n; j++)
    {
        A[j] = rand() % (a_max - a_min +1) + a_min; //make sure the number is between a-min and a_max
    }

    k = findMax(A, n);
    BucketSort(A , n, k);
    cout << "bucket Sort OK ";

return 0;
}

void printout(int A[], int n)
{
    for(int i=1; i<=n; i++)
    {
        cout << A[i] << "  ";
    }
    cout << endl;
}

int findMax(int A[], int n)
{
    int k=A[1];
    for(int i=1; i<=n; i++)
    {
        if(A[i]>k)
        {
            k = A[i];
        }
    }
    return k;
}

bucketList* BsInsert(bucketList* head,int value)
{
	bucketList dummyNode;
	bucketList *newNode = new bucketList(value);
	bucketList *pre,*current;
	dummyNode.next = head;
	pre = &dummyNode;
	current = head;
	while(current != NULL && current->data >= value)
	{
		pre = current;
		current = current->next;
	}
	newNode->next = current;
	pre->next = newNode;
	return dummyNode.next;
}

bucketList* BsMerge(bucketList *head1,bucketList *head2){
	bucketList dummyNode;
	bucketList *dummy = &dummyNode;
	while(head1!=NULL && head2!=NULL)
	{
		if(head1->data >= head2->data)
		{
			dummy->next = head1;
			head1 = head1->next;
		}
		else
		{
			dummy->next = head2;
			head2 = head2->next;
		}
		dummy = dummy->next;
	}
	if(head1!=NULL)
        dummy->next = head1;
	if(head2!=NULL)
	    dummy->next = head2;

	return dummyNode.next;
}

void BucketSort(int A[],int n, int k)
{
    int x=1, y=1;
    while(x != 0)
    {
        x = k/pow(10, y);
        y++;
    }
    int bucketSum = pow(10,y-2);
	vector<bucketList*> buckets(bucketSum,(bucketList*)(0));
	for(int i=1; i<=n; i++)
	{
		int index = A[i]/bucketSum;
		bucketList *head = buckets.at(index);
		buckets.at(index) = BsInsert(head,A[i]);
	}
	bucketList *head = buckets.at(0);
	for(int i=1; i<bucketSum; i++)
	{
		head = BsMerge(head,buckets.at(i));
	}
	for(int i=1; i<=n; i++)
	{
		A[i] = head->data;
		head = head->next;
	}
}

