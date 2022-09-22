#include <stdlib.h>
#include <stdio.h>

#define SIZE 1024

int getNumberOfNeighbors(int row, int column, char grid[SIZE][SIZE])
{
    int count = 0;

    if (grid[row - 1][column - 1] == 1)
        count++;
    if (grid[row - 1][column] == 1)
        count++;
    if (grid[row - 1][column + 1] == 1)
        count++;
    if (grid[row][column + 1] == 1)
        count++;
    if (grid[row + 1][column + 1] == 1)
        count++;
    if (grid[row + 1][column] == 1)
        count++;
    if (grid[row + 1][column - 1] == 1)
        count++;
    if (grid[row][column - 1] == 1)
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

int main(int argc, char **argv)
{
    char grid[SIZE][SIZE], newgrid[SIZE][SIZE];
    int i, j;

    // Initialize grids with zeros
    initializeWithZeros(grid);
    initializeWithZeros(newgrid);

    // GLIDER
    int lin = 1, col = 1;
    grid[lin][col + 1] = 1;
    grid[lin + 1][col + 2] = 1;
    grid[lin + 2][col] = 1;
    grid[lin + 2][col + 1] = 1;
    grid[lin + 2][col + 2] = 1;

    // R-pentomino
    lin = 10;
    col = 30;
    grid[lin][col + 1] = 1;
    grid[lin][col + 2] = 1;
    grid[lin + 1][col] = 1;
    grid[lin + 1][col + 1] = 1;
    grid[lin + 2][col + 1] = 1;

    printf("Alive: %d \n", countAliveCells(grid));

    return 0;
}