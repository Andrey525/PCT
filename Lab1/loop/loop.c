/*
 * loop.c:
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hpctimer.h"

enum { n = 16 * 1024 * 1024 };

int main()
{
    int *v, i, sum = 0;
    double t;

    if ((v = malloc(sizeof(*v) * n)) == NULL) {
        fprintf(stderr, "No enough memory\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < n; i++)
        v[i] = 1;

    t = hpctimer_wtime();
    /* TODO: Unroll this loop */
    int t1 = 0;
    int t2 = 0;
    int t3 = 0;
    int t4 = 0;
    int t5 = 0;
    int t6 = 0;
    int t7 = 0;
    int t8 = 0;
    int t9 = 0;
    int t10 = 0;
    int t11 = 0;
    int t12 = 0;
    int t13 = 0;
    int t14 = 0;
    int t15 = 0;
    int t16 = 0;
    int t17 = 0;
    int t18 = 0;
    int t19 = 0;
    int t20 = 0;
    int t21 = 0;
    int t22 = 0;
    int t23 = 0;
    int t24 = 0;
    int t25 = 0;
    int t26 = 0;
    int t27 = 0;
    int t28 = 0;
    int t29 = 0;
    int t30 = 0;
    int t31 = 0;
    int t32 = 0;
    int t33 = 0;
    int t34 = 0;
    int t35 = 0;
    int t36 = 0;
    int t37 = 0;
    int t38 = 0;
    int t39 = 0;
    int t40 = 0;
    int t41 = 0;
    int t42 = 0;
    int t43 = 0;
    int t44 = 0;
    int t45 = 0;
    int t46 = 0;
    int t47 = 0;
    int t48 = 0;
    int t49 = 0;
    int t50 = 0;
    int t51 = 0;
    int t52 = 0;
    int t53 = 0;
    int t54 = 0;
    int t55 = 0;
    int t56 = 0;
    int t57 = 0;
    int t58 = 0;
    int t59 = 0;
    int t60 = 0;
    int t61 = 0;
    int t62 = 0;
    int t63 = 0;
    int t64 = 0;
    for (i = 0; i < n; i += 64) {
        t1 += v[i];
        t2 += v[i + 1];
        t3 += v[i + 2];
        t4 += v[i + 3];
        t5 += v[i + 4];
        t6 += v[i + 5];
        t7 += v[i + 6];
        t8 += v[i + 7];
        t9 += v[i + 8];
        t10 += v[i + 9];
        t11 += v[i + 10];
        t12 += v[i + 11];
        t13 += v[i + 12];
        t14 += v[i + 13];
        t15 += v[i + 14];
        t16 += v[i + 15];
        t17 += v[i + 16];
        t18 += v[i + 17];
        t19 += v[i + 18];
        t20 += v[i + 19];
        t21 += v[i + 20];
        t22 += v[i + 21];
        t23 += v[i + 22];
        t24 += v[i + 23];
        t25 += v[i + 24];
        t26 += v[i + 25];
        t27 += v[i + 26];
        t28 += v[i + 27];
        t29 += v[i + 28];
        t30 += v[i + 29];
        t31 += v[i + 30];
        t32 += v[i + 31];
        t33 += v[i + 32];
        t34 += v[i + 33];
        t35 += v[i + 34];
        t36 += v[i + 35];
        t37 += v[i + 36];
        t38 += v[i + 37];
        t39 += v[i + 38];
        t40 += v[i + 39];
        t41 += v[i + 40];
        t42 += v[i + 41];
        t43 += v[i + 42];
        t44 += v[i + 43];
        t45 += v[i + 44];
        t46 += v[i + 45];
        t47 += v[i + 46];
        t48 += v[i + 47];
        t49 += v[i + 48];
        t50 += v[i + 49];
        t51 += v[i + 50];
        t52 += v[i + 51];
        t53 += v[i + 52];
        t54 += v[i + 53];
        t55 += v[i + 54];
        t56 += v[i + 55];
        t57 += v[i + 56];
        t58 += v[i + 57];
        t59 += v[i + 58];
        t60 += v[i + 59];
        t61 += v[i + 60];
        t62 += v[i + 61];
        t63 += v[i + 62];
        t64 += v[i + 63];
    }
    sum = t1 + t2 + t3 + t4 + t5 + t6 + t7 + t8 + t9 + t10 + t11 + t12 + t13 + t14 + t15 + t16 + t17 + t18 + t19 + t20 + t21 + t22 + t23 + t24 + t25 + t26 + t27 + t28 + t29 + t30 + t31 + t32 + t33 + t34 + t35 + t36 + t37 + t38 + t39 + t40 + t41 + t42 + t43 + t44 + t45 + t46 + t47 + t48 + t49 + t50 + t51 + t52 + t53 + t54 + t55 + t56 + t57 + t58 + t59 + t60 + t61 + t62 + t63 + t64;
    // sum = t1 + t2 + t3 + t4 + t5 + t6 + t7 + t8 + t9 + t10 + t11 + t12 + t13 + t14 + t15 + t16 + t17 + t18 + t19 + t20 + t21 + t22 + t23 + t24 + t25 + t26 + t27 + t28 + t29 + t30 + t31 + t32;
    // sum = t1 + t2 + t3 + t4 + t5 + t6 + t7 + t8 + t9 + t10 + t11 + t12 + t13 + t14 + t15 + t16;
    // sum = t1 + t2 + t3 + t4 + t5 + t6 + t7 + t8;

    // for (sum = 0, i = 0; i < n; i++) {
    //     sum += v[i];
    // }
    t = hpctimer_wtime() - t;

    printf("Sum = %d\n", sum);
    printf("Elapsed time (sec.): %.6f\n", t);

    free(v);
    return 0;
}
