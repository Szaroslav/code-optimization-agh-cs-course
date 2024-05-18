#include <stdlib.h>
#include <stdio.h>

#define A(i, j) a[j * m + i]
#define B(i, j) b[j * m + i]
#define abs(x) (x < 0.0 ? -x : x)

void generate_matrix(int m, int n, double *a) {
    for (int i = 0; i < n; i++) {
        A(i, i) = 1.0;
    }
}

void copy_matrix(int m, int n, double *a, double *b) {
    int i, j;

    for (j = 0; j < n; j++) {
        for (i = 0; i < m; i++) {
            B(i, j) = A(i, j);
        }
    }
}

double compare_matrices(int m, int n, double *a, double *b) {
    int i, j;
    double max_diff = 0.0, diff;

    for (j = 0; j < n; j++) {
        for (i = 0; i < m; i++) {
            diff = abs(A(i, j) - B(i, j));
            max_diff = (diff > max_diff ? diff : max_diff);
        }
    }

    return max_diff;
}

void print_matrix(int m, int n, double *a) {
    int i, j;

    for (j = 0; j < n; j++) {
        for (i = 0; i < m; i++) {
            printf("%le ", A(i, j));
        }
        printf("\n");
    }
    printf("\n");
}
