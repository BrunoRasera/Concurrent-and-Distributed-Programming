/*
Programação Concorrente e Distribuída - 2022/2

Trabalho 1
Game of Life / High Life - PThreads e OpenMP

Bruno Rasera
Letícia Lisboa

To increase the stack size in linux and avoid segmentation fault errors, use ulimit -s unlimited 
To compile openMP version, use the -fopenmp flag on gcc
To compite pthread version, use  the -pthread flag on gcc
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>

#define SIZE 2048
#define GENERATIONS 2000
#define NUM_THREADS 12

char grid[SIZE][SIZE], newgrid[SIZE][SIZE];

int getNumberOfNeighborsAlive(int row, int column)
{
    int count = 0;
    int previousRow, previousColumn, nextRow, nextColumn;

    // Checks if row or column is on the edge, since board needs infinite edges
    if (row == 0)
    {
        previousRow = SIZE - 1;
    }
    else
    {
        previousRow = row - 1;
    }
    if (row == SIZE - 1)
    {
        nextRow = 0;
    }
    else
    {
        nextRow = row + 1;
    }
    if (column == 0)
    {
        previousColumn = SIZE - 1;
    }
    else
    {
        previousColumn = column - 1;
    }
    if (column == SIZE - 1)
    {
        nextColumn = 0;
    }
    else
    {
        nextColumn = column + 1;
    }

    // Calculate neighbors alive
    if (grid[previousRow][previousColumn] == 1)
        count++;
    if (grid[previousRow][column] == 1)
        count++;
    if (grid[previousRow][nextColumn] == 1)
        count++;
    if (grid[row][nextColumn] == 1)
        count++;
    if (grid[nextRow][nextColumn] == 1)
        count++;
    if (grid[nextRow][column] == 1)
        count++;
    if (grid[nextRow][previousColumn] == 1)
        count++;
    if (grid[row][previousColumn] == 1)
        count++;

    return count;
}

int countAliveCells()
{
    int i, j, alive = 0;

    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            if (grid[i][j] == 1)
            {
                alive++;
            }
        }
    }
    return alive;
}

void initializeWithZeros()
{
    int i, j;

    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            grid[i][j] = 0;
        }
    }
}

void *calculateNewGrid(void *threadarg)
{
    int i, j;
    int *th_id = (int*)threadarg;

    for (i = 0; i < SIZE; i++)
    {
        for (j = *th_id * (SIZE / NUM_THREADS); j < (*th_id + 1) * (SIZE / NUM_THREADS); j++)
        {
            int neighborsAlive = getNumberOfNeighborsAlive(i, j);

            // Dead cell with 3 alive neighbors becomes alive
            if (grid[i][j] == 0 && neighborsAlive == 3)
            {
                newgrid[i][j] = 1;
            }

            // Alive cell
            if (grid[i][j] == 1)
            {
                // Alive cell dies
                if (neighborsAlive < 2 || neighborsAlive > 3)
                {
                    newgrid[i][j] = 0;
                }
                // Alive cell continues to live
                else
                {
                    newgrid[i][j] = 1;
                }
            }
        }
    }
    pthread_exit(NULL);
}

void printGrid(int printSize)
{
    int i, j;

    for (i = 0; i < printSize; i++)
    {
        for (j = 0; j < printSize; j++)
        {
            if (grid[i][j] == 0)
                printf(". ");
            else
                printf("# ");
        }
        printf("\n");
    }
}

int main(int argc, char **argv)
{
    int i, j, k, thread_id[NUM_THREADS];
    pthread_t thread[NUM_THREADS];
    struct timeval start, final;

    // Initialize grids with zeros
    initializeWithZeros();
    initializeWithZeros();

    // Add a Glider
    int lin = 1, col = 1;
    grid[lin][col + 1] = 1;
    grid[lin + 1][col + 2] = 1;
    grid[lin + 2][col] = 1;
    grid[lin + 2][col + 1] = 1;
    grid[lin + 2][col + 2] = 1;

    // Add a R-pentomino
    lin = 10;
    col = 30;
    grid[lin][col + 1] = 1;
    grid[lin][col + 2] = 1;
    grid[lin + 1][col] = 1;
    grid[lin + 1][col + 1] = 1;
    grid[lin + 2][col + 1] = 1;

    gettimeofday(&start, NULL);
    
    for (k = 0; k < GENERATIONS; k++)
    {
        // Creating threads
        for (i = 0; i < NUM_THREADS; i++)
        {
            thread_id[i] = i;
            pthread_create(&thread[i], NULL, &calculateNewGrid, (void *)&thread_id[i]);
        }

        // Joining threads
        for (i = 0; i < NUM_THREADS; i++)
        {
            pthread_join(thread[i], NULL);
        }

        // Copying result from newgrid to grid
        for (i = 0; i < SIZE; i++)
        {
            for (j = 0; j < SIZE; j++)
            {
                grid[i][j] = newgrid[i][j];
            }
        }
    }
    
    gettimeofday(&final, NULL);

    printf("Alive: %d \n", countAliveCells(grid));
    printf("Time elapsed: %d seconds\n", (int)(final.tv_sec - start.tv_sec));

    pthread_exit(NULL);
    return 0;
}