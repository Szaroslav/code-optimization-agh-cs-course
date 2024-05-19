#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define IDX(i, j, n) (((j) + (i) * (n)))

int chol(double *A, unsigned int n) {
    register int i, j, k;
    register double temp;

    for (j = 0; j < n; j++) {
        for (i = j; i < n; i++) {
            temp = A[IDX(i, j, n)];
            for (k = 0; k < j; k++) {
                temp -= A[IDX(i, k, n)] * A[IDX(j, k, n)];
            }
            A[IDX(i, j, n)] = temp;
        }

        if (A[IDX(j, j, n)] < 0.0) {
            return 1;
        }

        temp = sqrt(A[IDX(j, j, n)]);
        for (i = j + 1; i < n; i++) {
            A[IDX(i, j, n)] /= temp;
        }
    }

    return 0;
}
