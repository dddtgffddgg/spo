#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 10
#define NUM_THREADS 4

int matrix[N][N];
int det = 0;
pthread_mutex_t mutex;

typedef struct {
    int row;
    int n;
} ThreadData;

void *calculate_determinant(void *args) {
    ThreadData *data = (ThreadData *)args;
    int sub_det = 0;
    int temp[N][N];
    int sign = (data->row % 2 == 0) ? 1 : -1;
    
    for (int i = 0; i < data->n; i++) {
        int subi = 0;
        for (int j = 1; j < data->n; j++) {
            int subj = 0;
            for (int k = 0; k < data->n; k++) {
                if (k == i) continue;
                temp[subi][subj] = matrix[data->row * data->n + j][k];
                subj++;
            }
            subi++;
        }
        sub_det = sub_det + sign * matrix[data->row * data->n][i] * determinant(temp, data->n - 1);
        sign = -sign;
    }
    
    pthread_mutex_lock(&mutex);
    det += sub_det;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int determinant(int m[N][N], int n) {
    if (n == 1) return m[0][0];
    if (n == 2) return m[0][0] * m[1][1] - m[0][1] * m[1][0];
    
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int row = 0;
    
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].row = row;
        thread_data[i].n = n;
        pthread_create(&threads[i], NULL, calculate_determinant, (void *)&thread_data[i]);
        row++;
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    return det;
}

int main() {
    pthread_mutex_init(&mutex, NULL);
    printf("Enter the elements of %dx%d matrix:\n", N, N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }
    
    int det = determinant(matrix, N);
    printf("Determinant of the matrix: %d\n", det);
    
    pthread_mutex_destroy(&mutex);
    return 0;
}
