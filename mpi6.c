/**
 * Distributed-Memory Programming with MPI #6
 * Assignment: integrate.c
 */

#include <stdio.h>
#include <math.h>
#include "mpi.h"
#define PI 3.1415926535

int main(int argc, char *argv[]) {
	long long num, start, end, part;
	double width, x, sum, sum_par = 0;
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0)
		sscanf(argv[1], "%llu", &num);

	MPI_Bcast(num, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

	part = num / size;
	start = rank * part;
	end = (rank != size - 1) ? (start + part) : num;
	width = PI / num;

	for (long long n = start; n < end ; n++) {
		x = (n - 0.5) * width;
		sum_par += sin(x);
	}

	if (rank == 0) {
		x = (num - 0.5) * width;
		sum_par += sin(x);
	}

	MPI_Reduce(&sum_par, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
		printf("The total area is: %f\n", (float) (sum * width));

	MPI_Finalize();
	return 0;
}
