#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

void *threadFunction(void *var)
{
	// int thread_id = *(int *)var;

	int x = 0;
	while (1) // il oprim cu CTRL+C
		for (int i = 0; i < 1000000; i++)
		{
			x = sin(i) * sqrt(i) * cos(i) * sin(i - tan(i));
		}

	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	int P = 10;
	int i;

	pthread_t tid[P];
	int thread_id[P];
	for (i = 0; i < P; i++)
		thread_id[i] = i;

	for (i = 0; i < P; i++)
	{
		pthread_create(&(tid[i]), NULL, threadFunction, &(thread_id[i]));
	}

	for (i = 0; i < P; i++)
	{
		pthread_join(tid[i], NULL);
	}
	printf("gata");
	return 0;
}
