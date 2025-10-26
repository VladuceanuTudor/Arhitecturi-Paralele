#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define TEXT_SIZE 1000

int num_threads;
char *sir = NULL;
char *subsir = NULL;

void *gasireSubsir(void *arg)
{
    int id = *(int *)arg;
    int len = strlen(sir);
    int len_sub = strlen(subsir);

    int start = id * len / num_threads;
    int end = (id + 1) * len / num_threads + len_sub - 1; // am facult dupa cum am discutat la tabla la laborator.
    if (end > len)
        end = len; // pt ultimu thread

    int len_part = end - start + 1;
    char *part = malloc(len_part + 1);
    strncpy(part, sir + start, len_part);
    part[len_part] = '\0'; // am facut un sir in care sa caute fiecare thread

    char *p = strstr(part, subsir);

    if (p)
    {
        int poz_globala = start + (p - part);
        printf("Thread %d:  %d\n", id, poz_globala);
    }

    free(part);
    return NULL;
}

int main()
{
    sir = malloc(TEXT_SIZE);
    subsir = malloc(TEXT_SIZE);

    printf("S: ");
    scanf("%s", sir);

    printf("s: ");
    scanf("%s", subsir);

    printf("num threaduri: ");
    scanf("%d", &num_threads);

    pthread_t threads[num_threads];
    int ids[num_threads];

    for (int i = 0; i < num_threads; i++)
    {
        ids[i] = i;
        pthread_create(&threads[i], NULL, gasireSubsir, &ids[i]);
    }

    for (int i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);

    free(sir);
    free(subsir);
    return 0;
}
