#include <inttypes.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

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

double func(double x)
{
    // return exp(-x * x);
    // return sin(x + 2) / (0.4 + cos(x));
    return x / (pow(sin(2 * x), 3));
}

double run_serial(double a, double b, int n, double h) // последовательный алгоритм
{
    double s = 0.0;
    for (int i = 0; i < n; i++) {
        s += func(a + h * (i + 0.5));
    }
    s *= h;
    return s;
}

double run_parallel_parts(double a, double b, int n, double h, int count_of_threads) // распределение с разбиением на смежные части
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

double run_parallel_cyclical_distribution(double a, double b, int n, double h, int count_of_threads) // циклическое распределение между потоками
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

int main(int argc, char** argv)
{
    const double a = 0.1;
    const double b = 0.5;
    const int n = pow(10, 7);
    double h = (b - a) / n;
    double t = omp_get_wtime();
    double s = run_serial(a, b, n, h);
    t = omp_get_wtime() - t;
    printf("n = %d\n\n", n);
    printf("Elapsed time serial: %.6f sec.\n", t);
    printf("S serial: %.12f\n", s);

    printf("\nParallel versions:\n");
    for (int i = 2; i <= 4; i++) {
        printf("\n%d threads\n", i);

        printf("Distribution by parts\n");
        t = omp_get_wtime();
        s = run_parallel_parts(a, b, n, h, i);
        t = omp_get_wtime() - t;
        printf("Elapsed time parallel (parts): %.6f sec.\n", t);
        printf("S parallel (parts): %.12f\n", s);

        printf("Cyclical distribution\n");
        t = omp_get_wtime();
        s = run_parallel_cyclical_distribution(a, b, n, h, i);
        t = omp_get_wtime() - t;
        printf("Elapsed time parallel (cyclical): %.6f sec.\n", t);
        printf("S parallel (cyclical): %.12f\n", s);

        printf("For nowait distribution\n");
        t = omp_get_wtime();
        s = run_parallel_nowait_distribution(a, b, n, h, i);
        t = omp_get_wtime() - t;
        printf("Elapsed time parallel (for nowait): %.6f sec.\n", t);
        printf("S parallel (for nowait): %.12f\n", s);
    }

    // printf("Result Pi: %.12f\n", s * s);
    return 0;
}
