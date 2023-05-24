#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *fin;
FILE *fout;

typedef struct _Queue {
	int* key;
	int first, rear, qsize, max_queue_size;
}Queue;

typedef struct _Graph {
	int size;
	int* node;
	int** matrix;
}Graph;

int sum(int* a, int n);
Graph* CreateGraph(int* nodes, int n);
void InsertEdge(Graph* G, int a, int b);
void DeleteGraph(Graph* G);
int* Topsort(Graph* G);
Queue* MakeNewQueue(int X);
int IsEmpty(Queue* Q);
int Dequeue(Queue* Q);
void Enqueue(Queue* Q, int X);
void DeleteQueue(Queue* Q);
void countInput(int* n, char* str);
void parseInput(int* arr, char* str, int n);
int* selectionSort(int* a, int n);

int main(int argc, char* agrv[]) {
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");

	char numbers[100], inputs[150];
	fgets(numbers, 100, fin); // get nodes from input
	int n = 0;
	countInput(&n, numbers);
	int* nodes = (int*)malloc(sizeof(int)*n);
	int* sorted_arr = NULL;
	parseInput(nodes, numbers, n);

	Graph* G = CreateGraph(nodes, n);

	fgets(inputs, 100, fin); // get edges from input
	int len = strlen(inputs), i, j;
	for (i = 0; i < len; i += 4) {
		int a = inputs[i] - '0', b = inputs[i + 2] - '0';
		InsertEdge(G, a, b);
	}

	// PrintGraph(G);
	fprintf(fout, "%-3s", " ");
	for (i = 0; i < G->size; i++) fprintf(fout, "%-3d", G->node[i]);
	fprintf(fout, "\n");
	for (i = 0; i < G->size; i++) {
		fprintf(fout, "%-3d", G->node[i]);
		for (j = 0; j < G->size; j++) fprintf(fout, "%-3d", G->matrix[i][j]);
		fprintf(fout, "\n");
	}
	fprintf(fout, "\n");

	sorted_arr = Topsort(G);
	if(sorted_arr == NULL){
		fprintf(fout, "sorting error : cycle!");
	}else{
		for(i = 0; i < G->size; i ++)
			fprintf(fout, "%d ", sorted_arr[i]);
		fprintf(fout, "\n");
	}
	free(sorted_arr);
	DeleteGraph(G);

	fclose(fin);
	fclose(fout);

	return 0;
}

int sum(int* a, int n)
{
	int sum = 0, i;
	for (i = 0; i < n; ++i) sum += a[i];
	return sum;
}

/*
Create new graph with given nodes and the count of nodes.
saved nodes should be sorted.
return: 
        the pointer of new graph
 */
Graph* CreateGraph(int* nodes, int n) 
{
	Graph* newGraph = (Graph*) malloc(sizeof(Graph));
	newGraph->size = n;
	newGraph->node = selectionSort(nodes, n);
	newGraph->matrix = (int**) malloc(sizeof(int*) * n);
	int i, j;
	for (i = 0; i < n; ++i) newGraph->matrix[i] = (int*) malloc(sizeof(int) * n);
	for (i = 0; i < n; ++i)
		for (j = 0; j < n; ++j)
			newGraph->matrix[i][j] = 0;
	return newGraph;
}
/*
Insert edge from a to b in the graph.
*/
void InsertEdge(Graph* G, int a, int b) 
{
	int i, j;
	for (i = 0; i < G->size; ++i)
		for (j = 0; j < G->size; ++j)
			if (a == G->node[i] && b == G->node[j]) G->matrix[i][j] = 1; return;
}
/*
Deallocate the memory of the graph G.
*/
void DeleteGraph(Graph* G)
{
	int i;
	for (i = 0; i < G->size; ++i)
		free(G->matrix[i]);
	free(G->matrix);
	free(G->node);
	free(G);
}

/*
Topological sort
If two or more nodes have same priority,
    the node with the smaller number takes precedence.
return:
    the array of sorted
    NULL if the graph has a cycle
*/
int* Topsort(Graph* G)
{
	int n = G->size, i, j, k = 0, x;
	int* tsa = (int*) malloc(sizeof(int) * n);
	Queue* queue = MakeNewQueue(n);
	int* sum_of_indegree = (int*) malloc(sizeof(int) * n);
	int* columns = (int*) malloc(sizeof(int) * n);

	/* initialize the sum of indegrees */
	for (i = 0; i < n; ++i) sum_of_indegree[i] = 0;
	for (i = 0; i < n; ++i)
		for (j = 0; j < n; ++j)
			sum_of_indegree[i] += G->matrix[j][i];

	/* enqueue elems whose sum of indegree is 0 */
	/* dequeue 1 elem and delete its outgoing edges */
	/* note that queue's element is INDEX */
	for (i = 0; i < n; ++i)
		if (sum_of_indegree[i] == 0) Enqueue(queue, i);
	while (!IsEmpty(queue))
	{
		x = Dequeue(queue);
		tsa[k++] = G->node[x];
		for (j = 0; j < n; ++j)
			if (G->matrix[x][j] == 1)
			{
				G->matrix[x][j] = 0;
				if (--sum_of_indegree[j] == 0) Enqueue(queue, j);	
			}
	}
	DeleteQueue(queue);
	return tsa;
}

/*
Create new Queue with maximum size X
return:
        the pointer of new queue
*/
Queue* MakeNewQueue(int X)
{
	Queue* newQueue = (Queue*) malloc(sizeof(Queue));
	newQueue->key = (int*) malloc(sizeof(int) * X);
	newQueue->first = NULL;
	newQueue->rear = NULL;
	newQueue->qsize = 0;
	newQueue->max_queue_size = X;
	return newQueue;
}
/*
Check the queue either is empty or not.
return:
        1, empty
        0, non-empty
*/
int IsEmpty(Queue* Q){return Q->qsize == 0;}
/*
pop the front key in queue
return:
        the front key in queue, 0 if Q is empty
*/
int Dequeue(Queue* Q)
{
	if (IsEmpty(Q)) return 0;
	int x = Q->first, i;
	for (i = 0; i < Q->qsize - 1; ++i)
		Q->key[i] = Q->key[i + 1];
	Q->first = Q->key[0];
	Q->qsize--;
	return x;
}
/*
push the key in queue
*/
void Enqueue(Queue* Q, int X)
{
	if (IsEmpty(Q)) Q->first = X;
	Q->key[Q->qsize++] = X;
	Q->rear = X;
}

/*
Deallocate the memory of the queue
*/
void DeleteQueue(Queue* Q)
{
	free(Q->key);
	free(Q);
}

/*
increase n by # of units in str
*/
void countInput(int* n, char* str) 
{
	int len = strlen(str), i;
	for (i = 0; i < len; i++)
		if (0 <= str[i] - '0' && str[i] - '0' < 10) (*n)++;
}

/*
parse str and convert string into array of integers
*/
void parseInput(int* arr, char* str, int n) 
{
	int len = strlen(str), i;
	int cnt = 0;
	for (i = 0; i < len; i++)
		if (0 <= str[i] - '0' && str[i] - '0' < 10) arr[cnt++] = str[i] - '0';
}

/*
an ordinary selection sort
why selection sort? isn't it too slow?
because input array will be restricted to set of unit numbers...(yes I just wanted to)
*/
int* selectionSort(int* a, int n)
{
	int i, j, minIndex, tmp;
	int* sa = (int*) malloc(sizeof(int) * n);
	for (i = 0; i < n; ++i) sa[i] = a[i];
	for (i = 0; i < n; ++i)
	{
		minIndex = i;
		for (j = i; j < n; ++j) 
			if (sa[j] < sa[minIndex]) minIndex = j;
		tmp = sa[i];
		sa[i] = sa[minIndex];
		sa[minIndex] = tmp;
	}
	return sa;
}