
## Exercitiul 1:
 - abia la 100.000 imi da un rezultat eronat din cauza rc.
![[Pasted image 20251028133811.png]]
## Exercitiul 2:

```bash
Test 9990/10000
Test 9991/10000
Test 9992/10000
Test 9993/10000
Test 9994/10000
Test 9995/10000
Test 9996/10000
Test 9997/10000
Test 9998/10000
Test 9999/10000
Test 10000/10000
Output correct on intensive test
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/03/lab03$ 
```

Desi am avut 10.000 de interatii a dat corect rezultatul asa cum am vazut si la exercitiul 1. 
## Exercitiul 3:

```c
void* threadFunction(void *var)
{
    int thread_id = *(int*)var;
    int i;
    //TODO MAKE OPERATIONS ON a SAFE
    for(i = 0; i < N; i++)
        pthread_mutex_lock(&mutex);
        a = a + TOADD;
        pthread_mutex_unlock(&mutex);
}
```

- am adaugat un mutex pt a il face safe, din motive de testare cat mai buna ma ales de data asta sa il testez cu 100.000 de iteratii.
```bash
Test 9997/10000
Test 9998/10000
Test 9999/10000
Test 10000/10000
Output correct on intensive test
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/03/lab03$ 
```
## Exercitiul 4:

```c
pthread_barrier_t bar;

void* threadFunction(void *var)
{
    //TODO preserve the correct order by using barriers
    int thread_id = *(int*)var;
    if(thread_id==0)
    {
        pthread_barrier_wait(&bar);
        pthread_barrier_wait(&bar);
        printf("I should be displayed last\n");
    }
    else
    if(thread_id==1)
    {
        pthread_barrier_wait(&bar);
        printf("I should be displayed in the middle\n");
        pthread_barrier_wait(&bar);
    }
    else if(thread_id==2){
        printf("I should be displayed first\n");
        pthread_barrier_wait(&bar);
        pthread_barrier_wait(&bar);
    }
}
```

- practic am facut astfel incat primul print sa  nu necesite trecerea unei bariere, la al doi-lea sa trebuiasca sa treaca threadurile doar o barieara, iar pt al trei-lea print sa trebuiasca sa treaca de 2 bariere.
- poate nu sunt prea bun la explicatii dar are sens...

```bash
I should be displayed first
I should be displayed in the middle
I should be displayed last
[1] + Done                       "/usr/bin/gdb" --interpreter=mi --tty=${DbgTerm} 0<"/tmp/Microsoft-MIEngine-In-e00bt4p4.lw2" 1>"/tmp/Microsoft-MIEngine-Out-scaqc4cq.plh"
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/03$ 
```

```bash
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/03/lab03$ ./testCorrectnessIntensive.sh barrier bar 1 10000
...
...
Test 9997/10000
Test 9998/10000
Test 9999/10000
Test 10000/10000
Output correct on intensive test
```
## Exercitiul 5:

```c
sem_t sem;

void* threadFunction(void *var)
{
    //TODO preserve the correct order by using semaphores you will need to add a library and the correct flag when compiling
    int thread_id = *(int*)var;
    if(thread_id==0)
        {sem_wait(&sem);
        sem_wait(&sem);
        printf("I should be displayed last\n");}
    else if(thread_id==1)
        {printf("I should be displayed first or in the middle\n");
        sem_post(&sem);}
    else if(thread_id==2)
        {printf("I should be displayed first or in the middle\n");
        sem_post(&sem);}
}

...

int main(int argc, char *argv[])
{
	...
    sem_init(&sem, 0, 0);
    ...
```

```bash
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/03/lab03$ ./semaphoreSignal 
I should be displayed first or in the middle
I should be displayed first or in the middle
I should be displayed last

tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/03/lab03$ ./testCorrectnessIntensive.sh semaphoreSignal sem-seq 1 10000
...
...
Test 9996/10000
Test 9997/10000
Test 9998/10000
Test 9999/10000
Test 10000/10000
Output correct on intensive test
```
## Exercitiul 6:

```c
thread_mutexattr_t attr;
pthread_mutexattr_init(&attr);
pthread_mutexattr_setrobst(&attr, PTHREAD_MUTEX_ROBUST);
pthread_mutex_init(&mutex, &attr);
pthread_mutexattr_destroy(&attr);
```

- am setat atributul ROBUST la mutex care permite recuperarea mutexului daca threadul detinator moare (adica cel care a facut lock). Perfect pentru acest caz.
![[Pasted image 20251028132931.png]]
## Exercitiul 7:

```c
pthread_mutex_t mutexA;
pthread_mutex_t mutexB;
pthread_barrier_t bar;
  
void* threadFunction(void *var)
{
    //TODO preserve the correct order by using barriers
    // HINT MERGE SA FOLOSIM O BARIERA

    int thread_id = *(int*)var;
    if(thread_id==0) {
        pthread_mutex_lock(&mutexA);
        sleep(1);
        pthread_mutex_lock(&mutexB);
        printf("There should be two messages displayed, I am one of them\n");
        pthread_mutex_unlock(&mutexB);
        pthread_mutex_unlock(&mutexA);
        pthread_barrier_wait(&bar);

    } else {
        pthread_barrier_wait(&bar);
        pthread_mutex_lock(&mutexB);
        sleep(1);
        pthread_mutex_lock(&mutexA);
        printf("There should be two messages displayed, I am one of them\n");
        pthread_mutex_unlock(&mutexA);
        pthread_mutex_unlock(&mutexB);
    }
}
```

- iar in main am initializat bariera cu nr de procese (P):
```c
pthread_mutex_init(&mutexA, NULL);
pthread_mutex_init(&mutexB, NULL);
pthread_barrier_init(&bar, NULL, P);
```

![[Pasted image 20251028131720.png]]

* practic am folosit bariera astfel incat sa ajunga ambele threaduri in dreptul ei abia dupa ce primul a dat unlock la mutexi, asa ca nu mai avem probleme. 
* daca scoatem sleep urile si lasam codul in formatul initial tot putem avea probleme insa depinde de modul in care sunt instructuinile programate pe procesor (e undefined pt noi ca user), ceea ce nu e de dorit. In cazul meu merge bine dar este un comportament nesigur.
## Exercitiul 8:

```c
//pthread_mutex_init(&mutex, NULL);
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(&mutex, &attr);
    pthread_mutexattr_destroy(&attr);
    //astfel incat chiar daca un thred vrea sa dea lock de doua ori pe acelasi mutex sa nu ramana blocat.
    //atributul face asta
```

![[Pasted image 20251022134057.png]]
## Exercitiul 9:

- fiecare thread ia o bucata din vector, calculeaza suma locala si ulterior o adauga la suma globala, suma globala fiind modificata de fiecare thread este protejata de catre un mutex.
```c
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
```

```bash
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/03/lab03$ gcc sumVectorValues.c -o sumVectorValues -lpthread -O2
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/03/lab03$ time ./sumVectorValues 1000000000 1 1
Suma elementelor: 500000000500000000

real    0m9.633s
user    0m1.141s
sys     0m8.487s
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/03/lab03$ time ./sumVectorValues 1000000000 1 4
Suma elementelor: 500000000500000000

real    0m7.145s
user    0m1.436s
sys     0m6.229s
```

- dupa cum se poate vedea am obtinut scalabilitate.
## Exercitiul 10:

- am implementat astfel incat sa scaleze din prima(ex anterior), mai era o posibilitate sa adune toate direct in suma globala, care sa fie protejata de un mutex si atunci nu era scalabil, ba chiar invers, cu cat mai multe threaduri am fi avut cu atat mai mult ar fii asteptat sa prinda "loc" sa scrie un variabila (suma) protejata.
## Exercitiul 11:

```c
void* t1_func(void* arg) {
    // Faza 1
    M1 = (A11 + A22) * (B11 + B22);
    // Bariea – toate M* trebuie s fie gata
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
```

-  **Bariera** sincronizeaza fazele:
	- Faza 1 → toate M1..M7 se calculeaz complet.
	- Bariera → toți așteaptă.
	- Faza 2 → calculează C-urile doar după ce toate M-urile sunt disponibile.

