#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "mpi.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define EVENNUMBER(num) (num % 2 == 0 ? 1 : 0)

int main(int argc, char **argv) {

    int p, P, tag, N, I, L, global_i;
    tag = 42;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &p);
    /* Find problem size N from command line */
    
    if (argc < 2) {
        printf("No size N given\n");
        exit(0);
        // error_exit("No size N given");
    }
    
    N = atoi(argv[1]);
    // printf("p %d: N=%d\n", p, N);
    /* local size. Modify if P does not divide N */
    
    L = (int) N/P;

    int res = N % P;
    // Handles residual
    I = (p < res) ? L + 1 : L; 
    // /* random number generator initialization */
    srandom(p+1);
    double *x =  (double *) malloc((I*sizeof(double)));
    // /* data generation */
    for (int i = 0; i < I; i++) {
        x[i] = ((double) random())/(RAND_MAX);
    }
    // printf("p%d Original x: ", p);
    // for (int i = 0; i < I; i++) {
    //     printf("%f, ", x[i]);
    // }
    
    // printf("\n");
    // Odd-even transposition sort
    bool evenphase = 1;
    bool evenprocess = EVENNUMBER(p);

    // printf("p%d: %d\n", p, L);
    for (int step = 0; step < N; step++) {
        // Handles ghost points
        // printf("p%d, phase%d: %d\n", p, evenphase, I);
        if (P!=1) {

            if ((evenphase && evenprocess) || (!evenphase && !evenprocess)) {

                if ((EVENNUMBER(P) && (evenprocess || p <= P-3)) || (!EVENNUMBER(P) && (!evenprocess || p!=P-1))) {
                    double *received_elem = (double *) malloc(sizeof(double));
                    MPI_Send(&x[I-1], 1, MPI_DOUBLE, p+1, tag, MPI_COMM_WORLD);
                    MPI_Recv(received_elem, 1, MPI_DOUBLE, p+1, tag, MPI_COMM_WORLD, &status);               
                    if (*received_elem < x[I-1])
                        x[I-1] = *received_elem;
                    free(received_elem);
                }
            } else {
                if ((EVENNUMBER(P) && (!evenprocess || p >= 2)) || (!EVENNUMBER(P) && (evenprocess || p!=0))) {
                    double *received_elem = (double *) malloc(sizeof(double));
                    MPI_Recv(received_elem, 1, MPI_DOUBLE, p-1, tag, MPI_COMM_WORLD, &status);               
                    MPI_Send(&x[0], 1, MPI_DOUBLE, p-1, tag, MPI_COMM_WORLD);
                    if (*received_elem > x[0])
                        x[0] = *received_elem;
                    free(received_elem);
                }
            }
        }

        /* Inside the processor, we'll just perform bubble sort */
        for (int i = 0; i < I-1; i++) {
            // printf("p%d: %d %s, ", p, global_i, evennumber ? "true" : "false");
            if (x[i] > x[i+1]) {
                double temp = x[i];
                x[i] = x[i+1];
                x[i+1] = temp;
            }
        }
        // Change phase
        evenphase = (evenphase==1) ? 0 : 1;
    }
    
    // for (int i = 0; i < I; i++) {
    //     printf("%f, ", x[i]);
    // }
    
    FILE *fp;
    char out_str[50];
    // itoa(P_str, P, 11), itoa(N_str, N, 2);
    char P_str[11], N_str[9];
    sprintf(P_str, "%d", P);
    sprintf(N_str, "%d", N);

    strcpy(out_str, "sorted_example_P");
    strcat(out_str, P_str);
    strcat(out_str, "_N");
    strcat(out_str, N_str);
    strcat(out_str, ".txt");

    if (p==0){ // Master process
        fp = fopen(out_str, "w");
		for (int i = 0; i < I; i++) {
		    fprintf(fp, "%f, ", x[i]);
        }
        fclose(fp);		
        if (P!=1) // Only send if there are more than 1 processors
            MPI_Send("hi", 2, MPI_CHAR, 1, tag, MPI_COMM_WORLD);
    } else {
        char message[2]; // Nonesense message
        MPI_Recv(message, 2, MPI_CHAR, p-1, tag, MPI_COMM_WORLD, &status);
        fp = fopen(out_str, "a");
        // fprintf(fp, "\n");
        if (p!=P-1) { // If it's not the last processor, keep sending the signals forward
            MPI_Send(message, 2, MPI_CHAR, p+1, tag, MPI_COMM_WORLD);
        }
        fclose(fp);
    }
    free(x);
    printf("Process %d finished\n", p);
    MPI_Finalize();
    exit(0);

}

