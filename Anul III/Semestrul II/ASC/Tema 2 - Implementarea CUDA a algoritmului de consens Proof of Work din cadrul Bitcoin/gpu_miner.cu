#include <stdio.h>
#include <stdint.h>
#include "../include/utils.cuh"
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#define GL_BLOCKS_NO (1 << 8)         // Number of blocks in the grid
#define GL_BLOCK_SIZE (1 << 9)        // Size of each block
#define GL_CORES_NO (1 << 17)         // Number of cores

__global__ void findNonce(BYTE* block_content, size_t current_length, BYTE* block_hash, uint64_t* nonce, BYTE* difficulty, bool *found_nonce) {
	// Thread index calculation
    unsigned int thread_id = blockIdx.x * blockDim.x + threadIdx.x;
	// Nonce range for current thread
    unsigned int start = thread_id * (double)MAX_NONCE / GL_CORES_NO;
    unsigned int end = min((thread_id + 1) * (double)MAX_NONCE / GL_CORES_NO, MAX_NONCE);

    // Local variables
    BYTE device_block_hash[SHA256_HASH_SIZE];
    BYTE local_content[BLOCK_SIZE];

    // Starting content block
    d_strcpy((char*)local_content, (const char*)block_content);

	// Search for a valid nonce in the assigned range
    for (unsigned int crt_nonce = start; crt_nonce < end; crt_nonce++) {
		if (*found_nonce)
        	break;

    	char nonce_string[NONCE_SIZE];
        // Convert nonce to string
        intToString(crt_nonce, nonce_string);

        // Concatenate current nonce
        d_strcpy((char*)local_content + current_length, (const char*)nonce_string);

        // Apply hash
        apply_sha256(local_content, d_strlen((const char*)local_content), device_block_hash, 1);

        if (compare_hashes(device_block_hash, difficulty) <= 0) {
            // Store the found nonce
            *nonce = crt_nonce;

            // Store block_hash
            d_strcpy((char*)block_hash, (const char*)device_block_hash);

			// Update found_nonce flag
            *found_nonce = true;

			// Exit the loop if a valid nonce is found
            break; 
        }
    }
}

int main(int argc, char **argv) {
	BYTE hashed_tx1[SHA256_HASH_SIZE], hashed_tx2[SHA256_HASH_SIZE], hashed_tx3[SHA256_HASH_SIZE], hashed_tx4[SHA256_HASH_SIZE],
			tx12[SHA256_HASH_SIZE * 2], tx34[SHA256_HASH_SIZE * 2], hashed_tx12[SHA256_HASH_SIZE], hashed_tx34[SHA256_HASH_SIZE],
			tx1234[SHA256_HASH_SIZE * 2], top_hash[SHA256_HASH_SIZE], block_content[BLOCK_SIZE];
	BYTE block_hash[SHA256_HASH_SIZE] = "0000000000000000000000000000000000000000000000000000000000000000"; // TODO: Update
	uint64_t nonce = 0; // TODO: Update
	size_t current_length;

	// Top hash
	apply_sha256(tx1, strlen((const char*)tx1), hashed_tx1, 1);
	apply_sha256(tx2, strlen((const char*)tx2), hashed_tx2, 1);
	apply_sha256(tx3, strlen((const char*)tx3), hashed_tx3, 1);
	apply_sha256(tx4, strlen((const char*)tx4), hashed_tx4, 1);
	strcpy((char *)tx12, (const char *)hashed_tx1);
	strcat((char *)tx12, (const char *)hashed_tx2);
	apply_sha256(tx12, strlen((const char*)tx12), hashed_tx12, 1);
	strcpy((char *)tx34, (const char *)hashed_tx3);
	strcat((char *)tx34, (const char *)hashed_tx4);
	apply_sha256(tx34, strlen((const char*)tx34), hashed_tx34, 1);
	strcpy((char *)tx1234, (const char *)hashed_tx12);
	strcat((char *)tx1234, (const char *)hashed_tx34);
	apply_sha256(tx1234, strlen((const char*)tx34), top_hash, 1);

	// prev_block_hash + top_hash
	strcpy((char*)block_content, (const char*)prev_block_hash);
	strcat((char*)block_content, (const char*)top_hash);
	current_length = strlen((char*) block_content);

	// Allocate memory and copy data on device
	BYTE *device_block_content;
	cudaMalloc(&device_block_content, sizeof(BYTE) * BLOCK_SIZE);
    cudaMemcpy(device_block_content, block_content, sizeof(BYTE) * BLOCK_SIZE, cudaMemcpyHostToDevice);

	BYTE *device_block_hash;
    cudaMalloc(&device_block_hash, sizeof(BYTE) * SHA256_HASH_SIZE);
    cudaMemcpy(device_block_hash, block_hash, sizeof(BYTE) * SHA256_HASH_SIZE, cudaMemcpyHostToDevice);

    uint64_t *device_nonce;
    cudaMalloc(&device_nonce, sizeof(uint64_t));
    cudaMemcpy(device_nonce, &nonce, sizeof(uint64_t), cudaMemcpyHostToDevice);

	BYTE *device_difficulty;
	cudaMalloc(&device_difficulty, sizeof(BYTE) * SHA256_HASH_SIZE);
	cudaMemcpy(device_difficulty, difficulty_5_zeros, sizeof(BYTE) * SHA256_HASH_SIZE, cudaMemcpyHostToDevice);

	// Initialize device found_nonce flag
	bool found_nonce = false;
	bool *device_found_nonce;
    cudaMalloc(&device_found_nonce, sizeof(bool));
    cudaMemcpy(device_found_nonce, &found_nonce, sizeof(bool), cudaMemcpyHostToDevice);

	cudaEvent_t start, stop;
	startTiming(&start, &stop);

	findNonce<<<GL_BLOCKS_NO, GL_BLOCK_SIZE>>>(device_block_content, current_length, device_block_hash, device_nonce, device_difficulty, device_found_nonce);

	float seconds = stopTiming(&start, &stop);

	// Copy results back from device
	cudaMemcpy(&nonce, device_nonce, sizeof(uint64_t), cudaMemcpyDeviceToHost);
    cudaMemcpy(block_hash, device_block_hash, sizeof(BYTE) * SHA256_HASH_SIZE, cudaMemcpyDeviceToHost);

	printResult(block_hash, nonce, seconds);

	// Cleanup
    cudaFree(device_block_content);
    cudaFree(device_block_hash);
	cudaFree(device_difficulty);
    cudaFree(device_nonce);
	cudaFree(device_found_nonce);

	return 0;
}
