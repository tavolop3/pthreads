#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 8

// Dejo los vectores globales para que sean compartidos por
// todos los hilos, se podria tener un struct para los hilos tambien
int *A;
int N=100;
int maximo, minimo, total;
sem_t sem_max,sem_min,sem_tot;

double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

void *contar(void * ptr) {
    int *p,id;
    p = (int *) ptr;
    id = *p;
    int strip = N / NUM_THREADS;
    int ini = id * strip;
    int fin = ini + strip;
    int sum,min,max;

    min = max = sum = A[ini];
    for(int i = ini+1; i < fin; i++) {
        if(A[i] > max) {
            max = A[i];
        }
        if(A[i] < min) {
            min = A[i];
        }
        sum += A[i];
    }

    sem_wait(&sem_max);
    if(max > maximo) {
        maximo = max;
    }
    sem_post(&sem_max);

    sem_wait(&sem_min);
    if(min < minimo) {
        minimo = min;
    }
    sem_post(&sem_min);

    sem_wait(&sem_tot);
    total += sum;
    sem_post(&sem_tot);
}

int main(int argc, char*argv[]) {
    int ids[NUM_THREADS];
    pthread_attr_t attr;
    pthread_t threads[NUM_THREADS];
    pthread_attr_init(&attr);
    double timetick;
    if ((argc != 2) || ((N = atoi(argv[1])) <= 0) ) {
        printf("\nUsar: %s n\n  n: numero de elementos\n", argv[0]);
        exit(1);
    }

    A = (int*)malloc(sizeof(int)*N);
    sem_init(&sem_max, 0, 1);
    sem_init(&sem_min, 0, 1);
    sem_init(&sem_tot, 0, 1);

    for(int i = 0; i < N; i++) {
        A[i] = i;
    }

    timetick = dwalltime();

    /* Crea los hilos */
    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], &attr, contar, &ids[i]);
    }

    /* Espera a que los hilos terminen */
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    int promedio = total / N;

    printf("Sacar prom, max y min en vector de tamaño %d. Tiempo en segundos %f\n",N, dwalltime() - timetick);
    printf("promedio = %d\n", promedio);
    printf("maximo = %d\n", maximo);
    printf("minimo = %d\n", minimo);

    free(A);
    sem_destroy(&sem_max);
    sem_destroy(&sem_min);
    sem_destroy(&sem_tot);
    return 0;
}
