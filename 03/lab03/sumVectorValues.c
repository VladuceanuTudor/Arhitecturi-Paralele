#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int N;                  
int P;                  
int printLevel;         
long long *v;           
long long global_sum = 0; 
pthread_mutex_t mutex;  

void getArgs(int argc, char **argv) {
    if (argc < 4) {
        printf("Utilizare: ./program N printLevel P\n");
        printf("printLevel: 0 = nimic, 1 = sumă, 2 = detaliat\n");
        exit(1);
    }

    N = atoi(argv[1]);
    printLevel = atoi(argv[2]);
    P = atoi(argv[3]);
}

long long *allocVector(int n) {
    long long *v = malloc(sizeof(long long) * n);
    if (!v) {
        perror("Eroare la alocarea vectorului");
        exit(1);
    }
    return v;
}

void initVector() {
    v = allocVector(N);
    for (long long i = 0; i < N; i++) {
        v[i] = i + 1;
    }
}


void printSum() {
    printf("Suma elementelor: %lld\n", global_sum);
}


void *threadFunction(void *arg) {
    int thread_id = *(int *)arg;

    long long start = thread_id * (N / P);
    long long end = (thread_id == P - 1) ? N : start + (N / P);

    long long local_sum = 0;
    for (long long i = start; i < end; i++) {
        local_sum += v[i];
    }

    pthread_mutex_lock(&mutex);
    global_sum += local_sum;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
    getArgs(argc, argv);
    initVector();
    pthread_mutex_init(&mutex, NULL);

    pthread_t threads[P];
    int thread_ids[P];

    for (int i = 0; i < P; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, threadFunction, &thread_ids[i]);
    }

    for (int i = 0; i < P; i++) {
        pthread_join(threads[i], NULL);
    }

    if (printLevel > 0)
        printSum();

    pthread_mutex_destroy(&mutex);
    free(v);
    return 0;
}
