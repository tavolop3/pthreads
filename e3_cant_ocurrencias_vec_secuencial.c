#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>

#define X 1 

double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}


int main(int argc, char*argv[]) {
    int *A,ocurrencias,N;
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

    for(int i=0; i<N; i++) {
        if(A[i] == X) {
            ocurrencias++;
        }
    }

    printf("Contar ocurrencias de %d en vector de tamaño %d. Tiempo en segundos %f\n",X,N, dwalltime() - timetick);
    printf("ocurrencias = %d\n", ocurrencias);

    free(A);
    return 0;
}
