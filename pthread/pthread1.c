/**
 * A Simple Pthread Test
 *     a. use "gcc -pthread pthread1.c" to compile it
 *     b. usually in a 64-bit Linux system, sizeof pointer = sizeof long
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* func(void*);

int main(int argc, char* argv[]) {

    if (argc != 2)
        return 1;
    long thread_count = strtol(argv[1], NULL, 10);
    if (thread_count <= 0)
        return 1;
    pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t));
    long* func_args = (long*) malloc(thread_count * sizeof(long));

    for (long thread_no = 0; thread_no < thread_count; ++thread_no) {
        func_args[thread_no] = thread_no;
        pthread_create(&threads[thread_no], NULL, func, (void*) func_args[thread_no]);
    }

    printf("in main thread\n");

    for (long thread_no = 0; thread_no < thread_count; ++thread_no) {
        void* thread_status;
        pthread_join(threads[thread_no], &thread_status);
        printf("thread %ld terminated with return code %ld\n", thread_no, (long) thread_status);
    }

    free(threads);
    free(func_args);
    return 0;

}

void* func(void* no) {
    for (int i = 1; i <= 1000; ++i)
        printf("#%04d in thread %ld\n", i, (long) no);
    return (void*) 0;
}
