#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<pthread.h>

#define NUM_THREADS 8
//Dimension por defecto de las matrices
int N=100;
double *A,*B,*C;

//Para calcular tiempo
double dwalltime(){
    double sec;
    struct timeval tv;

    gettimeofday(&tv,NULL);
    sec = tv.tv_sec + tv.tv_usec/1000000.0;
    return sec;
}

void *multiplicar(void *ptr) {
    int *p, id;
    p = (int *) ptr;
    id = *p;
    printf("id = %d\n", id);
    int strip = N / NUM_THREADS;
    int ini = id * strip;
    int fin = ini + strip;
    int i,j,k,fil,col;
    double total;

    for(i=ini;i<fin;i++){
        for(j=0;j<N;j++){
            total=0;
            fil = i*N;
            col = j*N; 
            for(k=0;k<N;k++){
                total = total + A[fil+k]*B[k+col];
            }
            C[fil+j]=total;
        }
    }   
}

int main(int argc,char*argv[]){
    int ids[NUM_THREADS];
    pthread_attr_t attr;
    pthread_t threads[NUM_THREADS];
    pthread_attr_init(&attr);
    int i,j;
    int check=1;
    double timetick;
    double total=0;

    //Controla los argumentos al programa
    if ((argc != 2) || ((N = atoi(argv[1])) <= 0) )
    {
        printf("\nUsar: %s n\n  n: Dimension de la matriz (nxn X nxn)\n", argv[0]);
        exit(1);
    }

    //Aloca memoria para las matrices
    A=(double*)malloc(sizeof(double)*N*N);
    B=(double*)malloc(sizeof(double)*N*N);
    C=(double*)malloc(sizeof(double)*N*N);

    //Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            A[i*N+j]=1;
            B[i+j*N]=1;
        }
    }   


    //Realiza la multiplicacion

    timetick = dwalltime();

    //Crea los hilos
    for (i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], &attr, multiplicar, &ids[i]);
    }

    //Espera a que los hilos terminen
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Multiplicacion de matrices de %dx%d. Tiempo en segundos %f\n",N,N, dwalltime() - timetick);

    //Verifica el resultado
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            check=check&&(C[i*N+j]==N);
        }
    }   

    if(check){
        printf("Multiplicacion de matrices resultado correcto\n");
    }else{
        printf("Multiplicacion de matrices resultado erroneo\n");
    }

    free(A);
    free(B);
    free(C);
    return(0);
}
