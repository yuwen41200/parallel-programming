/**
 * A Pthreads program that uses the Monte Carlo method to estimate PI.
 * Developer: Yu-wen Pwu, NCTU CS, Taiwan
 * Compilation command: g++ -Wall -Wextra -Wpedantic -pthread --std=c++11 pi.cpp -o pi
 */

#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/sysinfo.h>

long thread_count;
long long number_of_tosses, number_in_circle = 0;
pthread_mutex_t mutex;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<long double> dis(-1, std::nextafter(1, std::numeric_limits<long double>::max()));

void* estimate(void*);

int main(int argc, char* argv[]) {

	thread_count = (long) get_nprocs();
	if (argc != 2)
		return 1;
	number_of_tosses = strtoll(argv[1], NULL, 10);
	if (number_of_tosses <= 0)
		return 1;

	pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t));
	pthread_mutex_init(&mutex, NULL);

	for (long thread_no = 0; thread_no < thread_count; ++thread_no)
		pthread_create(&threads[thread_no], NULL, estimate, (void*) thread_no);

	for (long thread_no = 0; thread_no < thread_count; ++thread_no)
		pthread_join(threads[thread_no], NULL);

	long double pi_estimate = 4 * number_in_circle / (long double) number_of_tosses;
	printf("%.15Lf\n", pi_estimate);

	pthread_mutex_destroy(&mutex);
	free(threads);
	return 0;

}

void* estimate(void* no) {

	long t_no = (long) no;
	long long t_range = number_of_tosses / thread_count;
	long long t_start = t_no * t_range;
	long long t_end = (t_no == thread_count-1) ? number_of_tosses : (t_no+1) * t_range;
	long long t_number_in_circle = 0;

	for (long long toss = t_start; toss < t_end; ++toss) {
		long double x = dis(gen);
		long double y = dis(gen);
		if (x*x + y*y <= 1.0L)
			++t_number_in_circle;
	}

	pthread_mutex_lock(&mutex);
	number_in_circle += t_number_in_circle;
	pthread_mutex_unlock(&mutex);

	return NULL;

}
