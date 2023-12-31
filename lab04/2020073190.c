#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

#define FROMPARENT 0
#define FROMTHREAD 1

typedef struct ThreadedTree* ThreadedPtr;
typedef int ElementType;

struct ThreadedTree {
	int left_thread; // flag if ptr is thread, 1, if it is a thread or 0, if it points to a child node
	ThreadedPtr left_child;
	ElementType data;
	ThreadedPtr right_child;
	int right_thread; // flag if ptr is thread, 1, if it is a thread or 0, if it points to a child node
}ThreadedTree;

/*
Create a root of new Threaded Tree
Root data should be -1
return:
	the pointer of new Threaded Tree
*/
ThreadedPtr CreateTree()
{
	ThreadedPtr rootPtr = (ThreadedPtr) malloc(sizeof(struct ThreadedTree));
	rootPtr->data = -1;
	return rootPtr;
}


/*
Insert the key value "data" in Threaded Tree
root : Pointer to the root node of Threaded Tree
root_idx : Index of the root node of Threaded Tree, means the first node to find the insertion position
data : data key value of the node to be newly inserted,
idx : The index of the node to be newly inserted, 0 means root index
return :
	1, the insertion succeeds
	0, the insertion fails.
*/
int Insert(ThreadedPtr root, int root_idx, ElementType data, int idx)
{
	// insertion fail case?
	if (root == NULL || root_idx < 0 || idx < 0) return 0;
	else if (idx == 0) root->data = data;

	ThreadedPtr newNodePtr = (ThreadedPtr) malloc(sizeof(ThreadedTree));
	ThreadedPtr travPtr = root;
	while (1)
	{
		if (travPtr->left_thread == FROMTHREAD && travPtr->right_thread == FROMTHREAD)
		{
			
		}
		else if (travPtr->left_thread == FROMPARENT && travPtr->right_thread == FROMPARENT)
		{

		}
	}

	return 1;
}


/*
In Order Traversal
Implementing in a recursive form is prohibited.
When printing out, the interval between each key is one space
print out:
	"inorder traversal : data1 data2 data3" (When there are 3 data, except root node)

*/
void printInorder(ThreadedPtr root)
{
	// go to the leftmost node and print out
	ThreadedPtr travPtr = root;
	while (travPtr->left_child != NULL) travPtr = travPtr->left_child;
	printf("inorder traversal :");
	while (travPtr != NULL)
	{
		printf(" %d", travPtr->data);
		travPtr = travPtr->right_child;
	}
}

/*
Delete the Tree
*/
void DeleteTree(ThreadedPtr root)
{
	// go to the leftmost node and delete all
	ThreadedPtr travPtr = root; ThreadedPtr tempPtr = root;
	while (travPtr->left_child != NULL) travPtr = travPtr->left_child;
	while (tempPtr != NULL)
	{
		tempPtr = travPtr->right_child;
		free(travPtr);
		travPtr = tempPtr;
	}		
}

int main(int argc, char *argv[]){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");

	ThreadedPtr root = CreateTree();

	int NUM_NODES;
	fscanf(fin, "%d", &NUM_NODES);

	int root_idx=0, idx=0;

	while(++idx <= NUM_NODES){
		ElementType data;
		fscanf(fin, "%d", &data);
		
		if(Insert(root, root_idx, data, idx) == 0){
			fprintf(fout, "Insertion failed!\n");
			return 0;
		}
	}

	printInorder(root);
	DeleteTree(root);
	
	
	fclose(fin);
	fclose(fout);

	return 0;
}
