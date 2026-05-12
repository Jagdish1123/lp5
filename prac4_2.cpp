#include <iostream>
#include <cuda_runtime.h>
#define N 3

__global__ void matrixMul(int A[N][N], int B[N][N], int C[N][N]) {
    int row = threadIdx.y;
    int col = threadIdx.x;
    int sum = 0;
    for (int k = 0; k < N; k++) {
        sum += A[row][k] * B[k][col];
    }
    C[row][col] = sum;
}

int main() {
    int A[N][N] = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
    int B[N][N] = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };
    int C[N][N];
    int (*d_A)[N], (*d_B)[N], (*d_C)[N];
    cudaMalloc(&d_A, sizeof(A));
    cudaMalloc(&d_B, sizeof(B));
    cudaMalloc(&d_C, sizeof(C));
    cudaMemcpy(d_A, A, sizeof(A), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, sizeof(B), cudaMemcpyHostToDevice);
    dim3 threads(N, N);
    matrixMul<<<1, threads>>>(d_A, d_B, d_C);
    cudaMemcpy(C, d_C, sizeof(C), cudaMemcpyDeviceToHost);
    std::cout << "Result Matrix:\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << C[i][j] << " ";
        }
        std::cout << std::endl;
    }
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    return 0;
}