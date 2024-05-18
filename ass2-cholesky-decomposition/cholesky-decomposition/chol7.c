#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <arm_neon.h>

#define IDX(i, j, n) (((j) + (i) * (n)))
#define MAX(a, b) (a > b ? a : b)

typedef float64x2_t __m128d;
#define _mm_loadu_pd(a) vld1q_f64(a)
#define _mm_mul_pd(a, b) vmulq_f64(a, b)
#define _mm_add_pd(a, b) vaddq_f64(a, b)

int chol(double * A, unsigned int n) {
    const int BLOCK_SIZE = 16;

    register int i, j, k;
    register double temp;
    register __m128d temp0,
                     temp1,
                     temp2,
                     temp3,
                     temp4,
                     temp5,
                     temp6,
                     temp7,
                     temp8,
                     temp9,
                     temp10,
                     temp11,
                     temp12,
                     temp13,
                     temp14,
                     temp15;

    for (j = 0; j < n; j++) {
        for (i = j; i < n; i++) {
            temp = A[IDX(i, j, n)];

            for (k = 0; k < j;) {
                if (k < MAX(j - BLOCK_SIZE, 0)) {
                    temp0  = _mm_loadu_pd(&A[IDX(i, k,      n)]);
                    temp1  = _mm_loadu_pd(&A[IDX(j, k,      n)]);
                    temp2  = _mm_loadu_pd(&A[IDX(i, k + 2,  n)]);
                    temp3  = _mm_loadu_pd(&A[IDX(j, k + 2,  n)]);
                    temp4  = _mm_loadu_pd(&A[IDX(i, k + 4,  n)]);
                    temp5  = _mm_loadu_pd(&A[IDX(j, k + 4,  n)]);
                    temp6  = _mm_loadu_pd(&A[IDX(i, k + 6,  n)]);
                    temp7  = _mm_loadu_pd(&A[IDX(j, k + 6,  n)]);
                    temp8  = _mm_loadu_pd(&A[IDX(i, k + 8,  n)]);
                    temp9  = _mm_loadu_pd(&A[IDX(j, k + 8,  n)]);
                    temp10 = _mm_loadu_pd(&A[IDX(i, k + 10, n)]);
                    temp11 = _mm_loadu_pd(&A[IDX(j, k + 10, n)]);
                    temp12 = _mm_loadu_pd(&A[IDX(i, k + 12, n)]);
                    temp13 = _mm_loadu_pd(&A[IDX(j, k + 12, n)]);
                    temp14 = _mm_loadu_pd(&A[IDX(i, k + 14, n)]);
                    temp15 = _mm_loadu_pd(&A[IDX(j, k + 14, n)]);

                    temp0   = _mm_mul_pd(temp0,  temp1 );
                    temp2   = _mm_mul_pd(temp2,  temp3 );
                    temp4   = _mm_mul_pd(temp4,  temp5 );
                    temp6   = _mm_mul_pd(temp6,  temp7 );
                    temp8   = _mm_mul_pd(temp8,  temp9 );
                    temp10  = _mm_mul_pd(temp10, temp11);
                    temp12  = _mm_mul_pd(temp12, temp13);
                    temp14  = _mm_mul_pd(temp14, temp15);
                    temp0   = _mm_add_pd(temp0,  temp2 );
                    temp2   = _mm_add_pd(temp2,  temp4 );
                    temp4   = _mm_add_pd(temp4,  temp6 );
                    temp6   = _mm_add_pd(temp6,  temp8 );
                    temp8   = _mm_add_pd(temp8,  temp10);
                    temp10  = _mm_add_pd(temp10, temp12);
                    temp12  = _mm_add_pd(temp12, temp14);
                    temp0   = _mm_add_pd(temp0,  temp2 );
                    temp0   = _mm_add_pd(temp0,  temp4 );
                    temp0   = _mm_add_pd(temp0,  temp6 );
                    temp0   = _mm_add_pd(temp0,  temp8 );
                    temp0   = _mm_add_pd(temp0,  temp10);
                    temp0   = _mm_add_pd(temp0,  temp12);
                    temp0   = _mm_add_pd(temp0,  temp14);
                    temp   -= temp0[0] + temp0[1];

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
