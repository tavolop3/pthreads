#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define NUM_THREADS 8
#define X 1

// Dejo los vectores globales para que sean compartidos por
// todos los hilos, se podria tener un struct para los hilos tambien
int *A;
int N=100;
int ocurrencias = 0;
pthread_mutex_t mutex;

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
    int cant = 0;

    for(int i = ini; i < fin; i++) {
        if(A[i] == X) {
            cant++;
        }
    }
    
    pthread_mutex_lock(&mutex);
    ocurrencias += cant;
    pthread_mutex_unlock(&mutex);
}

int main(int argc, char*argv[]) {
    int ids[NUM_THREADS];
    pthread_attr_t attr;
    pthread_t threads[NUM_THREADS];
    pthread_attr_init(&attr);
    pthread_mutex_init(&mutex, NULL);
    double timetick;
    if ((argc != 2) || ((N = atoi(argv[1])) <= 0) ) {
        printf("\nUsar: %s n\n  n: numero de elementos\n", argv[0]);
        exit(1);
    }

    A = (int*)malloc(sizeof(int)*N);

    // Inicializar el vector con mitad pares mitad impares
    for(int i = 0; i < N; i++) {
        A[i] = i%2;
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

    printf("Contar ocurrencias de %d en vector de tamaño %d. Tiempo en segundos %f\n",X,N, dwalltime() - timetick);
    printf("ocurrencias = %d\n", ocurrencias);

    free(A);
    return 0;
}
