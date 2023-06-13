#include<stdio.h>
#include<stdlib.h>

FILE* fin;
FILE* fout;

typedef struct Array Array;

struct Array{
    int size;
    int* values;
};

Array* CreateArray(int size);
void QuickSort(Array* array, int left, int right);
int Partition(Array* array, int left, int right);
void PrintArray(Array* array, int left, int right);
void DeleteArray(Array* array);
void swap(int* a, int* b);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

    int size, i;
    Array* array;

    fscanf(fin, "%d", &size);
    array = CreateArray(size);
    for(i = 0; i < size; i++){
        fscanf(fin, "%d", &array->values[i]);
    }

    QuickSort(array, 0, size - 1);

    fprintf(fout, "\nsorting result :\n");
    PrintArray(array, 0, size - 1);

    DeleteArray(array);


	return 0;
}

/*
Create new array 
input : size of the number to be sorted
output : the pointer of array
*/
Array* CreateArray(int size)
{
    Array* newArr = (Array*) malloc(sizeof(Array));
    newArr->size = size;
    newArr->values = (int*) malloc(sizeof(int) * size);
    return newArr;
}

/*
Quick sort
*/
void QuickSort(Array* array, int left, int right)   
{
    if (right - left == 0) return;
    else
    {
        int p = Partition(array, left, right);
        if (p > left) QuickSort(array, left, p-1);
        if (p < right) QuickSort(array, p+1, right);
    }
}

/*
Divide into 2 sub-lists based on pivot.
+ Select right-most data as pivot
+ Print left, right, pivot index at first
+ Print array values for every swap -> Printing array values should be implemented in a PrintArray function
Print format : 
    1. "left : %d, right : %d, pivot : %d\n"
    2. refered on PrintArray function
return:
    pivot index
*/
int Partition(Array* array, int left, int right)    //  left, right, pivot are index
{
    int pivot = right;

    fprintf(fout, "left : %d, right : %d, pivot : %d\n", left, right, array->values[pivot]);
    PrintArray(array, left, right);

    if (right - left <= 1 && array->values[left] > array->values[right])    //  # of elem is 2 and need to swap
    { 
        swap(&array->values[left], &array->values[right]);
        PrintArray(array, left, right);
    }
    else if (right - left <= 1) return pivot;
    else
    {
        int i = left - 1, j = right;

        for (;;)
        {
            while (array->values[++i] <= array->values[pivot]);
            while (array->values[--j] >= array->values[pivot]);

            if (i < j) swap(&array->values[i], &array->values[j]);
            else if (i < pivot)
            {
                swap(&array->values[i], &array->values[right]);
                pivot = i;
                PrintArray(array, left, right);
                return pivot;
            }

            PrintArray(array, left, right);
            if (i >= j) break;
        }
    }
    
    return pivot;
}

/*
Print array values 
Print format :
    “%-3s” : out of bound and should be filled with "-" charater
    “%-3d” : array values to be printed
*/
void PrintArray(Array* array, int left, int right){
    // fprintf(fout, "%-3s", "-");
    // fprintf(fout, "%-3d", array->values[i]);
    // fprintf(fout, "\n");
    for (int i = 0; i < array->size; ++i)
    {
        if (left <= i && i <= right) fprintf(fout, "%-3d", array->values[i]);
        else fprintf(fout, "%-3s", "-");
    }
    fprintf(fout, "\n");
}

/*
DeleteArray
*/
void DeleteArray(Array* array)
{
    free(array->values);
    free(array);
}

/*
Swap values
*/
void swap(int* a, int* b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
