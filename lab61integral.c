#include <stdio.h>
#include <math.h>

long double f(long double x) {
    if (x >= 0 && x <= 1) {
        return cosl(x + x * x * x);
    } else if (x > 1 && x <= 2) {
        return expl(-x * x) - x * x + 2 * x;
    }
    return 0; // For safety, though it's outside the given range.
}

long double integrate(long double (*func)(long double), long double a, long double b, int n) {
    long double h = (b - a) / n;
    long double sum = 0.0;
    for (int i = 0; i < n; ++i) {
        long double x = a + (i + 0.5) * h;
        sum += func(x);
    }
    return sum * h;
}

int main() {
    long double a = 0.0, b = 2.0;
    long double epsilon = 1e-15;
    int n = 10;
    long double I_n, I_2n, delta;

    I_n = integrate(f, a, b, n);
    I_2n = integrate(f, a, b, 2 * n);
    delta = fabsl(I_2n - I_n) / 3.0;

    while (delta >= epsilon) {
        n *= 2;
        I_n = I_2n;
        I_2n = integrate(f, a, b, 2 * n);
        delta = fabsl(I_2n - I_n) / 3.0;
    }

    printf("Approximated integral value: %.20Lf\n", I_2n);
    return 0;
}
