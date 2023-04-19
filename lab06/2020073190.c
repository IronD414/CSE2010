#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

#define max(a, b) (a > b) ? a : b

struct AVLNode;
typedef struct AVLNode *Position;
typedef struct AVLNode *AVLTree;
typedef int ElementType;

typedef struct AVLNode{
	ElementType element;
	AVLTree left, right;
	int height;
}AVLNode;

int Height(Position T);
int differHeight(Position T);
int Find(ElementType X, AVLTree T);
AVLTree Insert(FILE* fout, ElementType X, AVLTree T);
AVLTree Delete(ElementType X, AVLTree T);
void PrintPreorder(FILE* fout, AVLTree T);
void DeleteTree(AVLTree T);
Position SingleRotateWithLeft(Position node);
Position SingleRotateWithRight(Position node);
Position DoubleRotateWithLeft(Position node);
Position DoubleRotateWithRight(Position node);

int Height(Position T)
{
    if (T == NULL) return -1;
    else return T->height;
}

int differHeight(Position T)    // return difference of children's height of certain node
{
    if (Height(T->left) > Height(T->right)) return Height(T->left) - Height(T->right);
    else return Height(T->right) - Height(T->left);
}

int Find(ElementType X, AVLTree T)
{
    Position tmpPtr = T;
    while (tmpPtr != NULL)
    {
        if (tmpPtr->element > X) tmpPtr = tmpPtr->left;
        else if (tmpPtr->element < X) tmpPtr = tmpPtr->right;
        else return 1;
    }
    return 0;
}
/*
Insert X in AVLTree T
return:
	pointer of root
print out:
	"insertion error : X is already in the tree!\n" , X is already in T
*/
AVLTree Insert(FILE* fout, ElementType X, AVLTree T)
{
    if (T == NULL)  //  insert X in root if T is null
    {
        T = (AVLTree) malloc(sizeof(struct AVLNode));
        T->element = X;
        T->height = 0;
        T->left = T->right = NULL;
        return T;
    } else if (Find(X, T))
    {
        fprintf(fout, "insertion error : X is already in the tree!\n");
        return T;
    } else if (T->element > X)  //  LL or LR
    {
        T->left = Insert(fout, X, T->left);
        if (differHeight(T) > 1)
        {
            if (T->left->element > X) T = SingleRotateWithLeft(T);  //  LL
            else T = DoubleRotateWithLeft(T);   //  LR
        }
    } else if (T->element < X)  //  RR or RL
    {
        T->right = Insert(fout, X, T->right);
        if (differHeight(T) > 1)
        {
            if (T->right->element > X) T = SingleRotateWithRight(T);  //  LL
            else T = DoubleRotateWithRight(T);   //  LR
        }
    }
    T->height = 1 + max(Height(T->left), Height(T->right));
    return T;
}

/*
Delete the X X in AVLTree T
return:
	pointer of root
print out:
	"deletion error : X is not in the tree!\n", X is not in T
*/
AVLTree Delete(ElementType X, AVLTree T)
{
    AVLTree present = T, prevL = NULL, prevR = NULL;
    while (1)
    {
        if (present != NULL)
        {
            if (present->element == X)  //  the case that is same
            {
                AVLTree ret = present->right, rret = present;
                if (present->right != NULL)
                {
                    for (;ret->left != NULL; rret = ret, ret = ret->left);
                    present->element = ret->element;
                    if (ret->right != NULL)
                        rret->left = ret->right;
                    else
                    {
                        free(rret->right);
                        rret->right = NULL;
                    }
                }
                else
                {
                    if (prevL != NULL)
                        prevL->left = present->left;
                    if (prevR != NULL)
                        prevR->right = present->left;
                    free(present);
                }
                break;
            }
            else if (present->element > X || present->element == -1)
            {
                prevL = present;
                prevR = NULL;
                present = present->left;
            }
            else if (present->element < X)
            {
                prevL = NULL;
                prevR = present;
                present = present->right;
            }
        }
        else break; //  couldn't find anything

    }
}
/*
Pre order Traversal
*/
void PrintPreorder(FILE* fout, AVLTree T)
{
    if (T == NULL) return;
    else
    {
        fprintf(fout, "%d(%d) ", T->element, T->height);
        PrintPreorder(fout, T->left);
        PrintPreorder(fout, T->right);
        return;
    }
}
/*
Delete Tree
free allocated memory
*/
void DeleteTree(AVLTree T)
{
    if (T == NULL) return;
    DeleteTree(T->left);
    DeleteTree(T->right);
    free(T);
}
/*
Rotation functions for AVLTree
*/
Position SingleRotateWithLeft(Position node)
{
    Position tmp = node->left;
    node->left = tmp->right;
    tmp->right = node;
    node->height = 1 + max(Height(node->left), Height(node->right));
    tmp->height = 1 + max(Height(tmp->left), Height(tmp->right));
    return tmp;
}
Position SingleRotateWithRight(Position node)
{
    Position tmp = node->right;
    node->right = tmp->left;
    tmp->left = node;
    node->height = 1 + max(Height(node->left), Height(node->right));
    tmp->height = 1 + max(Height(tmp->left), Height(tmp->right));
    return tmp;
}
Position DoubleRotateWithLeft(Position node)
{
    node->left = SingleRotateWithRight(node->right);    //   I still don't understand...
    return SingleRotateWithLeft(node);
}
Position DoubleRotateWithRight(Position node)
{
    node->right = SingleRotateWithLeft(node->left);    //   I still don't understand...
    return SingleRotateWithRight(node);
}

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	AVLTree Tree = NULL;
	char cv;
	int key;

	int i=0;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d\n", &key);
				Tree = Insert(fout, key, Tree);
				break;
			case 'd':
				fscanf(fin, "%d\n", &key);
				Tree = Delete(key, Tree);
				break;
		}
		PrintPreorder(fout, Tree);
		fprintf(fout, "\n");
	}
	DeleteTree(Tree);
	fclose(fin);
	fclose(fout);
	return 0;
}
