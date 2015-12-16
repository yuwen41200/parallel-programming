/**
 * Concurrent Wave Equation
 * This program was originally written in serial method by the teacher.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAXPOINTS 1000000
#define MAXSTEPS  1000000
#define MINPOINTS 20
#define PI        3.14159265

void checkParam();
void initLine();
void magic(int);
void updateAll();
void printResult();

int totalSteps, totalPoints;
float currVal[MAXPOINTS+2], prevVal[MAXPOINTS+2], nextVal[MAXPOINTS+2];

int main(int argc, char *argv[]) {
	sscanf(argv[1], "%d", &totalPoints);
	sscanf(argv[2], "%d", &totalSteps);
	checkParam();
	//printf("%ld\n", clock());
	printf("Initializing points on the line...\n");
	initLine();
	//printf("%ld\n", clock());
	printf("Updating all points for all time steps...\n");
	updateAll();
	//printf("%ld\n", clock());
	printf("Printing final results...\n");
	printResult();
	//printf("%ld\n", clock());
	printf("\nDone.\n\n");
	return 0;
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

void initLine() {
	float x, fac, k, temp;
	fac = 2.0 * PI;
	k = 0.0;
	temp = totalPoints - 1;
	for (int i = 1; i <= totalPoints; i++) {
		x = k / temp;
		prevVal[i] = currVal[i] = sin(fac * x);
		k += 1.0;
	}
}

// My calculus is poor. I don't know what it means, but it works.
void magic(int i) {
	float dtime, c, dx, tau, sqtau;
	dtime = 0.3;
	c = 1.0;
	dx = 1.0;
	tau = (c * dtime / dx);
	sqtau = tau * tau;
	nextVal[i] = (2.0 * currVal[i]) - prevVal[i] + (sqtau * (-2.0) * currVal[i]);
}

void updateAll() {
	for (int i = 1; i<= totalSteps; i++) {
		for (int j = 1; j <= totalPoints; j++) {
			if ((j == 1) || (j == totalPoints))
				nextVal[j] = 0.0;
			else
				magic(j);
			prevVal[j] = currVal[j];
			currVal[j] = nextVal[j];
		}
	}
}

void printResult() {
	for (int i = 1; i <= totalPoints; i++) {
		printf("%6.4f ", currVal[i]);
		if (i % 10 == 0)
			printf("\n");
	}
}
