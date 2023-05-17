#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BNode* BNodePtr;

struct BNode{
    int order;
    int size;           /* number of children */
    BNodePtr *child;    /* children pointers */
    int *key;           /* keys */
    int is_leaf;
}BNode;



BNodePtr CreateTree(int order);

BNodePtr* helpInsert(BNodePtr* rootPtr, int key);

void Insert(BNodePtr* root, int key);

int Find(BNodePtr root, int key);

void PrintTree(BNodePtr root);

void DeleteTree(BNodePtr root);

int main(int argc, char* argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    int order;
    fscanf(fin, "%d", &order);
    BNodePtr root = CreateTree(order);

    char cv;
    int key;
    while(!feof(fin)){
        fscanf(fin, "%c", &cv);
        switch(cv){
            case 'i':
                fscanf(fin, "%d", &key);
                if(Find(root, key))
                    fprintf(fout, "insert error : key %d is already in the tree!\n", key);
                else
                    Insert(&root, key);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if(Find(root, key))
                    fprintf(fout, "key %d found\n", key);
                else
                    fprintf(fout, "finding error : key %d is not in the tree!\n", key);
                break;
            case 'p':
                if (root->size == 1)
                    fprintf(fout, "print error : tree is empty!");
                else
                    PrintTree(root);
                fprintf(fout, "\n");
                break;
        }
    }
   
    DeleteTree(root);
    fclose(fin);
    fclose(fout);

    return 0;
}

/*
Create new BTree with given order
order: order of BTree (order >= 2)
return: 
        the pointer of new BTree
 */
BNodePtr CreateTree(int order)
{
    BNodePtr newTree = (BNodePtr) malloc(sizeof(struct BNode));
    newTree->order = order;
    newTree->size = 1;
    newTree->child = (BNodePtr*) malloc(sizeof(BNodePtr) * order);
    newTree->key = (int*) malloc(sizeof(int) * (order - 1));
    newTree->is_leaf = 1;
    return newTree;
}

/*
Helping insert function
*/
BNodePtr *helpInsert(BNodePtr* rootPtr, int key)
{
    /*
    PART1: PREREQUISITES
        retNodes: return an array of BNodePtr, that consists of left, middle, and right
        idx_mid: index of middle of tmpKey
        tmpKey: array of int, key Appended 
        idx_key: index of place key should be placed
    */
    BNodePtr root = *rootPtr;
    BNodePtr* retNodes = (BNodePtr*) malloc(sizeof(BNodePtr) * 3);
    for (int i = 0; i < 3; ++i)
    {
        retNodes[i] = (BNodePtr) malloc(sizeof(struct BNode));
        retNodes[i]->key = (int*) malloc(sizeof(int) * (root->order - 1));
        retNodes[i]->child = (BNodePtr*) malloc(sizeof(BNodePtr) * root->order);
    }
    for (int n = 0; n < 3; ++n)
    {
        retNodes[n]->order = root->order;
        retNodes[n]->is_leaf = 0;
        if (n == 1) 
        {
            retNodes[n]->size = root->size;
            for (int i = 0; i < root->size - 1; ++i) 
                retNodes[n]->key[i] = root->key[i];
            for (int i = 0; i < root->size; ++i) 
                retNodes[n]->child[i] = root->child[i];
            retNodes[n]->is_leaf = root->is_leaf;
        }else retNodes[n]->size = 1;
    }
    int idx_mid;
    int* tmpKey = (int*) malloc(sizeof(int) * root->order);
    int idx_key = root->size - 1;
    int flag0 = 1;
    for (int i = 0; flag0 && i < root->size - 1; ++i) // finding the position
    {
        if (key < root->key[i]) 
        {
            idx_key = i;
            flag0 = 0;
        }
    }

    /*
    PART2: INSERTING
    */
    if (root->size < root->order) // just merge and return if there is enough space
    {
        if (!root->is_leaf)
        {
            BNodePtr* prev = helpInsert(&root->child[idx_key], key);
            if (prev[0]->size != 1)
            {
                int flag0 = 1;
                for (int i = 0; flag0 && i < root->size - 1; ++i) // finding the position
                {
                    if (prev[1]->key[0] < root->key[i]) 
                    {
                        idx_key = i;
                        flag0 = 0;
                    }
                }
                //fprintf(fout, "we have to upper %d into %d\n", prev[1]->key[0], idx_key);

                for (int i = root->size - 1; i > idx_key; i--) 
                    retNodes[1]->key[i] = root->key[i - 1];
                retNodes[1]->key[idx_key] = prev[1]->key[0];
                retNodes[1]->size++;
                for (int j = root->size - 1; j > idx_key; j--)
                    retNodes[1]->child[j + 1] = root->child[j];
                retNodes[1]->child[idx_key] = prev[0];
                retNodes[1]->child[idx_key + 1] = prev[2];
                return retNodes;
            }else
            {
                retNodes[1]->child[idx_key] = prev[1];
                return retNodes;
            }
        }else
        {
            for (int i = root->size - 1; i > idx_key; --i) 
                retNodes[1]->key[i] = root->key[i - 1];
            retNodes[1]->key[idx_key] = key;
            retNodes[1]->size++;
            return retNodes; 
        }
    }else if (root->is_leaf) // have to split when leaf
    {
        idx_mid = (root->order - 1) / 2;

        idx_key = root->size - 1;
        int flag = 1;
        for (int i = 0; flag && i < root->size - 1; ++i) // finding the position
        {
            if (key < root->key[i]) 
            {
                idx_key = i;
                flag = 0;
            }
        }

        for (int i0 = 0; i0 < idx_key; i0++) 
            tmpKey[i0] = root->key[i0];
        tmpKey[idx_key] = key;
        for (int i1 = idx_key + 1; i1 < root->order; i1++) 
            tmpKey[i1] = root->key[i1 - 1];
    // size modifying
        retNodes[0]->size = idx_mid + 1; 
        retNodes[1]->size = 2; 
        retNodes[2]->size = root->order - idx_mid;
    // key modifying
        for (int j0 = 0; j0 < idx_mid; ++j0) 
            retNodes[0]->key[j0] = tmpKey[j0];
        retNodes[1]->key[0] = tmpKey[idx_mid];
        for (int j1 = idx_mid + 1; j1 < root->order; ++j1) 
            retNodes[2]->key[j1 - idx_mid - 1] = tmpKey[j1];
    // is_leaf modifying
        retNodes[0]->is_leaf = 1; 
        retNodes[1]->is_leaf = 0; 
        retNodes[2]->is_leaf = 1;
        /*
        for (int i = 0; i < 3; ++i)
        {
            fprintf(fout, "my %d\n", retNodes[i]->key[0]);
        }
        */
    // child modifying
        retNodes[1]->child[0] = retNodes[0]; 
        retNodes[1]->child[1] = retNodes[2];
        return retNodes;
    }else // when not leaf
    {
        BNodePtr* prev = helpInsert(&root->child[idx_key], key);
        
        int flag0 = 1;
        for (int i = 0; flag0 && i < root->size - 1; ++i) // finding the position
        {
            if (prev[1]->key[0] < root->key[i]) 
            {
                idx_key = i;
                flag0 = 0;
            }
        }

        if (prev[0]->size <= 1) // if prev did not split, just point it and return
        {
            root->child[idx_key] = prev[1];
            retNodes[1] = root;
            return retNodes;
        }else // prev splited and cur split
        {
            int flag = 1;
            for (int i = 0; flag && i < root->size - 1; ++i) // finding the position
            {
                if (prev[1]->key[0] < root->key[i]) 
                {
                    idx_key = i;
                    flag = 0;
                }
            }
            idx_mid = (root->order - 1) / 2;
    
            for (int i0 = 0; i0 < idx_key; ++i0) 
                tmpKey[i0] = root->key[i0];
            tmpKey[idx_key] = key;
            for (int i1 = idx_key + 1; i1 < root->order; ++i1) 
                tmpKey[i1] = root->key[i1 - 1];
        // size modifying
            retNodes[0]->size = idx_mid + 1; 
            retNodes[1]->size = 2; 
            retNodes[2]->size = root->order - idx_mid;
        // child modifying
            if (idx_key == idx_mid)
            {
                for (int k0 = 0; k0 < idx_key; ++k0) 
                    retNodes[0]->child[k0] = root->child[k0];
                retNodes[0]->child[idx_key] = prev[0];
                retNodes[2]->child[0] = prev[2];
                for (int k1 = 1; k1 < root->order - idx_key; ++k1) 
                    retNodes[2]->child[k1] = root->child[k1 + idx_key];
            }else if (idx_key < idx_mid)
            {
                for (int k1 = 0; k1 < idx_key; ++k1) 
                    retNodes[0]->child[k1] = root->child[k1];
                for (int k2 = idx_key + 1; k2 < idx_mid; ++k2)
                    retNodes[0]->child[k2 + 1] = root->child[k2];
                retNodes[0]->child[idx_key] = prev[0];
                retNodes[0]->child[idx_key + 1] = prev[2];
                for (int k0 = idx_mid + 1; k0 < root->order; ++k0) 
                    retNodes[2]->child[k0 - idx_mid - 1] = root->child[k0 - 1];
            }else
            {
                for (int k0 = 0; k0 < idx_mid; ++k0) retNodes[0]->child[k0] = root->child[k0];
                for (int k1 = 0; k1 < idx_key; ++k1) 
                    retNodes[2]->child[k1] = root->child[idx_mid + 1 + k1];
                for (int k2 = idx_key + 1; k2 < root->order - idx_mid - 1; ++k2)
                    retNodes[2]->child[k2 + 1] = root->child[idx_mid + 1 + k2];
                retNodes[2]->child[idx_key] = prev[0];
                retNodes[2]->child[idx_key + 1] = prev[2];
            }
        // key modifying
            for (int j0 = 0; j0 < idx_mid; ++j0) 
                retNodes[0]->key[j0] = tmpKey[j0];
            retNodes[1]->key[0] = tmpKey[idx_mid];
            for (int j1 = idx_mid + 1; j1 < root->order; ++j1) 
                retNodes[2]->key[j1 - idx_mid - 1] = tmpKey[j1];
        // is_leaf modifying (not needed)
            retNodes[1]->child[0] = retNodes[0];
            retNodes[1]->child[1] = retNodes[2];
            return retNodes;
        }
    }

    /*
    PART3: FREEING
    */
    free(tmpKey);
}

/*
Insert the key value into BTree 
key: the key value in BTree node 
*/
void Insert(BNodePtr* root, int key)
{
    if ((*root)->size <= 1)
    {
        (*root)->key[0] = key;
        (*root)->size++;
        return;
    }
    BNodePtr newTree = helpInsert(root, key)[1];   
    (*root)->order = newTree->order;
    (*root)->size = newTree->size;
    for (int i = 0; i < newTree->size - 1; ++i)
        (*root)->key[i] = newTree->key[i];
    for (int j = 0; j < newTree->size; ++j)
        (*root)->child[j] = newTree->child[j];
    (*root)->is_leaf = newTree->is_leaf;
    return;
}

/*
Find node that has key in BTree
key: the key value in BTree node 
return 1 if exists, else 0
*/
int Find(BNodePtr root, int key)
{
    if (root->is_leaf)
    {
        for (int i = 0; i < root->size - 1; ++i)
        {
            if (key == root->key[i]) return 1;
            else if (key < root->key[i]) return 0;
        }
        return 0;
    }else
    {
        for (int i = 0; i < root->size - 1; ++i)
        {
            if (key == root->key[i]) return 1;
            else if (key < root->key[i]) return Find(root->child[i], key);
        }
        return Find(root->child[root->size - 1], key);
    }
}

/* 
Print Tree, inorder traversal 
*/
void PrintTree(BNodePtr root)
{
    if (root->is_leaf) 
        for (int i = 0; i < root->size - 1; ++i) 
            fprintf(fout, "%d ", root->key[i]);
    else
    {
        for (int j = 0; j < root->size - 1; ++j)
        {
            PrintTree(root->child[j]);
            fprintf(fout, "-%d ", root->key[j]);
        }
        fprintf(fout, "-");
        PrintTree(root->child[root->size - 1]);
    }
}

/*
Free memory, delete a BTree completely 
*/
void DeleteTree(BNodePtr root)
{
    if (root->is_leaf)
    {
        free(root->child);
        free(root->key);
        free(root);
    }else
    {
        for (int j = 0; j < root->size - 1; ++j)
        {
            DeleteTree(root->child[j]);
        }
        DeleteTree(root->child[root->size - 1]);
        free(root->child);
        free(root->key);
        free(root);
    }
}