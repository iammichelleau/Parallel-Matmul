#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int n = 0;

int main(int argc, const char *argv[]) {
    FILE *fr;
    int i, j, k, dim;
    char filename[] = "";
    
    strcpy(filename, (char *)argv[1]);
    strcat(filename, ".txt");
    fr = fopen(filename,"r");
    
    fscanf(fr, "%d", &dim);
    n = dim;
    
    double *A[n], *C[n];
    for(i = 0; i < n; i++){
        A[i] = (double *)malloc(n * sizeof(double));
        C[i] = (double *)malloc(n * sizeof(double));
    }
    
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++)
            fscanf(fr, "%lf", &(A[i][j]));
    }
    
    int tid, nthreads, chunk = 10;
    clock_t start, end;
    double cpu_time_used;
    
    start = clock();
    
    #pragma omp parallel shared(A,C,nthreads,chunk) private(tid,i,j,k)
    {
        tid = omp_get_thread_num();
        if (tid == 0){
            nthreads = omp_get_num_threads();
//            printf("Starting matrix multiple example with %d threads\n",nthreads);
//            printf("Initializing matrices...\n");
        }
        
        #pragma omp for schedule (static, chunk)
        for (i = 0; i < n; i++){
            for (j = 0; j < n; j++)
                C[i][j]= 0;
        }
        
//        printf("Thread %d starting matrix multiply...\n",tid);
        #pragma omp for schedule (static, chunk)
        for (i = 0; i < n; i++){
//            printf("Thread=%d did row=%d\n",tid,i);
            for(j = 0; j < n; j++){
                for (k = 0; k < n; k++)
                    C[i][j] += A[i][k] * A[k][j];
            }
        }
    }
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("%.5lf\n", cpu_time_used);
    
    FILE *fw;
    
    strcpy(filename, (char *)argv[1]);
    strcat(filename, "_result.txt");
    fw = fopen(filename,"w");
    
    for (i = 0; i < n; i++){
        for (j = 0; j < n; j++)
            fprintf(fw, "%6.2f   ", C[i][j]);
        fprintf(fw, "\n");
    }
    fprintf (fw, "Done.\n");
}
