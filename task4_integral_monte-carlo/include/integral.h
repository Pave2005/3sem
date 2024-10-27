#pragma once

typedef struct _integral
{
    double left_border;
    double right_border;
    double (*function)(double);
    double result;
} Integral;

void*  monte_carlo        (void* arg);
double calculate_integral (double func(double x), double _left_border, double _right_border);
