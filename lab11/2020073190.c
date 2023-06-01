#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *fin;
FILE *fout;

typedef int ElementType;
typedef ElementType List;
typedef struct HashTbl* HashTable;

typedef struct HashTbl{
	int TableSize;
	List *TheLists;
}HashTbl;

HashTable createTable(int TableSize);
void Insert(HashTable H, ElementType Key, int solution);
void Delete(HashTable H, ElementType Key, int solution);
int Find(HashTable H, ElementType Key, int solution);
void printTable(HashTable H);
void deleteTable(HashTable H);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");


	char solution_str[20];
	int solution, TableSize;

	fscanf(fin, "%s", solution_str);
	if(!strcmp(solution_str, "linear"))
		solution = 1;
	else if(!strcmp(solution_str, "quadratic"))
		solution = 2;
	else{
		fprintf(fout, "Error: Invalid hashing solution!");
		return 0;
	}
	
	fscanf(fin, "%d", &TableSize);

	HashTable H = createTable(TableSize);

	char cv;
	int key;
	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d", &key);
				Insert(H, key, solution);
				break;

			case 'f':
				fscanf(fin, "%d", &key);
				int result = Find(H, key, solution);
				if(result)
					fprintf(fout, "%d is in the table\n", key);
				else
					fprintf(fout, "%d is not in the table\n", key);
				break;

			case 'd':
				fscanf(fin, "%d", &key);
				Delete(H, key, solution);
				break;

			case 'p':
				printTable(H);
				break;
		}
	}

	deleteTable(H);

	return 0;
}

/*
Create new Hash Table with given TableSize
reture:
	the pointer of new hash table
*/
HashTable createTable(int TableSize)
{
	int i;
	HashTable newHT = (HashTable) malloc(sizeof(struct HashTbl));
	newHT->TableSize = TableSize;
	newHT->TheLists = (List*) malloc(sizeof(List) * TableSize);
	for (i = 0; i < TableSize; ++i)
		newHT->TheLists[i] = 0;
	return newHT;
}

/*
Insert the key in hash table with given solution (linear or quadratic).
print out : 
	Successful insertion : "insert %d into address %d\n"
	When inserting duplicate key value : "insertion error : %d already exists at address %d\n"
	When HashTable is full : "insertion error : table is full\n"
*/
void Insert(HashTable H, ElementType Key, int solution)
{
	int i, j = 0, hi;
	// first make sure that table is not full
	for (i = 0; i < H->TableSize; ++i)
		if (H->TheLists[i] == 0) 
			j++;
	if (j == 0)
	{
		fprintf(fout, "insertion error : table is full\n");
		return;
	}
	// then make sure that key is not in the table
	if (Find(H, Key, solution))
	{
		for (i = 0; i < H->TableSize; ++i)
			if (H->TheLists[i] == Key) fprintf(fout, "insertion error : %d already exists at address %d\n", Key, i);
		return;
	}else
	// then insert key by solution type
	{
		i = 0;
		if (solution == 1)
		{
			while (H->TheLists[(Key + i) % H->TableSize] != 0)
			{
				i++; 
				if (i >= H->TableSize)
				{
					fprintf(fout, "insertion error : table is full\n");
					return;
				}
			}	
		}else if (solution == 2)
		{
			j = 0;
			while (H->TheLists[(Key + i) % H->TableSize] != 0)
			{
				j++;
				i = j*j;
				if (j >= H->TableSize)
				{
					fprintf(fout, "insertion error : table is full\n");
					return;
				}
			}
		}
		hi = (Key + i) % H->TableSize;
		H->TheLists[hi] = Key;
		fprintf(fout, "insert %d into address %d\n", Key, hi);
		return;
	}
}
/*
Delete the key in hash table with given solution (linear or quadratic).
print out : 	
	when key value is found :  "delete %d\n"
	when key value does not exist : %d is not in the table\n"
*/
void Delete(HashTable H, ElementType Key, int solution)
{
	// make sure that table is not empty
	if (H == NULL || H->TableSize == 0 || H->TheLists == NULL) 
	{
		fprintf(fout, "deletion error : NULL Exception\n"); 
		return;
	}else
	{
		int i, hi = Key % H->TableSize;
		for (i = 0; i < H->TableSize; ++i)
			if (H->TheLists[i] != 0)
			{
				// make sure that key is found
				if (!Find(H, Key, solution)) 
				{
					fprintf(fout, "deletion error : %d is not in the table\n", Key); 
					return;
				}else
				// then delete the key in the table
				{
					for (i = 0; i < H->TableSize; ++i)
						if (H->TheLists[i] == Key) H->TheLists[i] = 0;
					return;
				}
			}
		fprintf(fout, "deletion error : Empty Table Exception\n");
		return;
	}
}

/*
Find the Key from hash table with given solution (linear or quadratic).
return:
	0 : when the key is not found
	1 : when the key is found
*/
int Find(HashTable H, ElementType Key, int solution)
{
	int i;
	for (i = 0; i < H->TableSize; ++i)
		if (H->TheLists[i] == Key) return 1;
	return 0;
}
/*
Print all values of Hash Table
print out the key values ​​in the Hash Table in the order of index in the Hash Table.
	empty Hash Table print : "0 "
	Non empty Hash Table print : "%d "
*/
void printTable(HashTable H)
{
	int i;
	for (i = 0; i < H->TableSize; ++i)
		fprintf(fout, "%d ", H->TheLists[i]);
	fprintf(fout, "\n");
}
/*
delete Table 
*/
void deleteTable(HashTable H)
{
	free(H->TheLists);
	free(H);
}
