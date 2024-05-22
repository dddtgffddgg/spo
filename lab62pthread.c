#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define NUM_THREADS 4

typedef struct {
    long double a;
    long double b;
    int n;
    long double result;
} ThreadData;

long double f(long double x) {
    if (x >= 0 && x <= 1) {
        return cosl(x + x * x * x);
    } else if (x > 1 && x <= 2) {
        return expl(-x * x) - x * x + 2 * x;
    }
    return 0; 
}

void* integrate(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    long double h = (data->b - data->a) / data->n;
    long double sum = 0.0;
    for (int i = 0; i < data->n; ++i) {
        long double x = data->a + (i + 0.5) * h;
        sum += f(x);
    }
    data->result = sum * h;
    return NULL;
}

long double parallel_integrate(long double a, long double b, int n) {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    long double range = (b - a) / NUM_THREADS;
    long double total_sum = 0.0;

    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_data[i].a = a + i * range;
        thread_data[i].b = a + (i + 1) * range;
        thread_data[i].n = n / NUM_THREADS;
        thread_data[i].result = 0.0;
        pthread_create(&threads[i], NULL, integrate, &thread_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
        total_sum += thread_data[i].result;
    }

    return total_sum;
}

int main() {
    long double a = 0.0, b = 2.0;
    long double epsilon = 1e-15;
    int n = 1000; // Initial number of steps
    long double I_n, I_2n, delta;

    I_n = parallel_integrate(a, b, n);
    I_2n = parallel_integrate(a, b, 2 * n);
    delta = fabsl(I_2n - I_n) / 3.0;

    while (delta >= epsilon) {
        n *= 2;
        I_n = I_2n;
        I_2n = parallel_integrate(a, b, 2 * n);
        delta = fabsl(I_2n - I_n) / 3.0;
    }

    printf("Approximated integral value: %.20Lf\n", I_2n);
    return 0;
}
