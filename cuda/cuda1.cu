/**
 * Concurrent Wave Equation
 * Compilation Command: nvcc cuda1.cu -o cuda1
 * This program was originally written in serial method by the teacher.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXPOINTS 1000000
#define MAXSTEPS  1000000
#define MINPOINTS 20

static void handleError(cudaError_t err, const char *file, int line) {
	if (err != cudaSuccess) {
		printf("%s in %s at line %d\n", cudaGetErrorString(err), file, line);
		exit(EXIT_FAILURE);
	}
}
#define HANDLE_ERROR(err) (handleError(err, __FILE__, __LINE__))

void checkParam();
__global__ void initLine(float*, float*, int);
__global__ void updateAll(float*, float*, float*, int, int);
void printResult();

int totalSteps, totalPoints, allocPoints;
float *currVal;
float *devCurrVal, *devPrevVal, *devNextVal;

int main(int argc, char *argv[]) {
	sscanf(argv[1], "%d", &totalPoints);
	sscanf(argv[2], "%d", &totalSteps);
	checkParam();

	allocPoints = totalPoints + 256;

	currVal = (float*) malloc(allocPoints * sizeof(float));
	if (!currVal)
		exit(EXIT_FAILURE);

	HANDLE_ERROR(cudaMalloc((void**) &devCurrVal, allocPoints * sizeof(float)));
	HANDLE_ERROR(cudaMalloc((void**) &devPrevVal, allocPoints * sizeof(float)));
	HANDLE_ERROR(cudaMalloc((void**) &devNextVal, allocPoints * sizeof(float)));

	dim3 threadsPerBlock(256);
	dim3 numOfBlocks(allocPoints/256);

	printf("Initializing points on the line...\n");
	initLine<<<numOfBlocks, threadsPerBlock>>>(devPrevVal, devCurrVal, totalPoints);

	printf("Updating all points for all time steps...\n");
	updateAll<<<numOfBlocks, threadsPerBlock>>>(devPrevVal, devCurrVal, devNextVal, totalPoints, totalSteps);

	printf("Printing final results...\n");
	HANDLE_ERROR(cudaMemcpy(currVal, devCurrVal, allocPoints * sizeof(float), cudaMemcpyDeviceToHost));
	printResult();

	printf("\nDone.\n\n");

	cudaFree(devCurrVal);
	cudaFree(devPrevVal);
	cudaFree(devNextVal);

	free(currVal);

	return EXIT_SUCCESS;
}

void checkParam() {
	char temp[20];
	while ((totalPoints < MINPOINTS) || (totalPoints > MAXPOINTS)) {
		printf("Enter number of points along vibrating string [%d-%d]: ", MINPOINTS, MAXPOINTS);
		scanf("%s", temp);
		totalPoints = atoi(temp);
		if ((totalPoints < MINPOINTS) || (totalPoints > MAXPOINTS))
			printf("Invalid. Please enter value between %d and %d.\n", MINPOINTS, MAXPOINTS);
	}
	while ((totalSteps < 1) || (totalSteps > MAXSTEPS)) {
		printf("Enter number of time steps [1-%d]: ", MAXSTEPS);
		scanf("%s", temp);
		totalSteps = atoi(temp);
		if ((totalSteps < 1) || (totalSteps > MAXSTEPS))
			printf("Invalid. Please enter value between 1 and %d.\n", MAXSTEPS);
	}
	printf("Using points = %d, steps = %d\n", totalPoints, totalSteps);
}

__global__ void initLine(float *__devPrevVal, float *__devCurrVal, int __totalPoints) {
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if (i < __totalPoints) {
		float x = (float) i / (__totalPoints - 1);
		__devPrevVal[i] = __devCurrVal[i] = __sinf(6.28318530 * x);
	}
}

__global__ void updateAll(float *__devPrevVal, float *__devCurrVal, float *__devNextVal,
                          int __totalPoints, int __totalSteps) {
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if (i < __totalPoints) {
		for (int i = 0; i < __totalSteps; i++) {
			if ((i == 0) || (i == __totalPoints - 1))
				__devNextVal[i] = 0.0;
			else
				__devNextVal[i] = 1.82 * __devCurrVal[i] - __devPrevVal[i];
			__devPrevVal[i] = __devCurrVal[i];
			__devCurrVal[i] = __devNextVal[i];
		}
	}
}

void printResult() {
	for (int i = 0; i < totalPoints; i++) {
		printf("%6.4f ", currVal[i]);
		if ((i + 1) % 10 == 0)
			printf("\n");
	}
}
