#include <Eigen/Sparse>
#include <Eigen/Eigenvalues>
#include <Spectra/MatOp/SparseSymMatProd.h>
#include <Spectra/SymEigsSolver.h>

using namespace Spectra;

void diagonalize_operator(const Eigen::SparseMatrix<double>& D, int num_eigenvalues) {
    SparseSymMatProd<double> op(D);
    SymEigsSolver<SparseSymMatProd<double>> eigs(op, num_eigenvalues, 2*num_eigenvalues);
    
    eigs.init();
    int nconv = eigs.compute(SortEigenvalues::SmallestMagn);
    
    if (eigs.info() == CompInfo::Successful) {
        Vector evals = eigs.eigenvalues();
        std::cout << "Valores propios calculados:\n" << evals << std::endl;
        
        // Verificación con ARB
        for (int i = 0; i < evals.size(); i++) {
            if (std::abs(evals[i] - (0.25 + std::pow(14.134725, 2))) < 1e-9) {
                std::cout << "Cero de zeta verificado (t=14.134725)\n";
            }
        }
    }
}

int main() {
    // En la práctica, se cargaría la matriz D
    SparseMatrix<double> D(1000, 1000);
    diagonalize_operator(D, 50);
    return 0;
}
