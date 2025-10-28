# exercitiul 3

```c
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
```

![[Captură de ecran 2025-10-08 124844.png]]

- calculul matematic a fost facut doar pentru a consuma cat mai multa putere computationala.
- am pus intr-un while true ca sa pot sa ma uit in task manager in timpul executiei.
# exercitiul 4

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int NReps = 10; 
int printLevel;
int N;
int P;
int *a;
int *b;
int *c;

void getArgs(int argc, char **argv)
{
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
```

- Am impartit vectorii in segmente egale, fiecare thread prelucrand o portiune diferita pentru a efectua adunarea in paralel. 
- Fiecare thread repeta operatia de adunare de `NReps` ori pentru a evidentia mai bine diferentele de performanta intre executia secventiala si cea paralela.
# exercitiul 5

```c
int NReps = 10000; // am pus 1000 de repetari pentru a vedea comportamentul mai bine
```

![[Pasted image 20251008134220.png]]

![[Pasted image 20251008134307.png]]

![[Pasted image 20251008134345.png]]

# exercitiul 6

```c
#include <stdio.h>

int main()
{
    int n, m;
    printf("Introduceti dim mat (n m): ");
    scanf("%d %d", &n, &m);

    int A[n][m], B[n][m], C[n][m];

    printf("Introduceti el A:\n");

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &A[i][j]);

    printf("Introduceti el B:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &B[i][j]);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            C[i][j] = A[i][j] + B[i][j];

    printf("Rez:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            printf("%d ", C[i][j]);
        printf("\n");
    }
  
    return 0;
}
```

![[Pasted image 20251016154056.png]]

# Exercitiul 7

```c
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
```

* practic am interpretat matricea ca un vector si am trensmis id-ul threadului ca parametru la functia thredurilor, in functie de id fiecare thread ia o bucatica din vectori si calculeaza suma.

# Exercitiul 8:

```c
#include <stdio.h>
#include <string.h>

int main()
{
    char sir[200], subsir[100];
    printf("S: ");
    scanf("%s", sir);
    printf("s: ");
    scanf("%s", subsir);

    char *p = strstr(sir, subsir);
    if (p)
        printf("Subsirul gasit la pozitia %d.\n", p - sir);

    return 0;
}
```

- am facut basic, nu prea am ce explica..

# Exercitiul 9:

```c
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
        printf("Thread %d:  %d\n", id, poz_globala);
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
```

![[Pasted image 20251016163928.png]]

- am impartit sirul in subsiruri pentru fiecare thread, am facut astfel incat sa se suprapuna sirurile in care se face cautarea pentru a putea gasii si daca subsirul cautat se aflta la margine.
- de astemea am folosit pe acele sectiuni tot strstr ca sa caut si ma afisat doar din threadul unde a fost gasit subsirul.