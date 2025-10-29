#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int printLevel;
int N;
int P;
int A11, A12, A21, A22, B11, B12, B21, B22;
int M1, M2, M3, M4, M5, M6, M7;
int C11, C12, C21, C22;

pthread_barrier_t barrier;

void getArgs(int argc, char **argv) {
    if (argc < 4) {
        printf("Usage: ./program N printLevel P\n");
        exit(1);
    }
    N = atoi(argv[1]);
    printLevel = atoi(argv[2]);
    P = atoi(argv[3]);
}

void init() {
    A11 = 3213215; A12 = 5454; A21 = 5456; A22 = 9898;
    B11 = 54544; B12 = 90821; B21 = 9807879; B22 = 329132;
}

void printPartial() {
    printf("%i %i %i %i\n", C11, C12, C21, C22);
}

void print() {
    if (printLevel > 0) printPartial();
}


void* t1_func(void* arg) {
    // Faza 1
    M1 = (A11 + A22) * (B11 + B22);
    // Bariera – toate M* trebuie să fie gata
    pthread_barrier_wait(&barrier);
    // Faza 2
    C11 = M1 + M4 - M5 + M7;
    return NULL;
}

void* t2_func(void* arg) {
    M2 = (A21 + A22) * B11;
    M3 = A11 * (B12 - B22);
    pthread_barrier_wait(&barrier);
    C12 = M3 + M5;
    return NULL;
}

void* t3_func(void* arg) {
    M4 = A22 * (B21 - B11);
    M5 = (A11 + A12) * B22;
    pthread_barrier_wait(&barrier);
    C21 = M2 + M4;
    return NULL;
}

void* t4_func(void* arg) {
    M6 = (A21 - A11) * (B11 + B12);
    M7 = (A12 - A22) * (B21 + B22);
    pthread_barrier_wait(&barrier);
    C22 = M1 - M2 + M3 + M6;
    return NULL;
}

int main(int argc, char *argv[]) {
    getArgs(argc, argv);
    init();

    const int NUM_THREADS = 4;
    P = NUM_THREADS;

    pthread_t threads[NUM_THREADS];
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    pthread_create(&threads[0], NULL, t1_func, NULL);
    pthread_create(&threads[1], NULL, t2_func, NULL);
    pthread_create(&threads[2], NULL, t3_func, NULL);
    pthread_create(&threads[3], NULL, t4_func, NULL);

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    pthread_barrier_destroy(&barrier);

    print();
    return 0;
}
