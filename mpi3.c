/**
 * Distributed-Memory Programming with MPI #3
 * Collective Calls
 */

int MPI_Barrier(MPI_Comm communicator);

int MPI_Bcast(void *message, int count, MPI_Datatype type,
              int root, MPI_Comm communicator);

int MPI_Scatter(void *send_buf, int send_count, MPI_Datatype send_type,
                void *recv_buf, int recv_count, MPI_Datatype recv_type,
                int root, MPI_Comm communicator);

int MPI_Gather(void *send_buf, int send_count, MPI_Datatype send_type,
               void *recv_buf, int recv_count, MPI_Datatype recv_type,
               int root, MPI_Comm communicator);

int MPI_Allgather(void *send_buf, int send_count, MPI_Datatype send_type,
                  void *recv_buf, int recv_count, MPI_Datatype recv_type,
                  MPI_Comm communicator);

int MPI_Reduce(void *operand, void *result, int count, MPI_Datatype type,
               MPI_Op binary_operator, int root, MPI_Comm communicator);

int MPI_Allreduce(void *send_buf, void *recv_buf, int count, MPI_Datatype type,
                  MPI_Op binary_operator, MPI_Comm communicator);
