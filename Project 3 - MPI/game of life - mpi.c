/*
Programação Concorrente e Distribuída - 2022/2

Trabalho 3
Game of Life - MPI

Bruno Rasera
Letícia Lisboa
Daniel Paiva

To increase the stack size in linux and avoid segmentation fault errors, use ulimit -s unlimited
To compile MPI version, use 'mpicc game\ of\ life\ -\ mpi.c'
To run, use  'mpiexec -np <number of procesess> ./a.out'
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <mpi.h>

#define SIZE 2048
#define GENERATIONS 2000

void aplyGOLRules(char *grid, int row, int col, int rows, int cols, char *newgrid, int rank, int size)
{
    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            // Skip this cell
            if (i == 0 && j == 0)
                continue;

            // Takes care of cells on the edges
            int r = row + i;
            if (r < 0)
                r = rows - 1;
            if (r >= rows)
                r = 0;
            int c = col + j;
            if (c < 0)
                c = cols - 1;
            if (c >= cols)
                c = 0;
            if (grid[r * cols + c] == 1)
                count++;
        }
    }

    // Apply the rules
    // Alive cell
    if (grid[row * cols + col] == 1)
    {
        if (count == 2 || count == 3)
            // Cell remains alive
            newgrid[row * cols + col] = 1;
        else
            // Cell dies
            newgrid[row * cols + col] = 0;
    }
    // Dead cell
    else
    {
        if (count == 3)
            // Cell becomes alive
            newgrid[row * cols + col] = 1;
        else
            // Cell remains dead
            newgrid[row * cols + col] = 0;
    }
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
    int rank, size;
    double total_mpi_time = 0.0;

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

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Divide the grid into sub-grids
    int rows_per_proc = SIZE / size;
    int cols_per_proc = SIZE;
    int send_counts[size], send_displs[size];

    for (int i = 0; i < size; i++)
    {
        send_counts[i] = rows_per_proc * cols_per_proc;
        send_displs[i] = i * rows_per_proc * cols_per_proc;
    }

    // Allocate memory for the sub-grid
    char *sub_matrix = (char *)malloc(rows_per_proc * cols_per_proc * sizeof(char));

    // Start time count
    total_mpi_time -= MPI_Wtime();

    for (i = 0; i < GENERATIONS; i++)
    {
        // Scatter the grid to all processes from the root process
        MPI_Scatterv(grid, send_counts, send_displs, MPI_CHAR, sub_matrix, rows_per_proc * cols_per_proc, MPI_CHAR, 0, MPI_COMM_WORLD);

        // Calculate the new generation for the sub-grid
        for (int row = 0; row < rows_per_proc; row++)
        {
            for (int col = 0; col < cols_per_proc; col++)
            {
                aplyGOLRules(sub_matrix, row, col, rows_per_proc, cols_per_proc, &newgrid[0][0], rank, size);
            }
        }

        // Gather the new sub-grids from all processes
        MPI_Gatherv(newgrid, rows_per_proc * cols_per_proc, MPI_CHAR, grid, send_counts, send_displs, MPI_CHAR, 0, MPI_COMM_WORLD);
    }

    // Stop time count
    total_mpi_time += MPI_Wtime();

    // Print final information
    if (rank == 0)
    {
        printf("Alive: %d \n", countAliveCells(grid));
        printf("MPI time with %d processes = %lf\n", size, total_mpi_time);
    }

    free(sub_matrix);
    MPI_Finalize();
    return 0;
}