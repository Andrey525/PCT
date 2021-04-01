#include <inttypes.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

double getrand() { return (double)rand() / RAND_MAX; }

double getrand_r(unsigned int* seed) { return (double)rand_r(seed) / RAND_MAX; }

double funcMC(double x, double y)
{
    return exp(pow(x + y, 2)); // Ω = {x ∈ (0, 1), y ∈ (0, 1 − x)}
}

double Monte_Carlo(int n)
{
    double in = 0;
    double s = 0;
    unsigned int seed = 0;
    for (int i = 0; i < n; i++) {
        double x = getrand_r(&seed); /* x in (0, 1) */
        double y = getrand_r(&seed); /* y in (0, 1 - x) */
        if (x > 0 && x < 1 && y < 1 - x) {
            in++;
            s += funcMC(x, y);
        }
    }
    double v = in / n;
    double res = v * s / in;
    return res;
}

double Monte_Carlo_parallel(int n, int count_of_threads)
{
    double in = 0;
    double s = 0;
#pragma omp parallel num_threads(count_of_threads)
    {
        unsigned int seed = omp_get_thread_num();
#pragma omp for reduction(+ \
                          : in, s)
        for (int i = 0; i < n; i++) {
            double x = getrand_r(&seed); /* x in (0, 1) */
            double y = getrand_r(&seed); /* y in (0, 1 - x) */
            if (x > 0 && x < 1 && y < 1 - x) {
                in++;
                s += funcMC(x, y);
            }
        }
    }
    double v = in / n;
    double res = v * s / in;
    return res;
}

int main(int argc, char** argv)
{

    const int n = 10000000;
    double t;
    double s;
    t = omp_get_wtime();
    s = Monte_Carlo(n);
    t = omp_get_wtime() - t;

    printf("S MC 10^7: %.12lf\n", s);

    printf("Elapsed time MC 10^7: %.6lf sec.\n", t);

    for (int i = 2; i <= 4; i++) {
        printf("\n%d threads\n", i);
        t = omp_get_wtime();
        s = Monte_Carlo_parallel(n, i);
        t = omp_get_wtime() - t;
        printf("S MC 10^7 parallel: %.12lf\n", s);
        printf("Elapsed time MC parallel 10^7: %.6lf sec.\n", t);
    }

    printf("\n_________FOR 10^8__________\n");

    t = omp_get_wtime();
    s = Monte_Carlo(n * 10);
    t = omp_get_wtime() - t;
    printf("S MC 10^8: %.12lf\n", s);

    printf("Elapsed time MC 10^8: %.6lf sec.\n", t);

    for (int i = 2; i <= 4; i++) {
        printf("\n%d threads\n", i);
        t = omp_get_wtime();
        s = Monte_Carlo_parallel(n * 10, i);
        t = omp_get_wtime() - t;
        printf("S MC 10^8 parallel: %.12lf\n", s);
        printf("Elapsed time MC 10^8 parallel: %.6lf sec.\n", t);
    }
    return 0;
}
