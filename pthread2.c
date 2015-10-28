/**
 * Multithreaded Calculation of Pi
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <float.h>
#define thread_count 4l

long term_count;
long double pi = 0;
pthread_mutex_t mutex;
void* sum(void*);

int main(int argc, char* argv[]) {

    if (argc != 2)
        return 1;
    term_count = strtol(argv[1], NULL, 10);
    if (term_count <= 0)
        return 1;
    printf("the range of long double is %Le ~ %Le\nthe number of decimal digits is %d\n",
           LDBL_MIN, LDBL_MAX, LDBL_DIG);

    pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t));
    long* func_args = (long*) malloc(thread_count * sizeof(long));
    pthread_mutex_init(&mutex, NULL);

    for (long thread_no = 0; thread_no < thread_count; ++thread_no) {
        func_args[thread_no] = thread_no;
        pthread_create(&threads[thread_no], NULL, sum, (void*) func_args[thread_no]);
    }

    for (long thread_no = 0; thread_no < thread_count; ++thread_no)
        pthread_join(threads[thread_no], NULL);

    pi *= 4;
    printf("the value of pi is %.18Lf\n", pi);

    free(threads);
    free(func_args);
    pthread_mutex_destroy(&mutex);
    return 0;

}

void* sum(void* no) {
    long t_no = (long) no;
    long t_range = term_count / thread_count;
    long t_start = t_no * t_range;
    long t_end = (t_no == thread_count-1) ? term_count : (t_no+1) * t_range;
    long double t_pi = 0;
    long double t_numerator = (t_start % 2 == 0) ? 1.0 : -1.0;
    for (long i = t_start; i < t_end; ++i, t_numerator = -t_numerator)
        t_pi += t_numerator / (2*i+1);
    pthread_mutex_lock(&mutex);
    pi += t_pi;
    pthread_mutex_unlock(&mutex);
    return NULL;
}
