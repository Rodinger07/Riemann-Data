#include <cuda_runtime.h>
#include <cufft.h>

__global__ void compute_exponential_sums(cufftDoubleComplex* result, const double* t_values, int n, double* reduced_basis, int basis_size) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < n) {
        double t = t_values[idx];
        double real_sum = 0.0;
        double imag_sum = 0.0;
        
        for (int k = 0; k < basis_size; k++) {
            double phase = t * reduced_basis[k];
            real_sum += cos(phase);
            imag_sum += sin(phase);
        }
        
        result[idx].x = real_sum;
        result[idx].y = imag_sum;
    }
}

void gpu_exponential_sums(const double* t_values, int n, double* reduced_basis, int basis_size, cufftDoubleComplex* result) {
    double* d_t;
    double* d_basis;
    cufftDoubleComplex* d_result;
    
    cudaMalloc(&d_t, n * sizeof(double));
    cudaMalloc(&d_basis, basis_size * sizeof(double));
    cudaMalloc(&d_result, n * sizeof(cufftDoubleComplex));
    
    cudaMemcpy(d_t, t_values, n * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_basis, reduced_basis, basis_size * sizeof(double), cudaMemcpyHostToDevice);
    
    dim3 blockDim(256);
    dim3 gridDim((n + blockDim.x - 1) / blockDim.x);
    
    compute_exponential_sums<<<gridDim, blockDim>>>(d_result, d_t, n, d_basis, basis_size);
    
    cudaMemcpy(result, d_result, n * sizeof(cufftDoubleComplex), cudaMemcpyDeviceToHost);
    
    cudaFree(d_t);
    cudaFree(d_basis);
    cudaFree(d_result);
}
