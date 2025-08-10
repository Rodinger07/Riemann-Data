#include <cuda_runtime.h>
#include <fplll.h>

extern "C" void lll_reduction(double** basis, int n, int m, double** reduced_basis) {
    ZZ_mat<mpz_t> matrix(n, m);
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrix[i][j] = basis[i][j] * 1e15;
        }
    }
    
    lll_reduction(matrix, LLL_DEF_DELTA, LLL_DEF_ETA, LM_WRAPPER, FT_DEFAULT, 0, LLL_DEFAULT);
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            reduced_basis[i][j] = to_double(matrix[i][j]) / 1e15;
        }
    }
}
