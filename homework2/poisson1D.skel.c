/* Reaction-diffusion equation in 1D
 * Solution by Jacobi iteration
 * simple MPI implementation
 *
 * C Michael Hanke 2006-12-12
 */

#include <stdlib.h>
#include <stdio.h>
/* Use MPI */
#include "mpi.h"


#define MIN(a, b) ((a) < (b) ? (a) : (b))


/* define problem to be solved */
#define N 6   /* number of inner grid points */
#define K 10 /* number of iterations */
#define h 1.0 / ((double) N+1)

/* implement coefficient functions */
extern double r(const double x){
    return x;
};
extern double f(const double x){
    return 4;
};
/* We assume linear data distribution. The formulae according to the lecture
   are:
      L = N/P; (number of elements per processor)
      R = N%P; (residual elements)
      I = (N+P-p-1)/P;    (number of local elements)
      n = p*L+MIN(p,R)+i; (global index for given (p,i)
   Attention: We use a small trick for introducing the boundary conditions:
      - The first ghost point on p = 0 holds u(0);
      - the last ghost point on p = P-1 holds u(1).
   Hence, all local vectors hold I elements while u has I+2 elements.
*/

int main(int argc, char *argv[])
{
/* local variable */
/* Initialize MPI */

    int p, P, tag, n;
    tag = 42;

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &p);

    if (N < P) {
        fprintf(stdout, "Too few discretization points...\n");
        exit(1);
    }
/* Compute local indices for data distribution */
    double I = ((double) (N+P-p-1)/ (double) P);
    int L = (double) N/ (double) P;
    int R = N % P;
    // int n = p*L+MIN(p,R)+i;
    printf("p: %d, I: %.2f, L: %d, R: %d\n", p, I, L, R);

    double *unew;
    double *u;

    /* arrays */
    unew = (double *) malloc(I*sizeof(double));
    /* Note: The following allocation includes additionally:
        - boundary conditins are set to zero
        - the initial guess is set to zero 
    */
    u = (double *) calloc(I+2, sizeof(double));

    if (p==0) {
        u[L-2] = 69.0;
    }
    // unew[(int) I] = 10;
    // for (int i = 0; i < I+2; i++) {
    //     printf("unew[%d]: %.2f \n", i, unew[i]); 
    // }
    

    /* Jacobi iteration */
    for (int step = 0; step < K; step++) {
        /* RB communication of overlap */
        
        // Buffer these elements
        // u[L-1] = (double *) calloc(1, sizeof(double));
        // u[0] = (double *) calloc(1, sizeof(double));         
        if (p == 0) {
            MPI_Send(&u[L-2], 1, MPI_DOUBLE, p+1, tag, MPI_COMM_WORLD);
            MPI_Recv(&u[L-1], 1, MPI_DOUBLE, p+1, tag, MPI_COMM_WORLD, &status);
        } else if (p == P-1 && p % 2 == 0) { // last processor and even
            MPI_Send(&u[1], 1, MPI_DOUBLE, p-1, tag, MPI_COMM_WORLD);
            MPI_Recv(&u[0], 1, MPI_DOUBLE, p-1, tag, MPI_COMM_WORLD, &status);
        } else if (p == P-1 && p % 2 != 0) { // last processor and odd
            MPI_Recv(&u[0], 1, MPI_DOUBLE, p-1, tag, MPI_COMM_WORLD, &status);
            MPI_Send(&u[1], 1, MPI_DOUBLE, p-1, tag, MPI_COMM_WORLD);                    
        } else if (p % 2 == 0) { // Even: red
            MPI_Send(&u[L-2], 1, MPI_DOUBLE, p+1, tag, MPI_COMM_WORLD);
            MPI_Recv(&u[L-1], 1, MPI_DOUBLE, p+1, tag, MPI_COMM_WORLD, &status);
            MPI_Send(&u[1], 1, MPI_DOUBLE, p-1, tag, MPI_COMM_WORLD);
            MPI_Recv(&u[0], 1, MPI_DOUBLE, p-1, tag, MPI_COMM_WORLD, &status);
        } else { // Odd: black
            MPI_Recv(&u[0], 1, MPI_DOUBLE, p-1, tag, MPI_COMM_WORLD, &status);
            MPI_Send(&u[1], 1, MPI_DOUBLE, p-1, tag, MPI_COMM_WORLD);
            MPI_Recv(&u[L-1], 1, MPI_DOUBLE, p+1, tag, MPI_COMM_WORLD, &status);
            MPI_Send(&u[L-2], 1, MPI_DOUBLE, p+1, tag, MPI_COMM_WORLD);
        }
        
        // /* local iteration step */
	    for (int i = 0; i < I; i++) {
            n = p*L+MIN(p,R)+i;
            unew[i] = (u[i]+u[i+2]-h*h*f((double) n * h))/(2.0-h*h*r((double) n * h));
            // printf("unew[i]: %.2f\n", unew[i]);
        }
	    for (int i = 0; i < I; i++) {
            u[i+1] = unew[i]; 
        }

        // Test if passing parameters works
        // if (p==0){
        //     printf("For p: %d, u[0]: %.2f \n", p, u[0]);
        // }
        

    }

/* output for graphical representation */
/* Instead of using gather (which may lead to excessive memory requirements
   on the master process) each process will write its own data portion. This
   introduces a sequentialization: the hard disk can only write (efficiently)
   sequentially. Therefore, we use the following strategy:
   1. The master process writes its portion. (file creation)
   2. The master sends a signal to process 1 to start writing.
   3. Process p waites for the signal from process p-1 to arrive.
   4. Process p writes its portion to disk. (append to file)
   5. process p sends the signal to process p+1 (if it exists).
*/

/* That's it */
    MPI_Finalize();
    exit(0);
}
