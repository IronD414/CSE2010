#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct Node {
	int vertex;	//# of node
	int dist;	//distance from primary node. (it will be updated constantly)
	int prev;	//to find final path
}Node;

typedef struct Graph {
	int size;
	int** vertices;
	Node* nodes;
}Graph;

typedef struct Heap {
	int Capacity;
	int Size;
	Node* Element;
}Heap;

Graph* createGraph(int size);
void deleteGraph(Graph* g);
void dijkstra(Graph* g);
int* shortestPath(Graph* g, int dest);
Heap* createMinHeap(int heapSize);
void deleteMinHeap(Heap* minHeap);
void insertToMinHeap(Heap* minHeap, int vertex, int distance);
Node deleteMin(Heap* minHeap);

void swap(Node* node1, Node* node2) {
	Node temp = *node1;
	*node1 = *node2;
	*node2 = temp;
}

void main(int argc, char *argv[]) {
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	Graph* g;
	int size;
	fscanf(fin, "%d\n", &size);
	g = createGraph(size + 1);

	char tmp = 0;
	while (tmp != '\n' && tmp != EOF) {
		int node1, node2, weight;
		fscanf(fin, "%d-%d-%d", &node1, &node2, &weight);
		g->vertices[node1][node2] = weight;
		tmp = fgetc(fin);
	}

	dijkstra(g);

	int j;
	for(int i = 2; i < g->size; i++){
		int* path = shortestPath(g, i);
		if(path == NULL){
			fprintf(fout, "can not reach to node %d\n", i);
			continue;
		}
		for (j = g->size - 1; j > 0; j--){
			if(path[j] == 0) continue;
			fprintf(fout, "%d->", path[j]);
		} 
		fprintf(fout, "%d (cost : %d)\n", i, g->nodes[i].dist);
		free(path);
	}
	deleteGraph(g);
}
/*
Allocate Graph Matrix

Initial distance: INF except 1st node (source)
Initial prev: -1
*/
Graph* createGraph(int size) 
{
	int i, j;
	Graph* newGraph = (Graph*) malloc(sizeof(Graph));
	newGraph->size = size;
	newGraph->vertices = (int**) malloc(sizeof(int*) * size);
	for (i = 0; i < size; ++i)
	{
		newGraph->vertices[i] = (int*) malloc(sizeof(int) * size);
		for (j = 0; j < size; ++j)
			newGraph->vertices[i][j] = 0;
	}
	newGraph->nodes = (Node*) malloc(sizeof(Node) * size);
	for (i = 0; i < size; ++i)
	{
		newGraph->nodes[i].vertex = i;
		newGraph->nodes[i].dist = INF;
		newGraph->nodes[i].prev = -1;
	}	//	nodes[0] is trash node, it won't be used, and origin node is nodes[1]
	newGraph->nodes[1].dist = 0;

	return newGraph;
}
// deallocate graph
void deleteGraph(Graph* g)
{
	int i;
	for (i = 0; i < g->size; ++i)
		free(g->vertices[i]);
	free(g->vertices);
	free(g->nodes);
	free(g);
}
/*
Allocate min heap
*/ 
Heap* createMinHeap(int heapSize) 
{
	int i;
	Heap* newHeap = (Heap*) malloc(sizeof(Heap));
	newHeap->Capacity = heapSize;
	newHeap->Size = 0;
	newHeap->Element = (Node*) malloc(sizeof(Node) * heapSize);
	newHeap->Element[0].vertex = -1;	//	trash value in heap
	newHeap->Element[0].dist = -1;	//	trash value
	for (i = 0; i < heapSize; ++i)
		newHeap->Element[i].dist = INF;
	return newHeap;
}
/*
Deallocate min heap
*/
void deleteMinHeap(Heap* minHeap)
{
	free(minHeap->Element);
	free(minHeap);
}
/*
Dijkstra Algorithm
*/
void dijkstra(Graph* g)
{
	//
	int i; Node target;
	Heap* heap = createMinHeap(g->size);
	int* status = (int*) malloc(sizeof(int) * g->size);	//	0: temporary, 1: permanent
	for (i = 1; i < g->size; ++i)
		status[i] = 0;

	insertToMinHeap(heap, g->nodes[1].vertex, g->nodes[1].dist);
	
	while (heap->Size > 0)
	{
		target = deleteMin(heap);
		if (target.dist < g->nodes[target.vertex].dist) 
			g->nodes[target.vertex].dist = target.dist;
		if (status[target.vertex]) continue;
		status[target.vertex] = 1;
		for (i = 1; i < g->size; ++i)
		{
			if (g->vertices[target.vertex][i] > 0)
			{
				if (!status[i] && g->nodes[target.vertex].dist + g->vertices[target.vertex][i] < g->nodes[i].dist)
				{
					//fprintf(fout, "updated node %d's distance from %d to %d\n", i, g->nodes[i].dist, g->nodes[target.vertex].dist + g->vertices[target.vertex][i]);
					g->nodes[i].dist = g->nodes[target.vertex].dist + g->vertices[target.vertex][i];	//	update distance
					//fprintf(fout, "updated node %d's prev from %d to %d\n", i, g->nodes[i].prev, target.vertex);
					g->nodes[i].prev = target.vertex;	//	update prev
					insertToMinHeap(heap, g->nodes[i].vertex, g->nodes[i].dist);	//	insert to or update heap

					// decrease priority?
				}
			}
		}
	}
}
/*
Return shortest path from source(1) to dest(i)
Return type is array and the size is the number of node.
You should save the path reversely. 
For example, If shortest path is 1->2->3 
and the number of node(g->size) is 5,
you must return [3, 2, 1 , 0, 0]
If there is no path, return NULL 
*/
int* shortestPath(Graph* g, int dest)
{
	if (g->nodes[dest].prev == -1) return NULL;	//	no path

	int i, x = dest;
	int* result = (int*) malloc(sizeof(int) * g->size);
	for (i = 0; i < g->size; ++i)
		result[i] = 0;
	for (i = 0; x != 1; ++i)	//	following prev
	{
		result[i] = x;
		x = g->nodes[x].prev;
	}
	result[++i] = 1;
	return result;
}
/*
Insert Node with vertex and distance into minHeap
*/
void insertToMinHeap(Heap* minHeap, int vertex, int distance) 
{
	int i;

	if (minHeap->Size <= 0)
	{
		minHeap->Element[1].vertex = vertex;
		minHeap->Element[1].dist = distance;
		minHeap->Size++;
		return;
	}
/*
	for (i = 1; i <= minHeap->Size; ++i)
	{
		if (minHeap->Element[i].vertex == vertex)
		{
			if (minHeap->Element[i].dist > distance) minHeap->Element[i].dist = distance;
			else return;
		}
	}
*/
	minHeap->Element[++(minHeap->Size)].vertex = vertex;
	minHeap->Element[minHeap->Size].dist = distance;
	int loc = minHeap->Size;

	while (minHeap->Element[1].dist != distance && minHeap->Element[loc / 2].dist > distance)	//	switch until reach top or heap is balanced
	{
		swap(&minHeap->Element[loc], &minHeap->Element[loc / 2]);
		loc /= 2;
	}
	return;
}
/*
pop the Node with minimum distance from minHeap
return:
	Node with minimum distance
*/
Node deleteMin(Heap *minHeap) 
{
	Node pop = minHeap->Element[1];
	Node last = minHeap->Element[minHeap->Size];
	int i, child;

	if (minHeap->Size == 1)
	{
		minHeap->Size--;
		return pop;
	}

	for (i = 1; i*2 <= minHeap->Size; i = child)
	{
		child = i * 2;
		if (child < minHeap->Size && minHeap->Element[child + 1].dist < minHeap->Element[child].dist)
		{
			child++;
		}
		if (last.dist > minHeap->Element[child].dist)
		{
			minHeap->Element[i] = minHeap->Element[child];
		}else break;
	}
	minHeap->Element[i] = last;
	minHeap->Size--;
	return pop;
}
