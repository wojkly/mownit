//
// Created by wojkly on 4/4/21.
//
//
// Created by wojkly on 3/12/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <gsl/gsl_blas.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <time.h>
#include <sys/times.h>
#include <sys/resource.h>
#include <unistd.h>

clock_t start, end;
struct tms st_cpu, en_cpu;

void timer_start(){ start = times(&st_cpu);}
void timer_end(){ end = times(&en_cpu);}

//void write_timer_to_file(char* operation_name, FILE* report_file){
float write_timer(){
    int clock_tics = sysconf(_SC_CLK_TCK);

    double real_time;
    real_time = (double ) (end - start)/ clock_tics;
//    fprintf(report_file, "%s:\n%s %f\n%s %f\n%s %f\n",operation_name,"Real time: ",real_time,"User time: ",user_time,"System time: ",system_time);
    printf(" %f ",real_time);
    return real_time;
}

void naive_multiply(double **A, double **B, double **C, int n){
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            for (int k = 0; k < n; ++k)
                C[i][j] += A[i][k]*B[k][j];
}
void free_matrix(double **a, int n);

void improved_multiply(double **A, double **B, double **C, int n){
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < n; ++k)
            for (int j = 0; j < n; ++j)
                C[i][j] += A[i][k]*B[k][j];
}

double **create_matrix(int n){
    double **A = calloc(n, sizeof(double *));
    for (int i = 0; i < n; ++i) {
        A[i] = calloc(n, sizeof(double ));
        for (int j = 0; j < n; ++j) {
            A[i][j] = ((double)rand()/(double)(RAND_MAX)) * 5.0;
        }
    }
    return A;
}
double **create_matrix_zeros(int n){
    double **A = calloc(n, sizeof(double *));
    for (int i = 0; i < n; ++i) {
        A[i] = calloc(n, sizeof(double ));
        for (int j = 0; j < n; ++j) {
            A[i][j] = 0.0;
        }
    }
    return A;
}
void free_matrix(double **A, int n){
    for (int i = 0; i < n; ++i) {
        free(A[i]);
    }
    free(A);
}

void test_matrices(){
    char *filename1 = "data_c.csv";
    FILE * f1;
    f1 = fopen(filename1, "w");

//    int first = 25;
//    int interval = 25;
//    int last = 251;
    int first = 100;
    int interval = 100;
    int last = 1001;

    fprintf(f1,"length,t,time");
    float time;
    for (int i = first; i < last; i += interval) {
        for (int j = 0; j < 10; ++j) {
            printf("\n %d ",i);
            double **A, **B, **C;
            A = create_matrix(i);
            B = create_matrix(i);
            C = create_matrix_zeros(i);

            //test naive
            timer_start();
            naive_multiply(A,B,C,i);
            timer_end();
            time = write_timer();
            fprintf(f1,"\n%d,%s,%f",i,"n",time);

            //test better
            free_matrix(C,i);
            C = create_matrix_zeros(i);
            timer_start();
            improved_multiply(A,B,C,i);
            timer_end();
            time = write_timer();
            fprintf(f1,"\n%d,%s,%f",i,"i",time);

            free_matrix(A,i);
            free_matrix(B,i);
            free_matrix(C,i);

            //test lib function
            double *a = calloc(i*i, sizeof(double ));
            double *b = calloc(i*i, sizeof(double ));
            double *c = calloc(i*i, sizeof(double ));
            for (int k = 0; k < i*i; ++k) {
                a[i] = ((double)rand()/(double)(RAND_MAX)) * 5.0;
                b[i] = ((double)rand()/(double)(RAND_MAX)) * 5.0;
                c[i] = 0.0;
            }
            gsl_matrix_view A2 = gsl_matrix_view_array(a,i,i);
            gsl_matrix_view B2 = gsl_matrix_view_array(b,i,i);
            gsl_matrix_view C2 = gsl_matrix_view_array(c,i,i);
            timer_start();
            gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,
                            1.0, &A2.matrix, &B2.matrix,
                            0.0, &C2.matrix);
            timer_end();
            time = write_timer();
            fprintf(f1,"\n%d,%s,%f",i,"b",time);

            free(a);
            free(b);
            free(c);
        }
    }
    fclose(f1);
}


int main (void)
{
    srand((unsigned int)time(NULL));

    test_matrices();

    return 0;
}
