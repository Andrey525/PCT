#include <inttypes.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#if 0
struct thread_data {
    double sum;
    uint8_t padding[64 - sizeof(double)];
};

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

// double func(double x) { return x / (pow(sin(2 * x), 3)); }

double run_serial(double a, double b, int n,
    double h) // последовательный алгоритм
{
    double s = 0.0;
    for (int i = 0; i < n; i++) {
        s += func(a + h * (i + 0.5));
    }
    s *= h;
    return s;
}

double run_parallel_parts(
    double a, double b, int n, double h,
    int count_of_threads) // распределение с разбиением на смежные части
{
    double s = 0.0;
    struct thread_data sumloc[omp_get_max_threads()];

#pragma omp parallel num_threads(count_of_threads)
    {
        int nthreads = omp_get_num_threads();
        int threadid = omp_get_thread_num();
        int points_per_thread = n / nthreads;
        int lo = threadid * points_per_thread;
        int hi = (threadid == nthreads - 1) ? n - 1 : lo + points_per_thread;
        sumloc[threadid].sum = 0.0;

        for (int i = lo; i <= hi; i++) {
            sumloc[threadid].sum += func(a + h * (i + 0.5));
        }

#pragma omp atomic
        s += sumloc[threadid].sum;
    }

    s *= h;
    return s;
}

double run_parallel_cyclical_distribution(
    double a, double b, int n, double h,
    int count_of_threads) // циклическое распределение между потоками
{
    double s = 0.0;
    struct thread_data sumloc[omp_get_max_threads()];

#pragma omp parallel num_threads(count_of_threads)
    {
        int nthreads = omp_get_num_threads();
        int threadid = omp_get_thread_num();

        sumloc[threadid].sum = 0.0;

        for (int i = threadid; i < n; i += nthreads) {
            sumloc[threadid].sum += func(a + h * (i + 0.5));
        }

#pragma omp atomic
        s += sumloc[threadid].sum;
    }

    s *= h;
    return s;
}

double run_parallel_nowait_distribution(double a, double b, int n, double h, int count_of_threads) // распределение между потоками с помощью nowait
{
    double s = 0.0;

#pragma omp parallel num_threads(count_of_threads)
    {
        double sloc = 0.0;
#pragma omp for nowait
        for (int i = 0; i < n; i++)
            sloc += func(a + h * (i + 0.5));
#pragma omp atomic
        s += sloc;
    }

    s *= h;
    return s;
}
#endif

double func(double x)
{
    return x / (pow(sin(2 * x), 3));
}

const double eps = 1E-5;
const int n0 = 10000000;

double Runge_medium_rectangle_method_serial(double a, double b)
{
    int n = n0, k;
    double sq[2], delta = 1;
    for (k = 0; delta > eps; n *= 2, k ^= 1) {
        double h = (b - a) / n;
        double s = 0.0;
        for (int i = 0; i < n; i++) {
            s += func(a + h * (i + 0.5));
        }
        sq[k] = s * h;
        if (n > n0) {
            delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
        }
    }
    return sq[k ^ 1];
}

double Runge_medium_rectangle_method_parallel(double a, double b, int count_of_threads)
{
    double sq[2];
    double s;

#pragma omp parallel num_threads(count_of_threads)
    {
        int n = n0, k;
        double delta = 1;
        for (k = 0; delta > eps; n *= 2, k ^= 1) {
            double h = (b - a) / n;
            s = 0.0;
            sq[k] = 0.0;

#pragma omp barrier
#pragma omp for reduction(+ \
                          : s)
            for (int i = 0; i < n; i++) {
                s += func(a + h * (i + 0.5));
            }

            sq[k] = s * h;

#pragma omp barrier

            if (n > n0) {
                delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
            }
        }
        sq[0] = sq[k ^ 1];
    }
    return sq[0];
}

int main(int argc, char** argv)
{
    const double a = 0.1;
    const double b = 0.5;

    double t = omp_get_wtime();
    double s = Runge_medium_rectangle_method_serial(a, b);
    t = omp_get_wtime() - t;
    printf("\n\nElapsed time serial: %.6lf sec.\n", t);
    printf("S serial: %.12lf\n", s);

    printf("\nParallel versions eps:\n");
    for (int i = 2; i <= 4; i++) {
        printf("\n%d threads\n", i);
        printf("For nowait distribution eps\n");
        t = omp_get_wtime();
        s = Runge_medium_rectangle_method_parallel(a, b, i);
        t = omp_get_wtime() - t;
        printf("Elapsed time parallel (for reduction): %.6lf sec.\n", t);
        printf("S parallel (for reduction): %.12lf\n", s);
    }

    return 0;
}
