#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char** argv){
    int rank, numprocs;
    int n, m;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    if (rank == 0) {
        // 1) Master reads dimensions
        printf("Enter the size of the matrices (n m): ");
        fflush(stdout);
        scanf("%d %d", &n, &m);
    }

    // 2) Broadcast n and m to all ranks
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // 3) Compute how many elements each rank will handle
    int total = n * m;
    if (total % numprocs != 0) {
        if (rank == 0)
            fprintf(stderr, "ERROR: n*m must be divisible by number of processes\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    int chunk = total / numprocs;

    // 4) Allocate flattened arrays
    int *A_flat = NULL, *B_flat = NULL, *C_flat = NULL;
    if (rank == 0) {
        A_flat = malloc(sizeof(int) * total);
        B_flat = malloc(sizeof(int) * total);
        C_flat = malloc(sizeof(int) * total);

        // read A
        printf("Enter the elements of matrix A (%d values):\n", total);
        fflush(stdout);
        for (int i = 0; i < total; i++)
            scanf("%d", &A_flat[i]);

        // read B
        printf("Enter the elements of matrix B (%d values):\n", total);
        fflush(stdout);
        for (int i = 0; i < total; i++)
            scanf("%d", &B_flat[i]);
    }

    // 5) Allocate local buffers on every rank
    int *A_local = malloc(sizeof(int) * chunk);
    int *B_local = malloc(sizeof(int) * chunk);
    int *C_local = malloc(sizeof(int) * chunk);

    // 6) Scatter A and B
    MPI_Scatter(A_flat, chunk, MPI_INT, A_local, chunk, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B_flat, chunk, MPI_INT, B_local, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    // 7) Local sum
    for (int i = 0; i < chunk; i++)
        C_local[i] = A_local[i] + B_local[i];

    // 8) Gather into C_flat on rank 0
    MPI_Gather(C_local, chunk, MPI_INT, C_flat, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // 9) Print the result in 2D form
        printf("The sum matrix C is:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++)
                printf("%d ", C_flat[i*m + j]);
            printf("\n");
        }

        free(A_flat);
        free(B_flat);
        free(C_flat);
    }

    free(A_local);
    free(B_local);
    free(C_local);

    MPI_Finalize();
    return 0;
}


// Compile with: mpicc matrix_sum_scatter_gather.c -o matrix_sum_scatter_gather
// Run with: mpirun -np <number_of_processes> ./matrix_sum_scatter_gather