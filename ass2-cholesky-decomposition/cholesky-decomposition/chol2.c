#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define IDX(i, j, n) (((j) + (i) * (n)))

void subtract(int j, double *A, unsigned int n) {
    for (int i = j; i < n; i++) {
        for (int k = 0; k < j; k++) {
            A[IDX(i, j, n)] -= A[IDX(i, k, n)] * A[IDX(j, k, n)];
        }
    }
}

int chol(double *A, unsigned int n) {
    int i, j;

    for (j = 0; j < n; j++) {
        subtract(j, A, n);

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
