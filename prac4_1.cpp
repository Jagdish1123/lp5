#include <iostream>
#include <cuda_runtime.h>

__global__ void vectorAdd(int *a, int *b, int *c, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {
        c[i] = a[i] + b[i];
    }
}

int main() {
    int n = 10;
    int size = n * sizeof(int);
    int a[n], b[n], c[n];
    for (int i = 0; i < n; i++) {
        a[i] = i;
        b[i] = 2 * i;
    }
    int *d_a, *d_b, *d_c;
    cudaMalloc(&d_a, size); //cudaMalloc requests memory on the GPU
    cudaMalloc(&d_b, size);
    cudaMalloc(&d_c, size);
    cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice); //copies data from host (CPU) to device (GPU)
    cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);
    vectorAdd<<<1, n>>>(d_a, d_b, d_c, n); //launches the kernel
    cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost); //copies data from device (GPU) to host (CPU)
    std::cout << "Result:\n";
    for (int i = 0; i < n; i++)
        std::cout << c[i] << " ";
    cudaFree(d_a); //frees memory on the GPU
    cudaFree(d_b);
    cudaFree(d_c);
    return 0;


    //Flow of program
    //1. Allocate memory on CPU
    //2. Allocate memory on GPU
    //3. Copy data from CPU to GPU
    //4. Launch kernel
    //5. Copy data from GPU to CPU
    //6. Free memory on GPU



}