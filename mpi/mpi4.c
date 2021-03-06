/**
 * Distributed-Memory Programming with MPI #4
 * Derived Datatypes
 */

int MPI_Type_commit(MPI_Datatype *new_type);

int MPI_Type_free(MPI_Datatype *new_type);

int MPI_Type_contiguous(int count, MPI_Datatype old_type, MPI_Datatype *new_type);

int MPI_Type_vector(int count, int block_len, int stride, MPI_Datatype old_type, MPI_Datatype *new_type);

int MPI_Type_indexed(int count, int *arr_of_block_len, int *arr_of_displace, MPI_Datatype old_type,
                     MPI_Datatype *new_type);

int MPI_Type_create_struct(int count, int *arr_of_block_len, MPI_Aint *arr_of_displace,
                           MPI_Datatype *arr_of_old_type, MPI_Datatype *new_type);
