import math

def f(x):
    if 0 <= x <= 1:
        return math.cos(x + x**3)
    elif 1 < x <= 2:
        return math.exp(-x**2) - x**2 + 2*x

def midpoint_rule_integration(a, b, n):
    h = (b - a) / n
    result = 0
    for i in range(n):
        xi = a + (i + 0.5) * h
        result += f(xi)
    result *= h
    return result

def runge_rule_integration(epsilon):
    n = 1  # начальное число шагов
    integral_n = midpoint_rule_integration(0, 2, n)
    integral_2n = midpoint_rule_integration(0, 2, 2 * n)
    delta = abs(integral_2n - integral_n) / 3  # формула для метода средних прямоугольников
    while delta >= epsilon:
        n *= 2
        integral_n = midpoint_rule_integration(0, 2, n)
        integral_2n = midpoint_rule_integration(0, 2, 2 * n)
        delta = abs(integral_2n - integral_n) / 3
    return integral_2n

epsilon = 0.0001  # желаемая точность
result = runge_rule_integration(epsilon)
print("Приближенное значение интеграла:", result)
