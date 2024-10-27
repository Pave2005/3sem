#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

#include "config.h"
#include "integral.h"

void* monte_carlo (void* arg)
{
    if (!arg) return NULL;

    Integral* integral = (Integral*)arg;

    int points = POINTS_COUNT / THREADS_COUNT;
    int count = 0;

    double square = (integral->right_border - integral->left_border) * (integral->function (integral->right_border) - 0);
    double x = 0;
    double y = 0;

    srand ((unsigned int)time(NULL));

    double func_max   = integral->function (integral->right_border);
    double segment    = integral->right_border - integral->left_border;
    unsigned int seed = 0;

    for (int i = 0; i < points; ++i)
    {
        x = integral->left_border + (double)rand_r (&seed) / (double)RAND_MAX * segment;
        y = (double)(rand_r (&seed)) / (double)RAND_MAX * func_max;

        if (fabs (y) - integral->function (x) < 0) count++;
    }

    integral->result = (square / points) * count;

    return integral;
}

double calculate_integral (double function(double x), double _left_border, double _right_border)
{

    Integral integral = {.left_border  =  _left_border,
                         .right_border = _right_border,
                         .function     =      function,
                         .result       =           0.0 };

    double segment = integral.right_border - integral.left_border;
    double sub_segment = segment / THREADS_COUNT;

    pthread_t threads[THREADS_COUNT]       = {};
    Integral  sub_integrals[THREADS_COUNT] = {};

    for (size_t i = 0; i < THREADS_COUNT; ++i)
    {
        sub_integrals[i].left_border  = integral.left_border + sub_segment * i;
        sub_integrals[i].right_border = sub_integrals[i].left_border + sub_segment;
        sub_integrals[i].function     = integral.function;

        pthread_create(&(threads[i]), NULL, monte_carlo, &(sub_integrals[i]));
    }

    for (size_t i = 0; i < THREADS_COUNT; ++i)
    {
        pthread_join(threads[i], NULL);

        integral.result += sub_integrals[i].result;
    }

    return integral.result;
}
