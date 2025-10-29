
## Exercitiul 1:

```c
void* threadFunction(void *args)
{
    // TODO: Implement parallel multiply of matrices: C = A * B
    // using P threads by splitting the inner loop.
  
    int thread_id = *(int*)args;
  
    int i,j,k;
    int start, end;
  
    start = thread_id*(N/P);
    end = (thread_id == P - 1) ? N : (thread_id + 1) * (N / P);
  
    for(i = start; i < end; i++) {
        for(j = 0; j < N; j++) {
            for(k = 0; k < N; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
  
    return NULL;
}
```

- o paralelizare normala, nimic special a forului pentru linii.
## Exercitiul 2:

```bash
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/04/lab04$ bash ./testCorrectnessIntensive.sh ex1 seq 5 1000

...
Test 997/1000
Test 998/1000
Test 999/1000
Test 1000/1000
Output correct on intensive test

tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/04/lab04$ time ./ex1 2000 0 1

real    0m31.451s
user    0m31.420s
sys     0m0.028s
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/04/lab04$ time ./ex1 2000 0 4

real    0m9.852s
user    0m37.347s
sys     0m0.041s
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/04/lab04$ time ./ex1 2000 0 8

real    0m18.343s
user    2m22.514s
sys     0m0.032s
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/04/lab04$ time ./ex1 2000 0 16

real    0m17.900s
user    4m31.029s
sys     0m0.413s

```

- nu scaleaza peste 4 threaduri deoarece sunt wsl si probabil am o limita impusa la numarul de threaduri pe care il pot folosi.
## Exercitiul 3:

```c
void* threadFunction(void *args)
{
    // TODO: Implement parallel multiply of matrices: C = A * B
    // using P threads by splitting the inner loop.
  
    int thread_id = *(int*)args;
  
    int i,j,k;
    int start, end;
  
    start = thread_id*(N/P);
    end = (thread_id == P - 1) ? N : (thread_id + 1) * (N / P);
  
    for(i = 0; i < N; i++) {
        for(j = start; j < end; j++) {
            for(k = 0; k < N; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
  
    return NULL;
}
```

- exact ca la ex1 doar ca pt coloane.
## Exercitiul 4:

```bash
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/04/lab04$ bash ./testCorrectnessIntensive.sh mid seq 5 1000

Test 996/1000
Test 997/1000
Test 998/1000
Test 999/1000
Test 1000/1000
Output correct on intensive test

tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/04/lab04$ time ./mid 2000 0 1

real    0m32.370s
user    0m32.314s
sys     0m0.049s
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/04/lab04$ time ./mid 2000 0 2

real    0m16.278s
user    0m32.285s
sys     0m0.040s
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/04/lab04$ time ./mid 2000 0 4

real    0m9.897s
user    0m32.804s
sys     0m0.035s
```
## Exercitiul 5:

```c
void* threadFunction(void *args)
{
    // TODO: Implement parallel multiply of matrices: C = A * B
    // using P threads by splitting the inner loop.
  
    int thread_id = *(int*)args;
  
    int i,j,k;
    int start, end;
    start = thread_id*(N/P);
    end = (thread_id == P - 1) ? N : (thread_id + 1) * (N / P);
  
    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            int c_loc = 0;
            for(k = start; k < end; k++) {
                c_loc += a[i][k] * b[k][j];
            }
            pthread_mutex_lock(&mux);
            c[i][j] += c_loc;
            pthread_mutex_unlock(&mux);
        }
    }
  
    return NULL;
}
```

- am facut adunarea pentru forul cu k intr-un c_local pentru a nu bloca la fiecare iteratie c`[i][j]` cu un mutex.
- abia la iesirea din forul cu k am utilizat un mux ca sa nu existe conflicte la modificarea valorii din c `[i][j]`  deoarece impartind forul cu k `c[i][j]`  devine o zona de memorie sharuita intre threaduri.
## Exercitiul 6:

```bash
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/04/lab04$ bash ./testCorrectnessIntensive.sh in seq 5 1000

...
Test 998/1000
Test 999/1000
Test 1000/1000
Output correct on intensive test

tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/04/lab04$ time ./in 2000 0 1

real    0m23.607s
user    0m23.992s
sys     0m0.043s
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/04/lab04$ time ./in 2000 0 2

real    0m11.092s
user    0m23.771s
sys     0m0.043s
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/04/lab04$ time ./in 2000 0 4

real    0m4.303s
user    0m15.680s
sys     0m0.194s
```
## Exercitiul 7:

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int NReps;
int printLevel;
int N;
int** a;
int** b;
int** c;
int** M1;
int** AUXM11;
int** AUXM12;
int** M2;
int** AUXM21;
int** M3;
int** AUXM31;
int** M4;
int** AUXM41;
int** M5;
int** AUXM51;
int** M6;
int** AUXM61;
int** AUXM62;
int** M7;
int** AUXM71;
int** AUXM72;
  
void getArgs(int argc, char **argv)
{
    if(argc < 4) {
        printf("Not enough paramters: ./program N NReps printLevel\nprintLevel: 0=no, 1=some, 2=verbouse\n");
        exit(1);
    }
    N = atoi(argv[1]);
    NReps = atoi(argv[2]);
    printLevel = atoi(argv[3]);
}
  
int ** allocMatrix(int n, int m)
{
    int ** matrix;
    matrix = malloc(sizeof(int *) * n);
    if(matrix == NULL) {
        printf("malloc failed!");
        exit(1);
    }
    int i;
    for(i = 0; i < N; i++) {
        matrix[i] = malloc(sizeof(int) * N);
        if(matrix[i] == NULL) {
            printf("malloc failed!");
            exit(1);
        }
    }
    return(matrix);
}

void init()
{
    int i, j;
    a = allocMatrix(N, N);
    b = allocMatrix(N, N);
    c = allocMatrix(N, N);
    M1 = allocMatrix(N, N);
    AUXM11 = allocMatrix(N, N);
    AUXM12 = allocMatrix(N, N);
    M2 = allocMatrix(N, N);
    AUXM21 = allocMatrix(N, N);
    M3 = allocMatrix(N, N);
    AUXM31 = allocMatrix(N, N);
    M4 = allocMatrix(N, N);
    AUXM41 = allocMatrix(N, N);
    M5 = allocMatrix(N, N);
    AUXM51 = allocMatrix(N, N);
    M6 = allocMatrix(N, N);
    AUXM61 = allocMatrix(N, N);
    AUXM62 = allocMatrix(N, N);
    M7 = allocMatrix(N, N);
    AUXM71 = allocMatrix(N, N);
    AUXM72 = allocMatrix(N, N);
  
    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            c[i][j] = 0;
            M1[i][j] = 0;
            AUXM11[i][j] = 0;
            AUXM12[i][j] = 0;
            M2[i][j] = 0;
            AUXM21[i][j] = 0;
            M3[i][j] = 0;
            AUXM31[i][j] = 0;
            M4[i][j] = 0;
            AUXM41[i][j] = 0;
            M5[i][j] = 0;
            AUXM51[i][j] = 0;
            M6[i][j] = 0;
            AUXM61[i][j] = 0;
            AUXM62[i][j] = 0;
            M7[i][j] = 0;
            AUXM71[i][j] = 0;
            AUXM72[i][j] = 0;
            if(i <= j) {
                a[i][j] = 1;
                b[i][j] = 1;
            }
            else {
                a[i][j] = 0;
                b[i][j] = 0;
            }
        }
    }
}
  
void printAll()
{
    int i, j;
    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            printf("%i\t",c[i][j]);
        }
        printf("\n");
    }
}
  
void printPartial()
{
    printAll();
}
  
void print()
{
    if(printLevel == 0)
        return;
    else if(printLevel == 1)
        printPartial();
    else
        printAll();
}
  
void mulMatrix(int** C,  int startCi, int startCj, int** A, int startAi, int startAj, int** B, int startBi, int startBj)
{
    // Do not parallelize me
    int i, j, k;
    for(i = 0; i < N/2; i++)
        for(j = 0; j < N/2; j++)
            for(k = 0; k < N/2; k++)
                C[startCi + i][startCj + j] += A[startAi + i][startAj + k] * B[startBi + k][startBj + j];
}
  
void addMatrix(int** C, int startCi, int startCj, int** A, int startAi, int startAj, int** B, int startBi, int startBj)
{
    // Do not parallelize me
    int i, j;
    for(i = 0; i < N/2; i++)
        for(j = 0; j < N/2; j++)
            C[startCi + i][startCj + j] = A[startAi + i][startAj + j] + B[startBi + i][startBj + j];
}
  
void subMatrix(int** C, int startCi, int startCj,  int** A, int startAi, int startAj, int** B, int startBi, int startBj)
{
    // Do not parallelize me
    int i, j;
    for(i = 0; i < N/2; i++)
        for(j = 0; j < N/2; j++)
            C[startCi + i][startCj + j] = A[startAi + i][startAj + j] - B[startBi + i][startBj + j];
}
  
void* strassenTask(void *arg) {
    int id = *(int*)arg;
  
    switch(id) {
        case 1:
            // M1 = (A11 + A22) * (B11 + B22)
            addMatrix(AUXM11, 0,0, a, 0,0, a, N/2, N/2);    
            addMatrix(AUXM12, 0,0, b, 0,0, b, N/2, N/2);    
            mulMatrix(M1, 0,0, AUXM11, 0,0, AUXM12, 0,0);
            break;
  
        case 2:
            // M2 = (A21 + A22) * B11
            addMatrix(AUXM21, 0,0, a, N/2,0, a, N/2, N/2);  
            mulMatrix(M2, 0,0, AUXM21, 0,0, b, 0,0);
            break;
  
        case 3:
            // M3 = A11 * (B12 - B22)
            subMatrix(AUXM31, 0,0, b, 0, N/2, b, N/2, N/2);
            mulMatrix(M3, 0,0, a, 0,0, AUXM31, 0,0);
            break;
  
        case 4:
            // M4 = A22 * (B21 - B11)
            subMatrix(AUXM41, 0,0, b, N/2,0, b, 0,0);      
            mulMatrix(M4, 0,0, a, N/2, N/2, AUXM41, 0,0);
            break;
  
        case 5:
            // M5 = (A11 + A12) * B22
            addMatrix(AUXM51, 0,0, a, 0,0, a, 0, N/2);      
            mulMatrix(M5, 0,0, AUXM51, 0,0, b, N/2, N/2);
            break;
  
        case 6:
            // M6 = (A21 - A11) * (B11 + B12)
            subMatrix(AUXM61, 0,0, a, N/2,0, a, 0,0);    
            addMatrix(AUXM62, 0,0, b, 0,0, b, 0, N/2);      
            mulMatrix(M6, 0,0, AUXM61, 0,0, AUXM62, 0,0);
            break;

  
        case 7:
            // M7 = (A12 - A22) * (B21 + B22)
            subMatrix(AUXM71, 0,0, a, 0, N/2, a, N/2, N/2);
            addMatrix(AUXM72, 0,0, b, N/2,0, b, N/2, N/2);
            mulMatrix(M7, 0,0, AUXM71, 0,0, AUXM72, 0,0);
            break;
  
        default:
            break;
    }
  
    return NULL;
}
  
int main(int argc, char *argv[])
{
    getArgs(argc, argv);
    init();
    pthread_t threads[7];
    int id[7];
    for (int t = 0; t < 7; ++t) {
        id[t] = t + 1; 
        if (pthread_create(&threads[t], NULL, strassenTask, &id[t]) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }
  
    //nu folosesc bariera deoarece am dat join la threaduri aici iar step-ul final il fac din threadul principal.
  
    for (int t = 0; t < 7; ++t) {
        pthread_join(threads[t], NULL);
    }
  
    addMatrix(c, 0, 0, M1, 0, 0, M4, 0, 0);
    subMatrix(c, 0, 0, c, 0, 0, M5, 0, 0);
    addMatrix(c, 0, 0, c, 0, 0, M7, 0, 0);
    addMatrix(c, 0, N/2, M3, 0, 0, M5, 0, 0);
    addMatrix(c, N/2, 0, M2, 0, 0, M4, 0, 0);
    subMatrix(c, N/2, N/2, M1, 0, 0, M2, 0, 0);
    addMatrix(c, N/2, N/2, c, N/2, N/2, M3, 0, 0);
    addMatrix(c, N/2, N/2, c, N/2, N/2, M6, 0, 0);
    //print();
    return 0;
}
```

- nu folosesc bariera deoarece am dat join la threaduri aici iar step-ul final il fac din threadul principal.

## Exercitiul 8:

```bash
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/04/lab04$ bash ./testCorrectnessIntensive.sh strass strass-seq 5 1000

...
Test 995/1000
Test 996/1000
Test 997/1000
Test 998/1000
Test 999/1000
Test 1000/1000
Output correct on intensive test

tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/04/lab04$ time ./strass 4000 0 1

real    0m51.880s
user    5m48.547s
sys     0m1.228s
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/04/lab04$ time ./strass 4000 0 2

real    0m57.154s
user    6m20.050s
sys     0m1.201s
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/04/lab04$ time ./strass 4000 0 4

real    1m50.445s
user    12m40.613s
sys     0m1.211s

```

- Scalarea e inversa fiindca fac paralelizarea nu este tocmai eficienta si creez mult overhead citind de mai multe ori pt fiecare thread in parte practic matricile pentru a efectua operatiile din algoritm.  
* Threadurile acceseaza aceeasi memorie si pierd timp in loc sa lucreze eficient.
