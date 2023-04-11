#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BST* Tree;
typedef struct BST{
	int value;
	struct BST* left;
	struct BST* right;
}BST;

Tree insertNode(Tree root, int key);
Tree deleteNode(Tree root, int key);
int findNode(Tree root, int key);
void printInorder(FILE* file, Tree root);
void deleteTree(Tree root);
Tree findMin(Tree subroot);

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char cv;
	int key;

	Tree root = NULL;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d", &key);
				if(!findNode(root, key))
				{
					root = insertNode(root, key);
					fprintf(fout, "insert %d\n", key);
				}
				else
					fprintf(fout, "insertion error: %d is already in the tree\n", key);
				break;
			case 'f':
				fscanf(fin,"%d",&key);
				if(findNode(root, key))
					fprintf(fout, "%d is in the tree\n", key);
				else
					fprintf(fout, "finding error: %d is not in the tree\n", key);
				break;
			case 'd':
				fscanf(fin, "%d", &key);
				if(findNode(root, key)){
					root = deleteNode(root, key);
					fprintf(fout, "delete %d\n", key);
				}
				else{
					fprintf(fout, "deletion error: %d is not in the tree\n", key);
				}
				break;
			case 'p':
				fscanf(fin, "%c", &cv);
				if(cv == 'i'){
					if(root == NULL)
						fprintf(fout, "tree is empty");
					else
						printInorder(fout, root);
				}
				fprintf(fout, "\n");
				break;
		}
	}
	deleteTree(root);
}
/*
Insert the value "key" in Tree "root"
Tree root : A pointer to the root node of the binary search tree.
int key : The value of the new node to insert.
return:
	pointer of root
print out:
	"insert key\n"
*/
Tree insertNode(Tree root, int key)
{
	Tree newNode = (Tree) malloc(sizeof(struct BST)); newNode->value = key;
	if (root == NULL) // insert immediately if tree is null
	{
		root = newNode;
	}else
	{
		Tree tmpPtr = root;
		while (1) // beware of infinite loop...
		{
			if (tmpPtr->value > key && tmpPtr->left == NULL) 
			{
				tmpPtr->left = newNode;
				break;
			}
			else if (tmpPtr->value > key) tmpPtr = tmpPtr->left;
			else if (tmpPtr->value < key && tmpPtr->right == NULL) 
			{
				tmpPtr->right = newNode;
				break;
			}
			else tmpPtr = tmpPtr->right;
		}
	}
	return root;
}
/*
Delete the value key in Tree root
Tree root : A pointer to the root node of the binary search tree.
int key : The value of the new node to be deleted.
return :
	pointer of root
*/
Tree deleteNode(Tree root, int key)
{
	// just return root if tree is null
	if (root == NULL) return root;
	Tree tmpPtr = root;
	// make tmpPtr that is the position of the node deleted
	while (tmpPtr->value != key)
	{
		if (tmpPtr->value > key) tmpPtr = tmpPtr->left;
		else tmpPtr = tmpPtr->right;
	}
	// make delPtr to know the position of the node deleted
	Tree delPtr = root;
	while (delPtr->left != tmpPtr && delPtr->right != tmpPtr)
	{
		if (delPtr->value > key) delPtr = delPtr->left;
		else delPtr = delPtr->right;
	}
	// determine whether it is leaf or not, has child or not
	if (tmpPtr->left == NULL)
	{
		if (tmpPtr->right == NULL)	// no child
		{
			if (delPtr->left == tmpPtr)
			{
				delPtr->left = NULL;
				free(delPtr->left);
			}else 
			{
				delPtr->right = NULL;
				free(delPtr->right);
			}
		}else	// left NULL, right child
		{
			Tree grandchild = tmpPtr->right;
			if (delPtr->left == tmpPtr)
			{
				delPtr->left = NULL;
				free(delPtr->left);
				delPtr->left = grandchild;
			}else
			{
				delPtr->right = NULL;
				free(delPtr->right);
				delPtr->right = grandchild;
			}
		}
	}else
	{
		if (tmpPtr->right == NULL)	// left child, right NULL
		{
			Tree grandchild = tmpPtr->left;
			if (delPtr->left == tmpPtr)
			{
				delPtr->left = NULL;
				free(delPtr->left);
				delPtr->left = grandchild;
			}else
			{
				delPtr->right = NULL;
				free(delPtr->right);
				delPtr->right = grandchild;
			}	
		}else	// has both child
		{
			Tree replaced = findMin(tmpPtr->right);
			tmpPtr->value = replaced->value;
			deleteNode(root, replaced->value);
		}
	}
	return root;
}
/*1
Find the value key in Tree root
Tree root : A pointer to the root node of the binary search tree.
int key : The value of the node trying to find.
return :
	1, key is found
	0, key is not found
*/
int findNode(Tree root, int key)
{
	// return 0 if tree is null
	if (root == NULL) return 0;
	// compare target value with left, right values to find certain node
	Tree tmpPtr = root;
	while (tmpPtr->value != key)
	{
		if (tmpPtr->value > key && tmpPtr->left == NULL) return 0;
		else if (tmpPtr->value > key) tmpPtr = tmpPtr->left;
		else if (tmpPtr->value < key && tmpPtr->right == NULL) return 0;
		else tmpPtr = tmpPtr->right;
	}
	return 1;
}
/*
In Order Traversal
Tree root : A pointer to the root node of the binary search tree.
print out:
	"root->value "
*/
void printInorder(FILE* file, Tree root)
{
	if (root == NULL) return;
	printInorder(file, root->left);
	fprintf(file, "%d ", root->value);
	printInorder(file, root->right);
}
/*
Delete Tree
free allocated memory
Tree root : A pointer to the root node of the binary search tree.
*/
void deleteTree(Tree root)
{
	if (root == NULL) return;
	else if (root->left == NULL && root->right == NULL) free(root);
	else
	{
		deleteTree(root->left);
		deleteTree(root->right);
		free(root);
	}
}
/*
find minimum node in the tree
*/
Tree findMin(Tree subroot)
{
	if (subroot == NULL) return NULL;
	else if (subroot->left == NULL) return subroot;
	else
	{
		return findMin(subroot->left);
	}
}