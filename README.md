# MPI Parallel Programs

This repository contains two MPI-based parallel C programs for High-Performance Computing (HPC) coursework:

1. **Problem 1: Counting Primes**  
   Count the number of prime numbers in a user-specified range \[x, y\] using two variants:
   - **Method (a)**: `MPI_Bcast` + `MPI_Reduce`  
   - **Method (b)**: `MPI_Send` + `MPI_Recv`

2. **Problem 2: Matrix Summation**  
   Compute the element-wise sum of two N×M matrices A and B in parallel, using `MPI_Scatter` and `MPI_Gather`.

---

## 📁 Repository Structure

```
.
├── problem1/
│   ├── primes_bcast_reduce.c     # Solution using MPI_Bcast & MPI_Reduce
│   ├── primes_bcast_reduce       # Executable
│   ├── primes_send_recv.c        # Solution using MPI_Send & MPI_Recv
│   └── primes_send_recv          # Executable
│
├── problem2/
│   ├── matrix_sum_scatter_gather.c  # Solution using MPI_Scatter & MPI_Gather
│   └── matrix_sum_scatter_gather    # Executable
│
└── README.md                      # (you are here)
```  

---

## Prerequisites

- MPI implementation (e.g., Open MPI or MPICH)  
- C compiler (`mpicc`)  
- Unix-like shell (Linux/macOS)

---

## 🧠 Problem 1: Counting Primes

Count the prime numbers within a user-defined range using:

- **Method A:** `MPI_Bcast` and `MPI_Reduce` only.
- **Method B:** `MPI_Send` and `MPI_Recv` only.

### 🔧 Compile & Run

```bash
# Compile and run the Bcast/Reduce version
cd 'Problem 1'
mpicc primes_bcast_reduce.c -o primes_bcast_reduce -lm
mpirun -np 4 ./primes_bcast_reduce
```

**Sample Output:**
```
Enter first Number: 1
Enter second Number: 16
The number of prime numbers between 1 and 16 is: 6
```

```bash
# Compile and run the Send/Recv version
mpicc primes_send_recv.c -o primes_send_recv -lm
mpirun -np 4 ./primes_send_recv
```

**Sample Output:**
```
Enter first Number: 1
Enter second Number: 16
Process 1 found 3 prime numbers between 1 and 5
Process 2 found 1 prime numbers between 6 and 10
Process 3 found 2 prime numbers between 11 and 16
Total prime numbers between 1 and 16 are: 6
```

---

## 🧮 Problem 2: Matrix Summation

Sum two user-defined matrices using `MPI_Scatter` and `MPI_Gather`.

### 🔧 Compile & Run

```bash
cd ../'Problem 2'
mpicc matrix_sum_scatter_gather.c -o matrix_sum_scatter_gather
mpirun -np 4 ./matrix_sum_scatter_gather
```

**Sample Output:**
```
Enter the size of the matrices (n m): 2 2
Enter the elements of matrix A (4 values):
1 2 3 4
Enter the elements of matrix B (4 values):
1 2 3 4
The sum matrix C is:
2 4 
6 8
```

---

## 📌 Notes

- All programs are written in **C using MPI**.
- Compilation uses `-lm` for linking the math library in Problem 1.
- Programs are tested using 4 MPI processes.
- Both problems assume the work divides evenly across processes.  
- In Problem 2, **`n*m` must be divisible by the number of processes**. Otherwise, the program will abort with an error.
- Ensure your MPI environment variables (e.g., `PATH` and `LD_LIBRARY_PATH`) are properly set for your MPI installation.
