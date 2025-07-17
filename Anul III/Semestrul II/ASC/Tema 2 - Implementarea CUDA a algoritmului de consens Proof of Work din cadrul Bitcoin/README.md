LINK: https://ocw.cs.pub.ro/courses/asc/teme/tema2

Nume: Vaideanu Renata
Grupă:332 CD

# Tema 2

# Description:
This program utilizes CUDA to search for a nonce that satisfies a given difficulty level in a blockchain block.
The function that handles the whole process, findNonce, is designed to search for a nonce within a specified range. 
It takes several parameters:

* block_content: A pointer to the content of the block.
* current_length: The current length of the block content.
* block_hash: A pointer to store the hash value of the block.
* nonce: A pointer to store the found nonce.
* difficulty: A pointer to the difficulty level.
* found_nonce: A pointer to a boolean value indicating whether a valid nonce has been found.

The function operates in parallel using CUDA, with each thread responsible for searching a specific nonce range. It calculates the range of nonces to be searched based on the thread index and the total number of threads. Within each thread, it iterates through the nonce range, generating nonce values, combining them with the block content, and computing their hash values. If a hash value satisfies the specified difficulty level, the corresponding nonce is stored, along with the block hash, and the `found_nonce` flag is updated to indicate success. The function then exits the loop, having completed its task.
 
## Note: 
During implementation, it was observed that the program performs optimally with the following configurations:
 * Number of blocks in the grid: 1 << 8
 * Size of each block: 1 << 9
 * Number of cores: 1 << 17

This way, the program can fully leverage the parallel processing capabilities of the GPU, resulting in optimal
performance, which involves intense computational workloads.