# MPI Parallel Programs

This repository contains two MPI-based parallel C programs for High-Performance Computing (HPC) coursework:

1. **Problem 1: Counting Primes**  
   Count the number of prime numbers in a user-specified range \[x, y\] using two variants:
   - **Method (a)**: `MPI_Bcast` + `MPI_Reduce`  
   - **Method (b)**: `MPI_Send` + `MPI_Recv`

2. **Problem 2: Matrix Summation**  
   Compute the element-wise sum of two N×M matrices A and B in parallel, using `MPI_Scatter` and `MPI_Gather`.

---

## Repository Structure

```
.
├── problem1/
│   ├── primes_bcast_reduce.c     # Solution using MPI_Bcast & MPI_Reduce
│   ├── primes_send_recv.c         # Solution using MPI_Send & MPI_Recv
│   └── Makefile                   # Build targets for Problem 1
│
├── problem2/
│   ├── matrix_sum_scatter_gather.c  # Solution using MPI_Scatter & MPI_Gather
│   └── Makefile                     # Build targets for Problem 2
│
└── README.md                      # (you are here)
```  

---

## Prerequisites

- MPI implementation (e.g., Open MPI or MPICH)  
- C compiler (`mpicc`)  
- Unix-like shell (Linux/macOS)

---

## Building

Each subdirectory has its own `Makefile`. From the repository root:

```bash
# Build both problems
make -C problem1
make -C problem2
```

Or build individually:

```bash
# Problem 1
cd problem1
make primes_bcast_reduce    # target for MPI_Bcast + MPI_Reduce version
make primes_send_recv       # target for MPI_Send + MPI_Recv version

# Problem 2
cd ../problem2
make matrix_sum             # target for scatter/gather version
```  

---

## Running

### Problem 1: Counting Primes

1. **MPI_Bcast + MPI_Reduce version**  
   ```bash
   mpirun -np <P> ./primes_bcast_reduce <lower> <upper>
   ```
   - The program will prompt for:
  1. Enter first Number: `n m`
  2. Enter second Number: values of matrix A

   - `<P>`: total number of processes  
   - `<lower>`: lower bound `(x)`  
   - `<upper>`: upper bound `(y)`

2. **MPI_Send + MPI_Recv version**  
   ```bash
   mpirun -np <P> ./primes_send_recv <lower> <upper>
   ```

Both programs will print:
```
Total primes between x and y: <count>
```

### Problem 2: Matrix Summation

```bash
mpirun -np <P> ./matrix_sum
```

- The master (rank 0) will prompt for:
  1. Matrix dimensions: `n m`
  2. `n*m` values of matrix A
  3. `n*m` values of matrix B

The final summed matrix C = A + B is printed by rank 0.

---

## Notes

- Both problems assume the work divides evenly across processes.  
- In Problem 2, **`n*m` must be divisible by the number of processes**. Otherwise, the program will abort with an error.
- Ensure your MPI environment variables (e.g., `PATH` and `LD_LIBRARY_PATH`) are properly set for your MPI installation.
