
## Ex 1: Odd-even Transposition Sort

```c
pthread_barrier_t bar;
  
void* threadFunction(void* arg)
{
    int thread_id = *(int*)arg;
    int offset = 0;
    int step = 2 * P;
  
    for (int i = 0; i < N; i++) {
        // pt even <-> odd
        for (int j = thread_id * 2 + offset; j < N - 1; j += step) {
            if (v[j] > v[j + 1]) {
                int aux = v[j];
                v[j] = v[j + 1];
                v[j + 1] = aux;
            }
        }
        offset++;
        offset %= 2;
  
        pthread_barrier_wait(&bar);
    }
}
```

- fiecare thread compara si inverseaza perechi de elemente disjuncte din vector (`v[j]`, `v[j+1]`) pe randurile pare si impare (controlate de `offset`).

- Dupa fiecare faza de comparari, toate thread-urile se sincronizeaza folosind bariera `pthread_barrier_wait(&bar)` pentru a trece impreuna la faza urmatoare.
## Ex 2:

```bash
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/05/05$ bash testCorrectnessIntensive.sh bubble oets 10 100
rm: cannot remove 'out*': No such file or directory
The result of your parallel program is
======================================
  0  0  1  1  1  2  3  5  6  8
  0  0  1  1  1  2  3  5  6  8
Sorted correctly
======================================
Running intensive correctness test with threads
...
...
Test 97/100
Test 98/100
Test 99/100
Test 100/100
Output correct on intensive test


tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/05/05$ time ./oets 100000 1 1
Sorted correctly

real    0m8.177s
user    0m8.160s
sys     0m0.013s
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/05/05$ time ./oets 100000 1 2
Sorted correctly

real    0m6.789s
user    0m8.933s
sys     0m1.823s
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/05/05$ time ./oets 100000 1 4
Sorted correctly

real    0m7.521s
user    0m9.859s
sys     0m5.268s
```

## Ex 3: mergeSort

```c
void* threadFunction(void* arg)
{
    int tid = *(int*)arg;
    for (int width = 1; width < N; width *= 2) {
        int w = width;
        int start = floor(tid * N / (2.0 * w * P)) * 2 * w;
        int end = floor((tid + 1) * N / (2.0 * w * P)) * 2 * w;
  
        for (int i = start; i < end; i += 2 * w) {
            merge(v, i, i + w, i + 2 * w, vNew);
        }
        pthread_barrier_wait(&barrier);
        if (tid == 0) {
            int* aux = v;
            v = vNew;
            vNew = aux;
        }
        pthread_barrier_wait(&barrier);
    }
    return NULL;
}
```

- Am impartit vectorul in intervale proportionale cu numarul de thread-uri si cu latimea curenta a subvectorilor, astfel incat fiecare thread sa se ocupe doar de partea lui. 
- Practic, pentru fiecare `width`, fiecare thread calculeaza `start` si `end` si aplica merge pe subvectorii din intervalul respectiv, iar la final thread-ul 0 face swap-ul vectorilor, sincronizat prin bariera.

## Ex 4:

```bash
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/05/05$ bash testCorrectnessIntensive.sh m-seq m-par 16 1
00
The result of your parallel program is
======================================
  0  1  2  2  4  4  5  6  6  7  7  8  9 12 13 15
  0  1  2  2  4  4  5  6  6  7  7  8  9 12 13 15
Sorted correctly
======================================
Running intensive correctness test with threads
Test 1/100
...
...
Test 99/100
Test 100/100
Output correct on intensive test


tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/05/05$ time ./m-par 67108864 1 1
Sorted correctly

real    0m18.913s
user    0m18.089s
sys     0m0.821s
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/05/05$ time ./m-par 67108864 1 2
Sorted correctly

real    0m15.164s
user    0m18.429s
sys     0m0.721s
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/05/05$ time ./m-par 67108864 1 4
Sorted correctly

real    0m13.383s
user    0m18.636s
sys     0m0.782s
tudor@DESKTOP-LU8CITI:~/Arhitecturi-Paralele/05/05$ time ./m-par 67108864 1 8
Sorted correctly

real    0m12.525s
user    0m19.286s
sys     0m0.811s
```

## Ex 5: Sheer Sort

```c


```

## Ex 6:

```bash


```

