/*
Programação Concorrente e Distribuída - 2022/2

Trabalho 1 
Game of Life / High Life - PThreads e OpenMP

Bruno Rasera
Letícia Lisboa
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define SIZE 2048
#define GENERATIONS 2000

int getNumberOfNeighborsAlive(int row, int column, char grid[SIZE][SIZE])
{
    int count = 0;
    int previousRow, previousColumn, nextRow, nextColumn;

    // Checks if row or column is on the edge, since board needs infinite edges
    if (row == 0)
    {
        previousRow = SIZE-1;
    }
    else
    {
        previousRow = row - 1;
    }
    if (row == SIZE-1)
    {
        nextRow = 0;
    }
    else
    {
        nextRow = row + 1;
    }
    if (column == 0)
    {
        previousColumn = SIZE-1;
    }
    else
    {
        previousColumn = column - 1;
    }
    if (column == SIZE-1)
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

int countAliveCells(char grid[SIZE][SIZE])
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

void initializeWithZeros(char grid[SIZE][SIZE])
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

void calculateNewGrid(char grid[SIZE][SIZE], char newgrid[SIZE][SIZE])
{
    int i, j;

    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            int neighborsAlive = getNumberOfNeighborsAlive(i, j, grid);

            // Dead cell with 3 or 6 alive neighbors becomes alive
            if (grid[i][j] == 0 && (neighborsAlive == 3 || neighborsAlive == 6))
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

    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            grid[i][j] = newgrid[i][j];
        }
    }
}

void printGrid(char grid[SIZE][SIZE], int printSize)
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
    char grid[SIZE][SIZE], newgrid[SIZE][SIZE];
    int i, j;
    clock_t start, end;
    double cpu_time_used;

    // Initialize grids with zeros
    initializeWithZeros(grid);
    initializeWithZeros(newgrid);

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

    start = clock();
    for ( i = 0; i < GENERATIONS; i++)
    {
        calculateNewGrid(grid, newgrid);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    printf("Alive: %d \n", countAliveCells(grid));
    printf("Time taken: %f \n", cpu_time_used);

    return 0;
}