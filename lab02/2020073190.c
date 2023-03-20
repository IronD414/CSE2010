#include<stdio.h>
#include<stdlib.h>
#include <malloc.h>

FILE *fin;
FILE *fout;

typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef int ElementType;

struct Node
{
	ElementType element;
	Position next;
};

List MakeEmpty(List L);
int IsEmpty(List L);
int IsLast(Position P, List L);
void Insert(ElementType X, List L, Position P);
Position FindPrevious(ElementType X, List L);
Position Find(ElementType X, List L);
void Delete(ElementType X, List L);
void DeleteList(List L);
int* GetElements(List L);
int ListSize=0;

// define area ends here

int main(int argc, char **argv)
{
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");
	char x;
	int* elements;

	Position header=NULL, tmp=NULL;
	header = MakeEmpty(header);
	
	while(fscanf(fin,"%c",&x)!=EOF)
	{
		if(x=='i')
		{
			int a,b;	fscanf(fin,"%d%d",&a,&b);
			tmp = Find(a, header);
			if(tmp!=NULL)
			{
				fprintf(fout, "insertion(%d) failed : the key already exists in the list\n", a);
				continue;
			}
			tmp = Find(b, header);
			if(tmp==NULL)
			{
				fprintf(fout, "insertion(%d) failed : can not find the location to be inserted\n", a);
				continue;
			}
			Insert(a, header, tmp);
			ListSize++;
		}
		else if(x=='d')
		{
			int a;	fscanf(fin,"%d",&a);
			tmp = Find(a, header);
			if(tmp==NULL)
			{
				fprintf(fout, "deletion(%d) failed : element %d is not in the list\n", a, a);
				continue;
			}
			Delete(a, header);
			ListSize--;
		}
		else if(x=='f')
		{
			int a;	fscanf(fin,"%d",&a);
			tmp = FindPrevious(a, header);
			if(IsLast(tmp, header)) fprintf(fout, "finding(%d) failed : element %d is not in the list\n",a,a);
			else
			{
				if(tmp->element>0) fprintf(fout, "key of the previous node of %d is %d\n", a, tmp->element);
				else fprintf(fout, "key of the previous node of %d is head\n",a);
			}
		}
		else if(x=='p') 
		{
			if(IsEmpty(header))
				fprintf(fout, "empty list!\n");
			else
			{
				elements = GetElements(header);
				for(int i = 0; i < ListSize; i++)
				{
					fprintf(fout, "key:%d ", elements[i]);
				}
				fprintf(fout, "\n");
				free(elements);
			}
		}
	}
	DeleteList(header);
	fclose(fin);
	fclose(fout);
	
	return 0;
}

List MakeEmpty(List L)
{
	L = (List) malloc(sizeof(struct Node));
	L->element = -1;
	return L;
}

int IsEmpty(List L) // return (L is empty)
{
	return L->next == NULL;
}

int IsLast(Position P, List L) // return (P == last)
{
	Position tmp = L;
	while (tmp->next != NULL) tmp = tmp->next;
	return P == tmp;
}

void Insert(ElementType X, List L, Position P)
{
	if (P->element == -1)
	{
		Position newNode = (Position) malloc(sizeof(struct Node));
		Position oldHead = L->next;
		L->next = newNode;
		L->next->next = oldHead;
		L->next->element = X;
	}
	else
	{
		Position tmp = L->next;
		while (tmp != P)
		{
			tmp = tmp->next;
		}
		Position newNode = (Position) malloc(sizeof(struct Node));
		tmp->next = newNode;
		tmp->next->element = X;
	}
}

int* GetElements(List L) // copy the elements to array of int from L, return NULL if L is empty
{
	int* newIntArray = (int*) malloc(sizeof(int)*ListSize);

	Position copying = L->next; int i = 0;
	while (copying != NULL)
	{
		newIntArray[i++] = copying->element;
		copying = copying->next;
	}
	return newIntArray;
}

Position FindPrevious(ElementType X, List L) // find previous node of X, return the Last one if X is not in L
{
	Position xpos = Find(X, L);
	if (xpos == NULL)
	{
		Position tmp = L;
		while (tmp->next != NULL) tmp = tmp->next;
		return tmp;
	}
	else
	{
		Position tmp = L;
		while (tmp->next != xpos) tmp = tmp->next;
		return tmp;	
	}
}

Position Find(ElementType X, List L) // return the position of X in L, return NULL if it doesn't exist
{
	if (X == -1) return L;
	Position tmp = L->next;
	while (tmp != NULL)
	{
		if (tmp->element == X) return tmp;
		tmp = tmp->next;
	}
	return NULL;
}

void Delete(ElementType X, List L)
{
	Position tmp = L;
	while (tmp->next->element != X)
	{
		tmp = tmp->next;
	}
	Position after_right = tmp->next->next;
	free(tmp->next);
	tmp->next = after_right;
}

void DeleteList(List L) // delete(freeing) the whole list
{
	Position p = L; Position tmp;
	while (p != NULL)
	{
		tmp = p->next;
		free(p);
		p = tmp;
	}
}