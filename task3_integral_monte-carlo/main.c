#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "functions.h"
#include "integral.h"

#define LEFT  2
#define RIGHT 3

void get_time (double func(double x), double _left_border, double _right_border)
{
    double left_border  = _left_border;
    double right_border = _right_border;

    struct timespec start;
    struct timespec finish;

    clock_gettime (CLOCK_MONOTONIC, &start);
    double res = calculate_integral (function, LEFT, RIGHT);
    clock_gettime (CLOCK_MONOTONIC, &finish);

    double exec_time = (finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) / (double)1000000000;

    printf ("integral = %lf\n", res);
    printf ("exec_time = %lf\n", exec_time);

}

int main ()
{
    get_time (function, LEFT, RIGHT);
}
