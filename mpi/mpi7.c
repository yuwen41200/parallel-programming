/**
 * Distributed-Memory Programming with MPI #7
 * Other Functions
 */

// startup and termination
int MPI_Abort(MPI_Comm communicator, int error_code);

// profiling calls
double MPI_Wtime();

// non-blocking point-to-point communication
int MPI_Irecv(void *message, int count, MPI_Datatype type,
              int src, int tag, MPI_Comm communicator, MPI_Request *request);

int MPI_Isend(void *message, int count, MPI_Datatype type,
              int dest, int tag, MPI_Comm communicator, MPI_Request *request);

int MPI_Wait(MPI_Request *request, MPI_Status *status);
