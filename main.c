#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "hpctimer.h"
#include <omp.h>

int m = 15000;
int n = 15000;

void matrix_vector_product_omp(double *a, double *b, double *c, int m, int n)
{
	#pragma omp parallel
	{
		int nthreads = omp_get_num_threads();
		int threadid = omp_get_thread_num();
		int items_per_thread = m / nthreads;
		int lb = threadid * items_per_thread;
		int ub = (threadid == nthreads - 1) ? (m - 1) : (lb + items_per_thread - 1);
	
		for (int i = lb; i <= ub; i++) {
			c[i] = 0.0;
			for (int j = 0; j < n; j++)
				c[i] += a[i * n + j] * b[j];
		}
	}
}

void run_parallel()
{
	double *a, *b, *c;
	a = malloc(sizeof(*a) * m * n);
	b = malloc(sizeof(*b) * n);
	c = malloc(sizeof(*c) * m);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			a[i * n + j] = i + j;
	}
	for (int j = 0; j < n; j++)
		b[j] = j;
	double t = hpctimer_getwtime();
	matrix_vector_product_omp(a, b, c, m, n);
    t = hpctimer_getwtime() - t;
	printf("Elapsed time (parallel): %.6f sec.\n", t);
	free(a);
	free(b);
	free(c);
}

void matrix_vector_product(double *a, double *b, double *c, int m, int n)
{
	for (int i = 0; i < m; i++) {
		c[i] = 0.0;
		for (int j = 0; j < n; j++)
			c[i] += a[i * n + j] * b[j];
	}
}

void run_serial()
{
	double *a, *b, *c;
	a = (double *) malloc(sizeof(*a) * m * n);
	b = (double *) malloc(sizeof(*b) * n);
	c = (double *) malloc(sizeof(*c) * m);
	
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			a[i * n + j] = i + j;
	}
	for (int j = 0; j < n; j++)
		b[j] = j;
	
	double t = hpctimer_getwtime();
	matrix_vector_product(a, b, c, m, n);
    t = hpctimer_getwtime() - t;
	
	printf("Elapsed time (serial): %.6f sec.\n", t);
	
	free(a);
	free(b);
	free(c);
}

int main(int argc, char **argv)
{
	#pragma omp parallel 
	{
		printf("Hello, multithreaded world: thread %d of %d\n", omp_get_thread_num(), omp_get_num_threads());
	} 
	printf("\nMatrix-vector product (c[m] = a[m, n] * b[n]; m = %d, n = %d)\n", m, n);
	printf("Memory used: %" PRIu64 " MiB\n", ((m * n + m + n) * sizeof(double)) >> 20);
	run_serial();
	run_parallel();
	return 0;
}
