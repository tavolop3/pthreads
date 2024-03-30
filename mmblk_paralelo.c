/* Blocked matrix multiplication example */
/* Fernando G. Tinetti                   */

#include<stdio.h>
#include<stdlib.h>   /* malloc() */
#include<pthread.h>


#define NUM_THREADS 8 // En realidad es el n en la matriz nxn de threads, osea son n² threads

double *a, *b, *c;
int n, bs;

struct id_info{
    int fil;
    int col;
};

/* Time in seconds from some point in the past */
double dwalltime();

void *multiplicar(void *ptr) {
    struct id_info *id = ptr;
    int fil = id->fil;
    int col = id->col;
    int strip = n / NUM_THREADS;
    int ini_fil = fil * strip;
    int fin_fil = ini_fil + strip;
    int ini_col = col * strip;
    int fin_col = ini_col + strip;
    int i,j,k,ft,ct;
    double total;

    for(i=ini_fil;i<fin_fil;i++){
        for(j=ini_col;j<fin_col;j++){
            total=0;
            ft = i*n;
            ct = j*n; 
            for(k=0;k<n;k++){
                total = total + a[ft+k]*b[k+ct];
            }
            c[ft+j]=total;
        }
    }   
}



/* Init square matrix with a specific value */
void initvalmat(double *mat, int n, double val, int transpose)
{
  int i, j;      /* Indexes */

	if (transpose == 0) {
	  for (i = 0; i < n; i++)
	  {
		for (j = 0; j < n; j++)
		{
		  mat[i*n + j] = val;
		}
	  }
	} else {
	  for (i = 0; i < n; i++)
	  {
		for (j = 0; j < n; j++)
		{
		  mat[j*n + i] = val;
		}
	  }
	}
}




/************** MAIN *************/
int main(int argc, char *argv[])
{
  int i, j, k;
    pthread_attr_t attr;
    pthread_t threads[NUM_THREADS*NUM_THREADS];
    pthread_attr_init(&attr);
  double timetick;

  /* Check command line parameters */
  if ( (argc != 3) || ((n = atoi(argv[1])) <= 0) || ((bs = atoi(argv[2])) <= 0) || ((n % bs) != 0))
  {
    printf("\nError en los parámetros. Usage: ./%s N BS (N debe ser multiplo de BS)\n", argv[0]);
    exit(1);
  }

  /* Getting memory */  
  a = (double *) malloc(n*n*sizeof(double));
  b = (double *) malloc(n*n*sizeof(double));
  c = (double *) malloc(n*n*sizeof(double));

  /* Init matrix operands */
  initvalmat(a, n, 1.0, 0);
  initvalmat(b, n, 1.0, 1);

 // printf("Multiplying %d x %d matrices\n", n, n);
  
  timetick = dwalltime();
  
  // matmulblks(a, b, c, n, bs);
 
    //Crea los hilos
    struct id_info ids[NUM_THREADS][NUM_THREADS];
    int thread_index = 0;
    for (i = 0; i < NUM_THREADS; i++) {
        for(j=0;j<NUM_THREADS;j++){
            ids[i][j].fil = i;
            ids[i][j].col = j;
            pthread_create(&threads[i*NUM_THREADS+j], &attr, multiplicar, &ids[i][j]);
        }
    }

    //Espera a que los hilos terminen
    for (i = 0; i < NUM_THREADS; i++) {
        for(j=0;j<NUM_THREADS;j++){
            pthread_join(threads[i*NUM_THREADS+j], NULL);
        }
    }

  double workTime = dwalltime() - timetick;

  /* Check results (just in case...) */
  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
    {
      if (c[i*n + j] != n)
      {
        printf("Error at %d, %d, value: %f\n", i, j, c[i*n + j]);
      }
    }
  }

  printf("MMBLK-SEC;%d;%d;%lf;%lf\n",n,bs,workTime,((double)2*n*n*n)/(workTime*1000000000));

  return 0;
}

/*****************************************************************/
#include <stdio.h>
#include <sys/time.h>

double dwalltime()
{
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}
