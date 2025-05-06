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
    // Compute how many rows each rank gets
    int base_rows   = n / numprocs;          // evenly‐divided part
    int remainder   = n % numprocs;          // extra rows to distribute
    // build the scatter/gather counts (in ELEMENTS, not rows)
    int *sendcounts = malloc(sizeof(int)*numprocs);
    int *displs     = malloc(sizeof(int)*numprocs);
    int offset = 0;
    for (int i = 0; i < numprocs; ++i) {
        int rows_i = base_rows + (i < remainder ? 1 : 0);
        sendcounts[i] = rows_i * m;          // each row has m elements
        displs[i]     = offset;
        offset      += sendcounts[i];
    }

    // 4) Allocate flattened arrays
    int total = n * m;
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
    int local_count = sendcounts[rank];
    int *A_local = malloc(sizeof(int) * local_count);
    int *B_local = malloc(sizeof(int) * local_count);
    int *C_local = malloc(sizeof(int) * local_count);

    // 6) Scatter A and B
    MPI_Scatterv(A_flat, sendcounts, displs, MPI_INT, A_local, local_count, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(B_flat, sendcounts, displs, MPI_INT, B_local, local_count, MPI_INT, 0, MPI_COMM_WORLD);

    // 7) Local sum
    for (int i = 0; i < local_count; i++)
        C_local[i] = A_local[i] + B_local[i];

    // 8) Gather into C_flat on rank 0
    MPI_Gatherv(C_local, local_count, MPI_INT, C_flat, sendcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

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
    free(sendcounts);
    free(displs);

    MPI_Finalize();
    return 0;
}


// Compile with: mpicc matrix_sum_scatter_gather.c -o matrix_sum_scatter_gather
// Run with: mpirun -np <number_of_processes> ./matrix_sum_scatter_gather