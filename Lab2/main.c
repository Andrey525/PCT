#include <inttypes.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

void print_all(double* a, double* b, double* c, int m, int n)
{
    printf("\nMatrix A:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%5.0lf ", a[i * n + j]);
        }
        printf("\n");
    }
    printf("\nVector B:\n");
    for (int j = 0; j < n; j++) {
        printf("%5.0lf ", b[j]);
    }
    printf("\n");
    printf("\nVector С:\n");
    for (int i = 0; i < m; i++) {
        printf("%5.0lf\n", c[i]);
    }
    printf("\n");
}

void matrix_vector_product(double* a, double* b, double* c, int m, int n)
{
    for (int i = 0; i < m; i++) {
        c[i] = 0.0;
        for (int j = 0; j < n; j++)
            c[i] += a[i * n + j] * b[j];
    }
}

void run_serial(int m, int n)
{
    double *a, *b, *c;
    a = malloc(sizeof(*a) * m * n);
    if (!a) {
        printf("Ошибка выделения памяти под матрицу A\n");
        exit(-1);
    }
    b = malloc(sizeof(*b) * n);
    if (!b) {
        printf("Ошибка выделения памяти под вектор B\n");
        exit(-1);
    }
    c = malloc(sizeof(*c) * m);
    if (!c) {
        printf("Ошибка выделения памяти под вектор C\n");
        exit(-1);
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            a[i * n + j] = i + j;
        }
    }
    for (int j = 0; j < n; j++) {
        b[j] = j;
    }

    double t = wtime();
    matrix_vector_product(a, b, c, m, n);
    t = wtime() - t;
    printf("Elapsed time (serial): %.6f sec.\n", t);

    // print_all(a, b, c, m, n);
    free(a);
    free(b);
    free(c);
}

void matrix_vector_product_omp(double* a, double* b, double* c, int m, int n)
{
#pragma omp parallel num_threads(2)
    {
        int nthreads = omp_get_num_threads();
        // printf("nthreads = %d\n", nthreads);
        int threadid = omp_get_thread_num();
        // printf("threadid = %d\n", threadid);
        int items_per_thread = m / nthreads;
        // printf("items_per_thread = %d\n", items_per_thread);
        int lb = threadid * items_per_thread;
        int ub = (threadid == nthreads - 1) ? (m - 1) : (lb + items_per_thread - 1);
        for (int i = lb; i <= ub; i++) {
            // c[i] = 0.0;
            for (int j = 0; j < n; j++)
                c[i] += a[i * n + j] * b[j];
        }
    }
    // nanosleep(&(struct timespec) { .tv_sec = 10 }, NULL);
}

void run_parallel(int m, int n)
{
    double *a, *b, *c;
    a = malloc(sizeof(*a) * m * n);
    if (!a) {
        printf("Ошибка выделения памяти под матрицу A\n");
        exit(-1);
    }
    b = malloc(sizeof(*b) * n);
    if (!b) {
        printf("Ошибка выделения памяти под вектор B\n");
        exit(-1);
    }
    c = malloc(sizeof(*c) * m);
    if (!c) {
        printf("Ошибка выделения памяти под вектор C\n");
        exit(-1);
    }

#pragma omp parallel num_threads(2)
    {
        int nthreads = omp_get_num_threads();
        // printf("nthreads = %d\n", nthreads);
        int threadid = omp_get_thread_num();
        // printf("threadid = %d\n", threadid);
        int items_per_thread = m / nthreads;
        // printf("items_per_thread = %d\n", items_per_thread);
        int lb = threadid * items_per_thread;
        int ub = (threadid == nthreads - 1) ? (m - 1) : (lb + items_per_thread - 1);
        for (int i = lb; i <= ub; i++) {
            for (int j = 0; j < n; j++) {
                a[i * n + j] = i + j;
            }
            c[i] = 0.0;
        }
    }

    // for (int i = 0; i < m; i++) {
    //     for (int j = 0; j < n; j++) {
    //         a[i * n + j] = i + j;
    //     }
    // }
    for (int j = 0; j < n; j++) {
        b[j] = j;
    }

    double t = wtime();
    matrix_vector_product_omp(a, b, c, m, n);
    t = wtime() - t;
    printf("Elapsed time (parallel): %.6f sec.\n", t);
    // print_all(a, b, c, m, n);
    free(a);
    free(b);
    free(c);
}

int main(int argc, char** argv)
{
    int m = 12000, n = 12000; // m - число строк, n - число столбцов
    printf("Memory used: %" PRIu64 " MiB\n", ((m * n + m + n) * sizeof(double)) >> 20);
    double t = wtime();
    run_serial(m, n);
    run_parallel(m, n);
    t = wtime() - t;
    printf("Elapsed time (all_prog): %.6f sec.\n", t);
    return 0;
}