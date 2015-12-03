/**
 * Distributed-Memory Programming with MPI #5
 * Assignment: prime.c
 */

#include <stdio.h>
#include <math.h>
#include "mpi.h"

int isprime(int n) {
	int square_root = (int) sqrt(n);
	for (int i = 3; i <= square_root; i += 2)
		if (n % i == 0)
			return 0;
	return 1;
}

int main(int argc, char *argv[]) {
	long long limit, start, end, part;
	int prime_count, prime_count_par = 0;
	int new_found, new_found_par = 0;
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0) {
		sscanf(argv[1], "%llu", &limit);
		printf("Starting. Numbers to be scanned = %lld.\n", limit);
	}

	MPI_Bcast(limit, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

	part = (limit - 11) / size;
	start = 11 + rank * part;
	end = (rank != size - 1) ? (start + part) : limit;
	start = (start % 2) ? start : (start + 1);

	for (long long n = start; n < end; n += 2) {
		if (isprime(n)) {
			prime_count_par++;
			new_found_par = n;
		}
	}

	if ((rank == 0) && (start % 2)) {
		if (isprime(limit)) {
			prime_count_par++;
			new_found_par = limit;
		}
	}

	MPI_Reduce(&prime_count_par, &prime_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(&new_found_par, &new_found, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	if (rank == 0)
		printf("Done. Largest prime is %d. Total primes %d.\n", new_found, prime_count + 4);

	MPI_Finalize();
	return 0;
}
