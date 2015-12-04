/**
 * Distributed-Memory Programming with MPI #2
 * Compile: mpicc -o mpi2 mpi2.c
 * Execute: mpirun -n 8 ./mpi2
 */

#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char *argv[]) {
	int rank, size;
	char msg[100];
	MPI_Status stat;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank != 0) {
		sprintf(msg, "process %d\n", rank);
		MPI_Send(msg, strlen(msg) + 1, MPI_CHAR, /*dest*/ 0, /*tag*/ 0, MPI_COMM_WORLD);
	}
	else {
		for (int i = 1; i < size; i++) {
			MPI_Recv(msg, 100, MPI_CHAR, i, /*tag*/ 0, MPI_COMM_WORLD, &stat);
			printf("%s", msg);
		}
		printf("process %d\n", rank);
	}

	MPI_Finalize();
	return 0;
}

// Alternative to Line 26:
// MPI_Recv(msg, 100, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
