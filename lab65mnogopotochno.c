#include <stdio.h>
#include <pthread.h>

#define N 4 // Размер матрицы
#define NUM_THREADS N // Количество потоков

int matrix[N][N] = {{1, 2, 3, 4}, 
                    {5, 6, 7, 8}, 
                    {9, 10, 11, 12}, 
                    {13, 14, 15, 16}}; // Пример матрицы

double determinant = 0; // Определитель

typedef struct {
    int row_start;
    int row_end;
} ThreadData;

void *calculateDeterminant(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int i, j, k;
    double det = 1;
    
    // Приведение матрицы к треугольному виду
    for (i = data->row_start; i < data->row_end; i++) {
        for (j = i + 1; j < N; j++) {
            double ratio = matrix[j][i] / (double)matrix[i][i];
            for (k = 0; k < N; k++) {
                matrix[j][k] -= ratio * matrix[i][k];
            }
        }
    }
    
    // Вычисление определителя как произведение элементов на главной диагонали
    for (i = 0; i < N; i++) {
        det *= matrix[i][i];
    }
    
    determinant = det;
    
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];
    int i;
    
    for (i = 0; i < NUM_THREADS; i++) {
        threadData[i].row_start = i * (N / NUM_THREADS);
        threadData[i].row_end = (i + 1) * (N / NUM_THREADS);
        if (i == NUM_THREADS - 1) {
            threadData[i].row_end = N;
        }
        
        pthread_create(&threads[i], NULL, calculateDeterminant, (void *)&threadData[i]);
    }
    
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Determinant: %.2f\n", determinant);
    
    return 0;
}
