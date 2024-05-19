#include <math.h>

#define IDX(i, j, n) (((j) + (i) * (n)))

/* Base */
int base_chol(double *A, unsigned int n) {
    int i, j, k;

    for (j = 0; j < n; j++) {
        for (i = j; i < n; i++) {
            for (k = 0; k < j; k++) {
                A[IDX(i, j, n)] -= A[IDX(i, k, n)] * A[IDX(j, k, n)];
            }
        }

        if (A[IDX(j, j, n)] < 0.0) {
            return 1;
        }

        A[IDX(j, j, n)] = sqrt(A[IDX(j, j, n)]);
        for (i = j + 1; i < n; i++) {
            A[IDX(i, j, n)] /= A[IDX(j, j, n)];
        }
    }

    return 0;
}
