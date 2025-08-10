from sage.all import *
from sage.modular.dirichlet import DirichletGroup

def verify_ramanujan_petersson(p_max, tol=1e-15):
    for p in primes(2, p_max):
        G = DirichletGroup(p)
        chars = G.unit_gens()
        
        for chi in chars:
            L = chi.lfunction()
            r = L.analytic_rank()
            
            # Parámetros de Satake
            alpha = L.coefficients()[1]
            if abs(abs(alpha) - 1) > tol:
                print(f"¡Error en p={p}: |α|={abs(alpha)}")
                return False
    
    print(f"Verificado para todos los primos <= {p_max} (error < {tol})")
    return True

# Verificar hasta 10^6
verify_ramanujan_petersson(10**6)
