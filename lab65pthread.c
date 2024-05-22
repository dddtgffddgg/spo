#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 3
#define NUM_THREADS 3

int matrix[N][N];
int det = 0;
pthread_mutex_t mutex;

typedef struct {
    int col;
    int n;
} ThreadData;

int determinant(int m[N][N], int n) {
    if (n == 1) return m[0][0];
    if (n == 2) return m[0][0] * m[1][1] - m[0][1] * m[1][0];
    
    int temp[N][N];
    int sign = 1;
    int det = 0;
    
    for (int i = 0; i < n; i++) {
        int subi = 0;
        for (int j = 1; j < n; j++) {
            int subj = 0;
            for (int k = 0; k < n; k++) {
                if (k == i) continue;
                temp[subi][subj] = m[j][k];
                subj++;
            }
            subi++;
        }
        det += sign * m[0][i] * determinant(temp, n - 1);
        sign = -sign;
    }
    return det;
}

void *calculate_determinant(void *args) {
    ThreadData *data = (ThreadData *)args;
    int temp[N][N];
    int sign = (data->col % 2 == 0) ? 1 : -1;
    int sub_det = 0;

    int subi = 0;
    for (int j = 1; j < data->n; j++) {
        int subj = 0;
        for (int k = 0; k < data->n; k++) {
            if (k == data->col) continue;
            temp[subi][subj] = matrix[j][k];
            subj++;
        }
        subi++;
    }
    sub_det = sign * matrix[0][data->col] * determinant(temp, data->n - 1);
    
    pthread_mutex_lock(&mutex);
    det += sub_det;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main() {
    pthread_mutex_init(&mutex, NULL);
    printf("Enter the elements of %dx%d matrix:\n", N, N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }
    
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].col = i;
        thread_data[i].n = N;
        pthread_create(&threads[i], NULL, calculate_determinant, (void *)&thread_data[i]);
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Determinant of the matrix: %d\n", det);
    
    pthread_mutex_destroy(&mutex);
    return 0;
}
