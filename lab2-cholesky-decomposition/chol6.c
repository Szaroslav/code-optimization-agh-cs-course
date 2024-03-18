#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <x86intrin.h>
#include <immintrin.h>

#define IDX(i, j, n) (((j)+ (i)*(n)))
#define MAX(a, b) a > b ? a : b


static double gtod_ref_time_sec = 0.0;

/* Adapted from the bl2_clock() routine in the BLIS library */

double dclock(){
    double the_time, norm_sec;
    struct timeval tv;
    gettimeofday( &tv, NULL );
    if ( gtod_ref_time_sec == 0.0 )
        gtod_ref_time_sec = ( double ) tv.tv_sec;
    norm_sec = ( double ) tv.tv_sec - gtod_ref_time_sec;
    the_time = norm_sec + tv.tv_usec * 1.0e-6;
    return the_time;
}



int chol(double * A, unsigned int n) {
    const int BLOCK_SIZE = 16;

    register int i, j, k;
    register double temp;
    register __m256d temp0,
                     temp1,
                     temp2,
                     temp3,
                     temp4,
                     temp5,
                     temp6,
                     temp7;


    for (j = 0; j < n; j++) {
        for (i = j; i < n; i++) {
            temp = A[IDX(i, j, n)];

            for (k = 0; k < j;) {
                if (k < MAX(j - BLOCK_SIZE, 0)) {
                    temp0 = _mm256_loadu_pd(&A[IDX(i, k,      n)]);
                    temp1 = _mm256_loadu_pd(&A[IDX(j, k,      n)]);
                    temp2 = _mm256_loadu_pd(&A[IDX(i, k + 4,  n)]);
                    temp3 = _mm256_loadu_pd(&A[IDX(j, k + 4,  n)]);
                    temp4 = _mm256_loadu_pd(&A[IDX(i, k + 8,  n)]);
                    temp5 = _mm256_loadu_pd(&A[IDX(j, k + 8,  n)]);
                    temp6 = _mm256_loadu_pd(&A[IDX(i, k + 12, n)]);
                    temp7 = _mm256_loadu_pd(&A[IDX(j, k + 12, n)]);

                    temp0   = _mm256_mul_pd(temp0, temp1);
                    temp2   = _mm256_mul_pd(temp2, temp3);
                    temp4   = _mm256_mul_pd(temp4, temp5);
                    temp6   = _mm256_mul_pd(temp6, temp7);
                    temp0   = _mm256_add_pd(temp0, temp2);
                    temp2   = _mm256_add_pd(temp2, temp4);
                    temp4   = _mm256_add_pd(temp4, temp6);
                    temp0   = _mm256_add_pd(temp0, temp2);
                    temp0   = _mm256_add_pd(temp0, temp4);
                    temp   -= temp0[0] + temp0[1] + temp0[2] + temp0[3];

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
            return (1);
        }

       temp = sqrt(A[IDX(j, j, n)]);
        for (i = j + 1; i < n; i++) {
            A[IDX(i, j, n)] /= temp;
        }
    }

    return (0);
}

int main(int argc, char ** argv){
    double* A;
    double t1, t2;
    int i, j, n, ret;
    double dtime;

    n = atoi(argv[1]);
    A = malloc(n * n * sizeof(double));
    assert(A != NULL);

    for (i = 0; i < n; i++) {
        A[IDX(i, i, n)] = 1.0;
    }

    dtime = dclock();
    if (chol(A, n)) {
      fprintf(stderr,"Matrix is not symmetric or not positive definite\n");
    }
    dtime = dclock()-dtime;
    printf( "Time: %le \n", dtime);

    fflush( stdout );

    free(A);
    return 0;
}
