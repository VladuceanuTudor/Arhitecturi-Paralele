#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 1000

int n, m, num_threads;
int *A, *B, *C;

void *aduna(void *arg)
{
    int id = *(int *)arg;
    int total = n * m;
    int start = id * total / num_threads;
    int end = (id + 1) * total / num_threads;

    for (int k = start; k < end; k++)
        C[k] = A[k] + B[k];

    return NULL;
}

int main()
{
    printf("n, m, num_threads:");
    scanf("%d %d %d", &n, &m, &num_threads);

    int total = n * m;

    A = malloc(total * sizeof(int));
    B = malloc(total * sizeof(int));
    C = malloc(total * sizeof(int));

    printf("A:\n");
    for (int i = 0; i < total; i++)
        scanf("%d", &A[i]);

    printf("B:\n");
    for (int i = 0; i < total; i++)
        scanf("%d", &B[i]);

    pthread_t threads[num_threads];
    int id[num_threads];

    for (int i = 0; i < num_threads; i++)
    {
        id[i] = i;
        pthread_create(&threads[i], NULL, aduna, &id[i]);
    }

    for (int i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);

    printf("Rez:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            printf("%d ", C[i * m + j]);
        printf("\n");
    }

    free(A);
    free(B);
    free(C);
    return 0;
}
