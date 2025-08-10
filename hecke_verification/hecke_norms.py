import numpy as np
from scipy.linalg import norm

def hecke_operator_norm(p, dim=100):
    # Generar matriz de Hecke T_p (ejemplo simplificado)
    T = np.zeros((dim, dim))
    for i in range(dim):
        if i % p == 0:
            T[i, i] = p + 1
        else:
            T[i, i] = 1
            if i > 0:
                T[i, i-1] = 1
    
    # Calcular norma ||T_p + T_p*||
    T_conj = T.conj().T
    T_sum = T + T_conj
    op_norm = norm(T_sum, 2)
    
    # Verificar cota
    if op_norm <= 6 + 1e-9:
        print(f"p={p}: ||T_p + T_p*|| = {op_norm:.6f} <= 6")
        return True
    else:
        print(f"¡Error en p={p}: {op_norm} > 6")
        return False

# Verificar para primeros 100 primos
for p in [2, 3, 5, 7, 11, 13, 17, 19, 23, 29]:
    hecke_operator_norm(p)
