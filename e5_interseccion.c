#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define NUM_THREADS 32

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

void *interseccionar(void * ptr) {
    int *p, id;
    p = (int *) ptr;
    id = *p;
    int strip = N / NUM_THREADS;
    int extra = N % NUM_THREADS; // elementos adicionales distribuidos uniformemente entre los primeros 'extra' hilos
    int ini = id * strip + (id < extra ? id : extra);
    int fin = ini + strip + (id < extra ? 1 : 0); // un elemento adicional para los primeros 'extra' hilos

    for(int i = ini; i < fin; i++) {
        for(int j = 0; j < N; j++) {
            if(A[i] == B[j]) {
                C[i] = A[i];
                break;
            }
        }
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

    // Inicializar los vectores con valores
    for(int i = 0; i < N; i++) {
        A[i] = i;
        B[i] = i;
        C[i] = -1;
    }

    timetick = dwalltime();

    /* Crea los hilos */
    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], &attr, interseccionar, &ids[i]);
    }

    /* Espera a que los hilos terminen */
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Interseccion de vectores de tamaño %d. Tiempo en segundos %f\n",N, dwalltime() - timetick);

    // printf("Valores de interseccion:\n");
    // for(int i = 0; i < N; i++) {
    //     if(C[i] != -1) {
    //         printf("Posicion %d: %d\n", i, C[i]);
    //     }
    // }

    free(A);
    free(B);
    free(C);
    return 0;
}
