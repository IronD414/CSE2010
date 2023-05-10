#include<stdio.h>
#include<stdlib.h>
#define INF 1e9


FILE *fin;
FILE *fout;

typedef struct HeapStruct{
	int Capacity;
	int Size;
	int *Elements;
}Heap;

Heap* CreateHeap(int heapSize);
int Insert(Heap *heap, int value);
int Find(Heap *heap, int value);
int DeleteMax(Heap* heap);
int* GetElements(Heap* heap);
int IsFull(Heap *heap);
int IsEmpty(Heap *heap);
int Size(Heap *heap);

void swap(int* a, int* b){
	int t = *a;
	*a = *b;
	*b = t;
}

int main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	char cv;
	Heap* maxHeap;
	int heapSize, key, max_element;
	int ret;
	int* elements;
	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'n':
				fscanf(fin, "%d", &heapSize);
				maxHeap = CreateHeap(heapSize);
				break;
			case 'i':
				fscanf(fin, "%d", &key);
				ret = Insert(maxHeap, key);
				if(ret == 0) fprintf(fout, "insert %d\n", key);
				else if(ret == 1) fprintf(fout, "insert error : heap is full\n");
				else if(ret == 2) fprintf(fout, "insert error : %d is already in the heap\n", key);
				else {
					fprintf(fout, "error: unknown return type\n");
					return -1;
				} 

				break;
			case 'd':
				max_element = DeleteMax(maxHeap);
				if(max_element == 0){
					fprintf(fout, "delete error : heap is empty\n");

				}else if(max_element > 0){
					fprintf(fout, "max element : %d deleted\n", max_element);
				}
				break;
			case 'p':
				if(IsEmpty(maxHeap)){
					fprintf(fout, "print error : heap is empty\n");
				}else{
					elements = GetElements(maxHeap);
					int size = Size(maxHeap);
					for(int i = 0; elements[i] > -1 && i < size; i++){
						fprintf(fout, "%d ", elements[i]);
					}
					fprintf(fout, "\n");
					free(elements);
				}
				break;
			case 'f':
				fscanf(fin, "%d", &key);
				if(Find(maxHeap, key)) fprintf(fout, "%d is in the heap\n", key);
				else fprintf(fout, "finding error : %d is not in the heap\n", key);
				break;
		}
	}

	return 0;
}

/*
Create new heap with given heapSize
Element[0] should save INF
heapSize: positive integer
return:
	the pointer of new heap  
*/
Heap* CreateHeap(int heapSize)
{
	Heap* newHeap = (Heap*) malloc(sizeof(Heap));
	newHeap->Capacity = heapSize;
	newHeap->Size = 0;
	newHeap->Elements = (int*) malloc(sizeof(int) * heapSize);
	newHeap->Elements[0] = INF;
	return newHeap;
}

/*
Insert the value into Heap
value: positive integer
return:
	0 , success
	1 , heap is full
	2 , duplicated
*/
int Insert(Heap *heap, int value)
{
	if (IsFull(heap)) return 1;
	else if(Find(heap, value)) return 2;
	else
	{
		int xpos = (heap->Size++) + 1;
		heap->Elements[xpos] = value;
		while(heap->Elements[1] != value && heap->Elements[xpos / 2] <= value)
		{
			swap(&heap->Elements[xpos], &heap->Elements[xpos / 2]);
			xpos /= 2;
		}
		return 0;
	}
}

/*
Find the value in Heap
return:
	1, success
	0, fail  
*/
int Find(Heap* heap, int value)
{
	for (int i = 0; i < Size(heap); ++i)
	{
		if (heap->Elements[i] == value) return 1;
	}
	return 0;
}

/*
Delete the maximum value in Heap
return:
	Max value, success
	0, heap is empty  
*/
int DeleteMax(Heap* heap)
{
	if (IsEmpty(heap)) return 0;
	else
	{
		int i, child;
		int max_element, last_element;
		max_element = heap->Elements[1];
		last_element = heap->Elements[heap->Size--];

		for (i = 1; i * 2 <= heap->Size; i = child) 
		{
			child = i * 2;
			if (child < heap->Size && heap->Elements[child + 1] > heap->Elements[child]) 
			{
				child++;
			}
			if (last_element < heap->Elements[child]) 
			{
				heap->Elements[i] = heap->Elements[child];
			}else break;
		}
		heap->Elements[i] = last_element;
		return max_element;
	}
}

/*
Return an array of copied elements from the heap
The length of the returned array should be the same as the size of the heap
The order of the returned array should be the same as the order of heap->Elements  
If heap is empty, return NULL
***WARNING***
Do not just return heap->Elements
You must copy the elements to another allocated array
*/
int* GetElements(Heap* heap)
{
	int* arr = (int*) malloc(sizeof(int) * heap->Size);
	for (int i = 0; i < heap->Size; ++i)
	{
		arr[i] = heap->Elements[i + 1];
	}
	return arr;
}

/*
check heap is full
return:
	1, heap is full
	0, heap is not full
*/
int IsFull(Heap* heap)
{
	return heap->Size == heap->Capacity;
}

/*
check heap is empty
return:
	1, heap is empty
	0, heap is not empty
*/
int IsEmpty(Heap *heap)
{
	return heap->Size == 0;
}

/*
get size of heap
return:
	size of heap
*/
int Size(Heap *heap)
{
	return heap->Size;
}



