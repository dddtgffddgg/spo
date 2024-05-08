#include <stdio.h>
#include <omp.h>
#include <math.h>

#define NUM_THREADS 4

long double f(long double x) {
    return sin(x); // Замените это на вашу функцию
}

int main() {
    long double a = 0.0; // Начальная граница интегрирования
    long double b = 1.0; // Конечная граница интегрирования
    long double dx = 1e-7; // Шаг интегрирования

    long double integral = 0.0;
    long double x;

    #pragma omp parallel for num_threads(NUM_THREADS) private(x) reduction(+:integral)
    for (int i = 0; i < (int)((b - a) / dx); i++) {
        x = a + i * dx;
        integral += f(x) * dx;
    }

    printf("Integral value: %.20Lf\n", integral);

    return 0;
}
