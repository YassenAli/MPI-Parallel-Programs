#include "mpi.h"
#include <stdio.h>
#include <stdbool.h>  
#include <math.h> 

bool isPrime(int num) {
    if (num <= 1) {
        return false;  
    }

    for (int i = 2; i <= sqrt(num); i++) {
        if (num % i == 0) {
            return false;  
        }
    }

    return true;  
}

int main(int argc, char** argv) {
    // Initialize the MPI environment
    int x, y, r;
    MPI_Init(&argc, &argv);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if(world_size > 1){
        if(world_rank == 0) {
         // master process 
         printf("Enter first Number: ");
         fflush(stdout);
         scanf("%d", &x);
         printf("Enter second Number: ");
         fflush(stdout);
         scanf("%d", &y);

         if(x>y){
            printf ("X is larger than Y");
            fflush(stdout);
            MPI_Finalize();
            return 0;
         }
         int numbersCount = y-x;
         r = numbersCount/(world_size-1);
        }
        MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&y, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&r, 1, MPI_INT, 0, MPI_COMM_WORLD);
        int start = x + (world_rank - 1) * r;
        int end = start + r-1;
        if(world_rank == world_size - 1){
            end = y;
        }
        int counter =0;
        for(int i = start; i<= end; i++){
            if(isPrime(i)){
             counter++;
            }
        }
        int result =0;
        MPI_Reduce(&counter, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        if(world_rank == 0){
            printf("The number of prime numbers between %d and %d is: %d\n", x, y, result);
        }
    }else{
        printf("rerun with more than one process");
    }

    
    MPI_Finalize();
}


// Compile with: mpicc -o primes_bcast_reduce primes_bcast_reduce.c -lm
// Run with: mpirun -np <number_of_processes> ./primes_bcast_reduce