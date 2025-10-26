#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

long int NReps = 1000; // am pus 1000 de repetari pentru a vedea comportamentul mai bine
int printLevel;
int N;
int P;
int *a;
int *b;
int *c;

void getArgs(int argc, char **argv)
{
	if (argc < 4)
	{
		printf("Not enough paramters: ./program N printLevel P\nprintLevel: 0=no, 1=some, 2=verbouse\n");
		exit(1);
	}
	N = atoi(argv[1]);
	printLevel = atoi(argv[2]);
	P = atoi(argv[3]);
}

void init()
{
	a = malloc(sizeof(int) * N);
	b = malloc(sizeof(int) * N);
	c = malloc(sizeof(int) * N);
	if (a == NULL || b == NULL || c == NULL)
	{
		printf("malloc failed!");
		exit(1);
	}

	int i;
	for (i = 0; i < N; i++)
	{
		a[i] = i % 500;
		b[i] = i % 500;
	}
}

void printPartial()
{
	int i;
	for (i = 0; i < 10; i++)
		printf("%i ", c[i]);
	printf("\n");
	for (i = 20; i < N; i += N / 10)
		printf("%i ", c[i]);
	printf("\n");
	for (i = N - 10; i < N; i++)
		printf("%i ", c[i]);
	printf("\n");
}

void printAll()
{
	int i;
	for (i = 0; i < N; i++)
		printf("%i ", c[i]);
	printf("\n");
}

void print()
{
	if (printLevel == 0)
		return;
	else if (printLevel == 1)
		printPartial();
	else
		printAll();
}

void *threadFunction(void *var)
{
	int thread_id = *(int *)var;

	// calculeaza zona pentru acest thread
	int base = N / P;
	int start = thread_id * base;
	int end = start + base;
	if (thread_id == P - 1) // restul
		end = N;
	if (start > N)
		start = N;
	if (end > N)
		end = N;

	int j, i;
	for (j = 0; j < NReps; j++)
		for (i = start; i < end; i++)
			c[i] = a[i] + b[i];

	pthread_exit(NULL);
	return NULL;
}

int main(int argc, char *argv[])
{
	getArgs(argc, argv);
	init();

	pthread_t *tid = malloc(sizeof(pthread_t) * P);
	int *thread_id = malloc(sizeof(int) * P);
	if (tid == NULL || thread_id == NULL)
	{
		printf("malloc failed for threads!\n");
		exit(1);
	}

	int i;
	for (i = 0; i < P; i++)
		thread_id[i] = i;

	for (i = 0; i < P; i++)
	{
		if (pthread_create(&tid[i], NULL, threadFunction, &thread_id[i]) != 0)
		{
			perror("pthread_create");
			exit(1);
		}
	}

	for (i = 0; i < P; i++)
	{
		pthread_join(tid[i], NULL);
	}

	print();

	free(a);
	free(b);
	free(c);
	free(tid);
	free(thread_id);

	return 0;
}
