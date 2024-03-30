#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define NUM_THREADS 8

// Dejo los vectores globales para que sean compartidos por
// todos los hilos, se podria tener un struct para los hilos tambien
int *A, *B, *C;
int N=100;

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

void *sumar(void * ptr) {
    int *p, id;
    p = (int *) ptr;
    id = *p;
    int strip = N / NUM_THREADS;
    int ini = id * strip;
    int fin = ini + strip;

    for(int i = ini; i < fin; i++) {
        C[i] = A[i] + B[i];
    }
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
    B = (int*)malloc(sizeof(int)*N);
    C = (int*)malloc(sizeof(int)*N);

    // Inicializar el vector con valores
    for(int i = 0; i < N; i++) {
        A[i] = i;
        B[i] = i;
        C[i] = 0;
    }

    /* Crea los hilos */
    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], &attr, sumar, &ids[i]);
    }

    timetick = dwalltime();

    /* Espera a que los hilos terminen */
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Suma de vectores de tamaño %d. Tiempo en segundos %f\n",N, dwalltime() - timetick);

    free(A);
    free(B);
    free(C);
    return 0;
}
