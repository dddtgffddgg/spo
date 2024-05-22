#include <stdio.h>
#include <stdlib.h>

#define N 10

int determinant(int matrix[N][N], int n) {
    if (n == 1) return matrix[0][0];
    if (n == 2) return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    
    int det = 0;
    int temp[N][N];
    int sign = 1;
    for (int i = 0; i < n; i++) {
        int subi = 0;
        for (int j = 1; j < n; j++) {
            int subj = 0;
            for (int k = 0; k < n; k++) {
                if (k == i) continue;
                temp[subi][subj] = matrix[j][k];
                subj++;
            }
            subi++;
        }
        det = det + sign * matrix[0][i] * determinant(temp, n - 1);
        sign = -sign;
    }
    return det;
}

int main() {
    int matrix[N][N];
    printf("Enter the elements of %dx%d matrix:\n", N, N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }
    
    int det = determinant(matrix, N);
    printf("Determinant of the matrix: %d\n", det);
    return 0;
}
