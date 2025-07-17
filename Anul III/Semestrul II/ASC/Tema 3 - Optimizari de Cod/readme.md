LINK: https://ocw.cs.pub.ro/courses/asc/teme/tema3

Nume: Vaideanu Renata
Grupă:332 CD

# Tema 3

## Description:
This project aims to implement a matrix operation defined as follows: C = (A^t * B + B * A) * B^t,
Where:
- A and B are square matrices of size N x N, containing double precision elements.
- A is an upper triangular matrix.
- A^t is the transpose of A.
- B^t is the transpose of B.
- x denotes matrix multiplication.
- \+ denotes matrix addition.

## Blas Implementation
The function initiates by allocating memory for two matrices, AtB and BA, along with the result matrix, C, required for the final calculation of A^T×B (AtB) and B×A (BA). 

The B matrix is duplicated using cblas_dcopy to preserve its original values for subsequent use in matrix multiplication. Utilizing cblas_dtrmm, AtB and BA are computed, accounting for the upper triangular nature of matrix A specified by CblasUpper. Following this, cblas_daxpy is employed to combine the two matrices, resulting in the computation of C using cblas_dgemm. The choice of cblas_dtrmm is dictated by the upper triangular property of matrix A. 

In the end, AtB and BA would be freed and C would be returned.

## Neopt Implementation
The unoptimized implementation starts the same as the previous implementation, allocating memory for AtB, BA and C. First, we're computing AtB, which involves multiplying the transpose of matrix A by matrix B. We go through each element of AtB and for each, we multiply elements from A and B, but only up to the current row.

Next, we're computing BA, the product of matrix B and matrix A. Similar to before, we go through each element of BA, but this time we multiply elements from B and A, but only up to the current column.

After that, we simply add corresponding elements of AtB and BA together.

Finally, we calculate the final result matrix C. We go through each element of C, computing dot products of rows of AtB and columns of B, and storing the result in C.

## Opt_m Implementation 
Similar to the previous two, the function starts with allocating memory for AtB, BA and C. In this implementation, I used the informations provided in the 9th lab on ocw. 

`Compute AtB = A_t * B` 
For each row of A and column of B, multiply corresponding elements up to the current row of A. Store the sum in AtB.
- register double *orig_pa: Points to the start of the current row of matrix A.
- register double *pa: Iterates through the current row of matrix A.
- register double *pb: Points to the start of the current column of matrix B.

`Compute BA = B * A`
For each row of B and column of A, multiply corresponding elements up to the current column of A. Store the sum in BA.
- register double *orig_pa: Points to the start of the current row of matrix B.
- register double *pa: Iterates through the current row of matrix B.
- register double *pb: Points to the start of the current column of matrix A.

`Combine AtB and BA`
Add corresponding elements of AtB and BA.

`Compute C = (AtB + BA) * B` using the provided pattern:
For each row of AtB and column of B, multiply corresponding elements and accumulate. Store the sum in C.
- register double *orig_pa: Points to the start of the current row of AtB.
- register double *pa: Iterates through the current row of AtB.
- register double *pb: Points to the start of the current column of B.

## Cachegrind

### blas.cache
`Cache Efficiency:`
Low miss rates in both instruction and data caches, from L1 to last-level caches, indicate effective utilization of the cache hierarchy, suggesting good spatial and temporal memory locality. This minimizes the need for accessing slower memory levels.

`Branch Prediction:`
The relatively low branch misprediction rate indicates predictable branching behavior, facilitating efficient execution flow. Further optimization opportunities may exist to reduce mispredictions even more.

`Memory Access Patterns:`
The ratio of data to instruction references offers insights into the program's memory access patterns, guiding optimizations like prefetching or reordering accesses to improve cache utilization.

`Cache Hierarchy Impact:`
Comparing L1 cache misses to last-level cache misses highlights the cache hierarchy's effectiveness. A notable difference indicates efficient service by lower-level caches, reducing the burden on higher-level caches and main memory.

`Potential Optimization Targets:`
Areas with higher cache miss rates in either instruction or data caches signal potential optimization areas. Strategies could include enhancing data locality, restructuring code to reduce instruction cache misses, or improving branch prediction accuracy.

`Overall Performance:`
Examining cache miss rates, branch misprediction rate, and total instructions executed offers a comprehensive view of performance characteristics, guiding optimizations for enhanced execution speed and resource utilization.

### neopt.cache
`Cache Miss Rates:`
Instruction Cache (I1) miss rate is 0.00%, indicating efficient instruction cache usage.
Data Cache (D1) miss rate is 2.6%, with a corresponding Last Level Data Cache (LLd) miss rate of 0.0%. This suggests good data cache performance with minimal misses.
Last Level Cache (LL) miss rate is also 0.0%, indicating efficient overall cache usage.

`Branch Prediction:`
The program executed a total of 133,493,126 branches, out of which 502,457 were mispredicted. This yields a branch misprediction rate of 0.4%, indicating relatively accurate branch prediction.

`Memory References:`
There were a total of 5,923,821,079 instruction references (I refs) and 2,964,186,532 data references (D refs), indicating a significant amount of memory access.
Data references consisted of 2,831,921,786 reads and 132,264,746 writes.

`Cache Miss Counts:`
The counts of L1 instruction (I1), L1 data (D1), and Last Level (LL) cache misses provide insights into the efficiency of cache utilization, with relatively low miss counts across all cache levels.

`Cache Sizes:`
The exact cache sizes are not provided in this output, but the miss rates and counts give an indication of how effectively the cache is being utilized relative to its size.

`Overall Performance:`
The low cache miss rates and branch misprediction rate suggest that the program is likely performing well in terms of cache utilization and branch prediction accuracy.
In summary, the program appears to have efficient cache utilization and relatively accurate branch prediction, indicating good performance in terms of memory access and branching behavior.

### opt_m.cache

`Cache Miss Rates:`
Instruction Cache (I1) miss rate remains at 0.00%, indicating efficient instruction cache utilization.
Data Cache (D1) miss rate is 10.2%, with a Last Level Data Cache (LLd) miss rate of 0.0%. This suggests relatively high data cache misses but efficient utilization of the last level cache.

`Branch Prediction:`
The program executed 133,493,356 branches, with 502,512 mispredictions. This yields a branch misprediction rate of 0.4%, consistent with the previous observation of accurate branch prediction.

`Memory References:`
Instruction references (I refs) totaled 1,832,309,131, while data references (D refs) were 757,070,131, indicating substantial memory access.

`Cache Miss Counts:`
The counts of L1 instruction (I1), L1 data (D1), and Last Level (LL) cache misses provide insights into the program's cache performance, with a relatively higher count of D1 misses compared to the previous example.

`Cache Sizes:`
Specific cache sizes are not provided in the output, but the miss rates and counts suggest that cache utilization might be less efficient compared to the previous scenario, particularly in the data cache.

`Overall Performance:`
Despite a higher data cache miss rate, the overall cache miss rates remain low, indicating effective cache utilization. The branch misprediction rate remains consistent with the previous case, indicating reliable branch prediction.
In summary, the program with optimizations enabled exhibits efficient instruction cache utilization, though experiencing a relatively higher data cache miss rate. However, the overall cache miss rates remain low, and the branch prediction accuracy is consistent with expectations.

### opt_m vs neopt
While the programs being profiled in both instances might be similar, the neopt instance appears to have much higher activity, as evidenced by the significantly larger number of instruction and data references. This could indicate that the program executed more instructions and accessed more data during its runtime. Additionally, the cache miss rates in the neopt instance are slightly higher, suggesting potentially less efficient cache utilization compared to the first instance.


## Graphics

In the end analysis, the graph depicted in performance_graphics.png illustrates the varying performances of the three algorithms. To highlight these distinctions, I opted to use matrices of different sizes (100, 200, 300, 400, 500, 600, 800, 1200). As anticipated, the unoptimized algorithm consumes the most time for the operations, particularly evident in the 1200-case scenario, where it nearly reaches a duration of 22 seconds. On the other hand, the opt_m variant exhibits a slightly improved performance, reducing the time from nearly 22 seconds to approximately 12 seconds. Ultimately, the blas variant emerges as the clear winner in terms of speed, achieving an elapsed time of less than a second even for the N = 1200 case.