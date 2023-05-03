#include<stdio.h>
#include<stdlib.h>
#include<time.h>

FILE *fin;
FILE *fout;

typedef struct _DisjointSet {
	int size_maze;
	int *ptr_arr;
}DisjointSets;

void init(DisjointSets *sets, DisjointSets *maze_print, int num);
void Union(DisjointSets *sets, int i, int j);
void Uni_R(DisjointSets *sets, int i);
void Uni_D(DisjointSets *sets, int i);
void Uni_L(DisjointSets *sets, int i);
void Uni_U(DisjointSets *sets, int i);
int isFinished(DisjointSets *sets);
int find(DisjointSets *sets, int i);
void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num);
void printMaze(DisjointSets *sets, int num);
void freeMaze(DisjointSets *sets, DisjointSets *maze_print);


int main(int argc, char* agrv[]) {
	srand((unsigned int)time(NULL));

	int num, i;
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");

	DisjointSets *sets, *maze_print;
	fscanf(fin, "%d", &num);

	sets = (DisjointSets*)malloc(sizeof(DisjointSets));
	maze_print = (DisjointSets*)malloc(sizeof(DisjointSets));

	init(sets, maze_print, num);
	createMaze(sets, maze_print, num);
	printMaze(maze_print, num);

	freeMaze(sets, maze_print);

	fclose(fin);
	fclose(fout);

	return 0;
}
/*
Allocate and Initialize Disjoint sets
    "sets": have num*num disjoint sets
    "maze_print": have num*num*2 values (two directions: right, down)   
        the values are either 0(no wall) or 1 (wall)
*/
void init(DisjointSets *sets, DisjointSets *maze_print, int num) 
{
	sets->size_maze = num;
	sets->ptr_arr = (int*) malloc(sizeof(int) * num * num);
	for (int i = 0; i < num*num; ++i)
	{
		sets->ptr_arr[i] = i;
	}
	maze_print->size_maze = num;
	maze_print->ptr_arr = (int*) malloc(sizeof(int) * num * num * 2);

	for (int i = 0; i < num*num; ++i)
	{
		maze_print->ptr_arr[i] = 1;
		maze_print->ptr_arr[i + num*num] = 1;
	}
	maze_print->ptr_arr[num*num - 1] = 0;
	return;	
}

/*
Merge two disjoint sets including i and j respectively
*/
void Union(DisjointSets *sets, int i, int j) 
{
	sets->ptr_arr[find(sets, i)] = find(sets, j);
		/*
		for (int i = 0; i < sets->size_maze; ++i)
		{
			for (int j = 0; j < sets->size_maze; ++j)
			{
				fprintf(fout, "%d, ", sets->ptr_arr[j + i*sets->size_maze]);
			}
			fprintf(fout, "\n");
		}
		*/
	return;
}
void Uni_R(DisjointSets *sets, int i){Union(sets, i, i + 1);}
void Uni_D(DisjointSets *sets, int i){Union(sets, i, i + sets->size_maze);}
void Uni_L(DisjointSets *sets, int i){Union(sets, i, i - 1);}
void Uni_U(DisjointSets *sets, int i){Union(sets, i, i - sets->size_maze);}
int isFinished(DisjointSets *sets)
{
	int x = find(sets, sets->ptr_arr[0]);
	for (int i = 0; i < (sets->size_maze * sets->size_maze); ++i)
	{
		if (find(sets, sets->ptr_arr[i]) != x)
		{
			return 0;
		}
	}
	return 1;
}
/*
find the set including given element "i" and return the representative element  
*/
int find(DisjointSets *sets, int i) 
{
	int tmp = sets->ptr_arr[i];
	while(sets->ptr_arr[tmp] != tmp)
	{
		tmp = sets->ptr_arr[tmp];
	}	
	return tmp;
}
/*
Create Maze without cycle
You should generate the maze by randomly choosing cell and direction 
*/
void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num) 
{
	int cell;
	int direction;
	while(!isFinished(sets))
	{
		cell = rand() % (num * num);
		direction = rand() % 4;
		switch(direction) {
		case 0:	// Right
			if (((cell % num) != (num - 1)) && (maze_print->ptr_arr[cell] != 0)) 
			{
				/*
				fprintf(fout, "cell number: %d\n", cell);
				fprintf(fout, "direction number: %d\n", direction);	// 0123 R D L U
				*/
				if (find(sets, sets->ptr_arr[cell]) == find(sets, sets->ptr_arr[cell+1])) break;
				Uni_R(sets, cell);
				maze_print->ptr_arr[cell] = 0;
			}
			break;
		case 1:	// Down
			if ((num*num - num > cell)&& (maze_print->ptr_arr[cell + num*num] != 0)) 
			{
				if (find(sets, sets->ptr_arr[cell]) == find(sets, sets->ptr_arr[cell+num])) break;
				Uni_D(sets, cell);
				maze_print->ptr_arr[cell + num*num] = 0;
			}
			break;
		case 2:	// Left
			if ((cell % num) != 0) 
			{
				if (maze_print->ptr_arr[cell - 1] != 0)
				{
					if (find(sets, sets->ptr_arr[cell]) == find(sets, sets->ptr_arr[cell-1])) break;
					Uni_L(sets, cell);
					maze_print->ptr_arr[cell - 1] = 0;
				}
			}
			break;
		case 3:	// Up
			if (cell > num-1) 
			{
				if (maze_print->ptr_arr[cell - num + num*num] != 0)
				{
					if (find(sets, sets->ptr_arr[cell]) == find(sets, sets->ptr_arr[cell-num])) break;
					Uni_U(sets, cell);
					maze_print->ptr_arr[cell - num + num*num] = 0;
				}
			}
			break;
		}
		/*
		if (cell == 0)	// upmost leftmost
		{	
			if (direction == 0)
			{
				Uni_R(sets, cell);
				maze_print->ptr_arr[cell] = 0;
			}else
			{
				Uni_D(sets, cell);
				maze_print->ptr_arr[cell + num*num] = 0;
			}
		}else if (cell == num - 1)	// upmost rightmost
		{	
			if (direction == 0)
			{
				Uni_D(sets, cell);
				maze_print->ptr_arr[cell + num*num] = 0;
			}else
			{
				Uni_L(sets, cell);
				maze_print->ptr_arr[cell - 1] = 0;
			}
		}else if (cell == num * num - 1)	// downmost rightmost
		{
			if (direction == 0)
			{
				Uni_L(sets, cell);
				maze_print->ptr_arr[cell - 1] = 0;
			}else
			{
				Uni_U(sets, cell);
				maze_print->ptr_arr[cell - num + num*num] = 0;
			}
		}else if (cell == num * num - num)	// downmost leftmost
		{
			if (direction == 0)
			{
				Uni_R(sets, cell);
				maze_print->ptr_arr[cell] = 0;
			}else
			{
				Uni_U(sets, cell);
				maze_print->ptr_arr[cell - num + num*num] = 0;
			}
		}else
		{
			direction = rand() % 3;
			fprintf(fout, "nono, direction number: %d\n", direction);
			if ((0 < cell) && (cell < num - 1))	// upmost edge
			{
				switch(direction)
				{
					case 0:
						Uni_R(sets, cell);
						maze_print->ptr_arr[cell] = 0;
						break;
					case 1:
						Uni_D(sets, cell);
						maze_print->ptr_arr[cell + num*num] = 0;
						break;
					case 2:
						Uni_L(sets, cell);
						maze_print->ptr_arr[cell - 1] = 0;
						break;
				}
			}else if ((cell % num) == (num - 1))	// rightmost edge
			{
				switch(direction)
				{
					case 0:
						Uni_D(sets, cell);
						maze_print->ptr_arr[cell + num*num] = 0;
						break;
					case 1:
						Uni_L(sets, cell);
						maze_print->ptr_arr[cell - 1] = 0;
						break;
					case 2:
						Uni_U(sets, cell);
						maze_print->ptr_arr[cell - num + num*num] = 0;
						break;
				}
			}else if ((num * num - num < cell) && (cell < num * num - 1))	// downmost edge
			{
				switch(direction)
				{
					case 0:
						Uni_R(sets, cell);
						maze_print->ptr_arr[cell] = 0;
						break;
					case 1:
						Uni_L(sets, cell);
						maze_print->ptr_arr[cell - 1] = 0;
						break;
					case 2:
						Uni_U(sets, cell);
						maze_print->ptr_arr[cell - num + num*num] = 0;
						break;
				}
			}else if ((cell % num) == 0)	// leftmost edge
			{
				switch(direction)
				{
					case 0:
						Uni_R(sets, cell);
						maze_print->ptr_arr[cell] = 0;
						break;
					case 1:
						Uni_D(sets, cell);
						maze_print->ptr_arr[cell + num*num] = 0;
						break;
					case 2:
						Uni_U(sets, cell);
						maze_print->ptr_arr[cell - num + num*num] = 0;
						break;
				}
			}else	// middle
			{
				direction = rand() % 4;
				fprintf(fout, "nono, nononono, direction number: %d\n", direction);
				switch(direction)
				{
					case 0:
						Uni_R(sets, cell);
						maze_print->ptr_arr[cell] = 0;
						break;
					case 1:
						Uni_D(sets, cell);
						maze_print->ptr_arr[cell + num*num] = 0;
						break;
					case 2:
						Uni_L(sets, cell);
						maze_print->ptr_arr[cell - 1] = 0;
						break;
					case 3:
						Uni_U(sets, cell);
						maze_print->ptr_arr[cell - num + num*num] = 0;
						break;
				}
			}
		}
		*/
	}
}
/*
Print Maze
Example> 
+---+---+---+---+---+
            |       |
+   +   +---+   +---+
|   |       |       |
+---+   +---+   +   +
|               |   |
+   +---+   +   +---+
|   |       |       |
+   +   +---+---+---+
|   |                
+---+---+---+---+---+
*/
void printMaze(DisjointSets *sets, int num) 
{
	/* for n: print very first line */
	for (int i = 0; i < num; ++i)
	{
		fprintf(fout, "+---");
	}
	fprintf(fout, "+\n");

	/* for n: print 2 line is repeated */
	for (int i = 0; i < num; ++i)
	{
		if (i > 0) fprintf(fout, "|");
		else fprintf(fout, " ");
	/* 		for n: [i][0]==1 -> print '|', nothing if 0 */
		for (int j = 0; j < num; ++j)
		{
			fprintf(fout, "   ");
			sets->ptr_arr[j + i*num] ? fprintf(fout, "|") : fprintf(fout, " ");
		}
		fprintf(fout, "\n");
	/* 		for n: [i][0]==1 -> print '---', nothing if 0 */
		for (int k = 0; k < num; ++k)
		{
			fprintf(fout, "+");
			sets->ptr_arr[k + i*num + num*num] ? fprintf(fout, "---") : fprintf(fout, "   ");
		}
		fprintf(fout, "+\n");
	}	
}
/*
Deallocate sets
*/
void freeMaze(DisjointSets *sets, DisjointSets * maze_print) 
{
	free(sets->ptr_arr);
	free(maze_print->ptr_arr);
	free(sets);
	free(maze_print);
	return;
}
