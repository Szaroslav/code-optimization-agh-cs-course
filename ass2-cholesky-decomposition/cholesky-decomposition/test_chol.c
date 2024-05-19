#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "parameters.h"

int base_chol(double *, unsigned int);
int chol(double *, unsigned int);
void copy_matrix(int, int, double *, double *);
void generate_matrix(int, int, double *);
double compare_matrices(int, int, double *, double *);
void print_matrix(int m, int n, double *a);

double dclock();

int main()
{
    uint32_t p, n, rep, actual_rep;
    double
        dtime, dtime_best = 0.0, dtime_total = 0.0,
        gflo,
        diff;
    double *a, *l_base, *l;

    printf("chol = [\n");

    for (p = PFIRST; p <= PLAST; p += PINC) {
        n = (N == -1 ? p : N);
        gflo = n * n * n * 1.0e-09;

        /* Allocate space for the matrices */
        /* Note: I create an extra column in A to make sure that
        prefetching beyond the matrix does not cause a segfault */
        a = (double *) malloc(n * n * sizeof(double));
        l_base = (double *) malloc(n * n * sizeof(double));
        l = (double *) malloc(n * n * sizeof(double));

        /* Generate matrix A */
        generate_matrix(n, n, a);

        copy_matrix(n, n, a, l_base);
        copy_matrix(n, n, a, l);

        /* Run the reference implementation so the answers can be compared */
        base_chol(l_base, n);

        /* Time the "optimized" implementation */
        for (rep = 0, actual_rep = 0; rep < NREPEATS; rep++) {
            copy_matrix(n, n, a, l);

            dtime = dclock();
            if (chol(l, n) != 0) {
                continue;
            }
            dtime = dclock() - dtime;

            actual_rep++;
            dtime_total += dtime;

            if (rep == 0) {
                dtime_best = dtime;
            }
            else {
                dtime_best = (dtime < dtime_best ? dtime : dtime_best);
            }
        }

        diff = compare_matrices(n, n, l, l_base);

        if (dtime_best > 0.0) {
            printf("    %d %le %le\n", p, gflo / dtime_best, diff);
            fflush(stdout);
        }

        free(a);
        free(l_base);
        free(l);
    }

    printf("];\n");

    exit(0);
}
