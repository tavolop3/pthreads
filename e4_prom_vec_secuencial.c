#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

int main(int argc, char*argv[]) {
    int *vector;
    double timetick;
    int min_value;
    int max_value;
    int N;
    int avg_value = 0;
    int sum = 0;

    if ((argc != 2) || ((N = atoi(argv[1])) <= 0) ) {
        printf("\nUsar: %s n\n  n: numero de elementos\n", argv[0]);
        exit(1);
    }

    vector = (int*)malloc(sizeof(int)*N);
    for(int i = 0; i < N; i++) {
        vector[i] = i;
    }
    timetick = dwalltime();

    min_value = max_value = sum = vector[0];
    for (int i = 1; i < N; ++i) {
        if (vector[i] < min_value) {
            min_value = vector[i];
        }
        if (vector[i] > max_value) {
            max_value = vector[i];
        }
        sum += vector[i];
    }

    avg_value = sum / N;

    printf("Sacar prom, max y min en vector de tamaño %d. Tiempo en segundos %f\n",N, dwalltime() - timetick);

    printf("Mínimo: %d\n", min_value);
    printf("Máximo: %d\n", max_value);
    printf("Promedio: %d\n", avg_value);

    return 0;
}
