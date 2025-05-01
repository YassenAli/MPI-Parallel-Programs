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
         int number = x;
         int i=1;
         for( ;i< world_size-1; i++){
            MPI_Send(&number, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            int temp = number + r - 1;
            MPI_Send(&temp, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            number += r;
         }

         MPI_Send(&number, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
         MPI_Send(&y, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

         int counter = 0;
         for(int i=1; i< world_size; i++){
            int temp;
            MPI_Recv(&temp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            counter += temp;
         }
            printf("Total prime numbers between %d and %d are: %d\n", x, y, counter);
            fflush(stdout);

        } else {
            int x,y, counter;

            MPI_Recv(&x,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            MPI_Recv(&y,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

            for(int i =x; i<=y ; i++){
                if(isPrime(i)){
                    counter++;
                }
            }

            MPI_Send(&counter, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            printf("Process %d found %d prime numbers between %d and %d\n", world_rank, counter, x, y);
        }
    }else{
        printf("rerun with more than one process");
    }

    
    MPI_Finalize();
}


// Compile with: mpicc -o primes_send_recv primes_send_recv.c -lm
// Run with: mpirun -np <number_of_processes> ./primes_send_recv