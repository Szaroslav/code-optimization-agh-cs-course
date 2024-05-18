#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define IDX(i, j, n) (((j) + (i) * (n)))
#define MAX(a, b) (a > b ? a : b)

int chol(double * A, unsigned int n) {
    register int i, j, k;
    register double temp;

    const int BLOCK_SIZE = 8;

    for (j = 0; j < n; j++) {
        for (i = j; i < n; i++) {
            temp = A[IDX(i, j, n)];
            for (k = 0; k < j;) {
                if (k < MAX(j - BLOCK_SIZE, 0)) {
                    temp -= A[IDX(i, k,     n)] * A[IDX(j, k,     n)];
                    temp -= A[IDX(i, k + 1, n)] * A[IDX(j, k + 1, n)];
                    temp -= A[IDX(i, k + 2, n)] * A[IDX(j, k + 2, n)];
                    temp -= A[IDX(i, k + 3, n)] * A[IDX(j, k + 3, n)];
                    temp -= A[IDX(i, k + 4, n)] * A[IDX(j, k + 4, n)];
                    temp -= A[IDX(i, k + 5, n)] * A[IDX(j, k + 5, n)];
                    temp -= A[IDX(i, k + 6, n)] * A[IDX(j, k + 6, n)];
                    temp -= A[IDX(i, k + 7, n)] * A[IDX(j, k + 7, n)];
                    k += BLOCK_SIZE;
                }
                else {
                    temp -= A[IDX(i, k, n)] * A[IDX(j, k, n)];
                    k++;
                }
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
