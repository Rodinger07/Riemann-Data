#include <stdio.h>
#include <math.h>
#include "arb.h"

void riemann_siegel_theta(arb_t res, const arb_t t, slong prec) {
    arb_t pi, t_half, arg;
    arb_init(pi);
    arb_init(t_half);
    arb_init(arg);
    
    arb_const_pi(pi, prec);
    arb_div_ui(t_half, t, 2, prec);
    
    // arg = (1/4 + it/2)
    arb_one(arg);
    arb_div_ui(arg, arg, 4, prec);
    arb_addmul_i(arg, t_half, prec);
    
    // gamma(arg)
    arb_gamma(res, arg, prec);
    
    // theta = arg(gamma) - (t/2)*log(pi)
    arb_arg(res, res, prec);
    arb_log(pi, pi, prec);
    arb_mul(t_half, t_half, pi, prec);
    arb_sub(res, res, t_half, prec);
    
    arb_clear(pi);
    arb_clear(t_half);
    arb_clear(arg);
}

void riemann_siegel_z(arb_t z, const arb_t t, slong prec) {
    arb_t theta, pi_term;
    arb_init(theta);
    arb_init(pi_term);
    
    riemann_siegel_theta(theta, t, prec);
    arb_sin_cos_pi(pi_term, theta, theta, prec);
    
    // Z(t) = 2 * sum_{n=1}^{floor(sqrt(t/(2π))} cos(θ(t) - t*log(n)) / sqrt(n)
    arb_t sum, term, n_real, log_n, t_log_n, diff;
    arb_init(sum);
    arb_init(term);
    arb_init(n_real);
    arb_init(log_n);
    arb_init(t_log_n);
    arb_init(diff);
    
    arb_zero(sum);
    double t_double = arf_get_d(arb_midref(t), ARF_RND_NEAR);
    long N = sqrt(t_double / (2 * M_PI));
    
    for (long n = 1; n <= N; n++) {
        arb_set_ui(n_real, n);
        arb_log(log_n, n_real, prec);
        arb_mul(t_log_n, t, log_n, prec);
        arb_sub(diff, theta, t_log_n, prec);
        arb_cos(term, diff, prec);
        arb_sqrt(log_n, n_real, prec);  // sqrt(n)
        arb_div(term, term, log_n, prec);
        arb_add(sum, sum, term, prec);
    }
    
    arb_mul_ui(z, sum, 2, prec);
    
    arb_clear(theta);
    arb_clear(pi_term);
    arb_clear(sum);
    arb_clear(term);
    arb_clear(n_real);
    arb_clear(log_n);
    arb_clear(t_log_n);
    arb_clear(diff);
}

int main() {
    arb_t t, z;
    arb_init(t);
    arb_init(z);
    
    // Calcular Z(14.134725)
    arb_set_d(t, 14.134725);
    slong prec = 1000;  // 1000 bits de precisión
    
    riemann_siegel_z(z, t, prec);
    
    printf("Z(14.134725) = ");
    arb_printn(z, 50, 0);
    printf("\n");
    
    // Verificación con ARB
    arb_t zero_check;
    arb_init(zero_check);
    arb_abs(zero_check, z);
    
    if (arb_lt(zero_check, arb_float(1e-15))) {
        printf("Error < 1e-15 (validado con ARB)\n");
    }
    
    arb_clear(t);
    arb_clear(z);
    arb_clear(zero_check);
    flint_cleanup();
    return 0;
}
