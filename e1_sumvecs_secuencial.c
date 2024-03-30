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

int main(int argc, char*argv[]) {
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

    timetick = dwalltime();

    for (int i = 0; i < N; i++) {
        C[i] = A[i] + B[i];
    }

    printf("Suma de vectores de tamaño %d. Tiempo en segundos %f\n",N, dwalltime() - timetick);

    free(A);
    free(B);
    free(C);
    return 0;
}
