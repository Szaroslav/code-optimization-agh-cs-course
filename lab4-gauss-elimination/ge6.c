//requires additional changes to the code to make it work

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <arm_neon.h>

static double gtod_ref_time_sec = 0.0;
#define IDX(i, j, n) (((j)+ (i)*(n)))
#define MAX(a, b) a > b ? a : b

typedef float64x2_t __m128d;
#define _mm_loadu_pd(a) vld1q_f64(a)
#define _mm_mul_pd(a, b) vmulq_f64(a, b)
#define _mm_sub_pd(a, b) vsubq_f64(a, b)
#define _mm_storeu_pd(a, b) vst1q_f64(a, b)

/* Adapted from the bl2_clock() routine in the BLIS library */

double dclock()
{
  double the_time, norm_sec;
  struct timeval tv;
  gettimeofday( &tv, NULL );
  if ( gtod_ref_time_sec == 0.0 )
    gtod_ref_time_sec = ( double ) tv.tv_sec;
  norm_sec = ( double ) tv.tv_sec - gtod_ref_time_sec;
  the_time = norm_sec + tv.tv_usec * 1.0e-6;
  return the_time;
}

int ge(double *A, int SIZE)
{
  const int BLOCK_SIZE = 8;

  register int i,j,k;
  register double multiplier;
  register __m128d mm_multiplier,
                   temp0,
                   temp1,
                   temp2,
                   temp3,
                   temp4,
                   temp5,
                   temp6,
                   temp7;
  for (k = 0; k < SIZE; k++) {
    for (i = k+1; i < SIZE; i++) {
      multiplier = A[IDX(i, k, SIZE)] / A[IDX(k, k, SIZE)];
      mm_multiplier[0] = multiplier;
      mm_multiplier[1] = multiplier;
      for (j = k+1; j < SIZE;) {
        if (j < MAX(SIZE - BLOCK_SIZE, 0)) {
          temp0 = _mm_loadu_pd(A + IDX(i, j,     SIZE));
          temp1 = _mm_loadu_pd(A + IDX(k, j,     SIZE));
          temp2 = _mm_loadu_pd(A + IDX(i, j + 2, SIZE));
          temp3 = _mm_loadu_pd(A + IDX(k, j + 2, SIZE));
          temp4 = _mm_loadu_pd(A + IDX(i, j + 4, SIZE));
          temp5 = _mm_loadu_pd(A + IDX(k, j + 4, SIZE));
          temp6 = _mm_loadu_pd(A + IDX(i, j + 6, SIZE));
          temp7 = _mm_loadu_pd(A + IDX(k, j + 6, SIZE));

          temp1 = _mm_mul_pd(temp1, mm_multiplier);
          temp3 = _mm_mul_pd(temp3, mm_multiplier);
          temp5 = _mm_mul_pd(temp5, mm_multiplier);
          temp7 = _mm_mul_pd(temp7, mm_multiplier);

          temp0 = _mm_sub_pd(temp0, temp1);
          temp2 = _mm_sub_pd(temp2, temp3);
          temp4 = _mm_sub_pd(temp4, temp5);
          temp6 = _mm_sub_pd(temp6, temp7);

          _mm_storeu_pd(A + IDX(i, j,     SIZE), temp0);
          _mm_storeu_pd(A + IDX(i, j + 2, SIZE), temp2);
          _mm_storeu_pd(A + IDX(i, j + 4, SIZE), temp4);
          _mm_storeu_pd(A + IDX(i, j + 6, SIZE), temp6);

          j += BLOCK_SIZE;
        }
        else {
          A[IDX(i, j, SIZE)] = A[IDX(i, j, SIZE)] - A[IDX(k, j, SIZE)] * multiplier;
          j++;
        }
      }
    }
  }
  return 0;
}

int main( int argc, const char* argv[] )
{
  register int i,j,k,iret;
  double dtime;
  int SIZE = 1500;
  double *matrix = (double *) malloc(SIZE * SIZE * sizeof(double)); // TODO - make near optimal dynamic allocation
  srand(1);
  for (i = 0; i < SIZE; i++) {
    for (j = 0; j < SIZE; j++) {
      matrix[IDX(i, j, SIZE)] = rand();
    }
  }
  printf("call GE");
  dtime = dclock();
  iret = ge(matrix, SIZE);
  dtime = dclock()-dtime;
  printf("Time: %le \n", dtime);

  double check=0.0;
  for (i = 0; i < SIZE; i++) {
    for (j = 0; j < SIZE; j++) {
      check = check + matrix[IDX(i, j, SIZE)];
    }
  }
  printf( "Check: %le \n", check);
  fflush( stdout );

  free(matrix);

  return iret;
}
