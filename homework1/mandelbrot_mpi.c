#include <stdio.h>
#include <complex.h>
#include <mpi.h>
#include <stdlib.h>

#define N 80
#define B 2
#define W 100
#define H 100


int cal_pixel(double complex d, int b, int n) {

	unsigned int count = 1;
	double complex z = 0;

	while (cabs(z) <= b && count < n) {
		z = z*z + d;
		count += 1;
	}
	return count;	
}

int main (int argc, char **argv) { 

    int rank, size, tag, rc, i;
    MPI_Status status;

    rc = MPI_Init(&argc, &argv);
    rc = MPI_Comm_size(MPI_COMM_WORLD, &size);
    rc = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    tag = 42;

	int wp = W/(size-1); 
	int hp = H;
	int xoff = rank*(W/size);
	int yoff = 0;
	printf("rank: %d, size: %d\n", rank, size);


	if (rank == 0) { 
		unsigned int * color_part;
		color_part = malloc(wp*hp * sizeof(unsigned int));
		unsigned int color[W][H];
		for (int p = 1; p < size; p++) {
			// Do receive msg from processors 
			// printf("men kolla: %d\n", (int) wp*hp*sizeof(unsigned int));
			rc = MPI_Recv(color_part, wp*hp*sizeof(unsigned int), MPI_UNSIGNED, p, tag, MPI_COMM_WORLD, &status);
			// for (int x = 0; x < wp; x++) {
			// 	for (int y = 0; y < hp; y++) {
			// 		printf("color_part[%d + %d*%d] = %d\n", x, y, hp, color_part[x + y*hp]);
			// 		// color[x][y+hp] = 
			// 	}	
			// }
			
		}

		FILE *fp;
		fp = fopen("color_parallel_c.txt","w");
		for (int j = 0; j < W; j++) {
			for (int i = 0; i < H; i++) {
				fprintf(fp, "%d ", color[j + i*H]);
			}
			fprintf(fp, "\n");
		}
		fclose(fp);			
		printf("klar");
		return 1;

	} else {
		double dx = 2*((double) B/((double) W-1));
		double dy = 2*((double) B/((double) H-1));
		// unsigned int * color_part;
		// color_part = malloc(wp*hp * sizeof(unsigned int));
		unsigned int color_part[wp*hp];

		for (int x = 0; x < wp; x++) {
			double dreal = (x+xoff) * dx - B;
			for (int y = 0; y < hp; y++) {
				double dimag = (y+yoff) * dy - B;
				double complex d = dreal + I * dimag;  
				// printf("From rank %d: d = %.2f %+.2fi,  ", rank, creal(d), cimag(d));
				// printf("xoff: %d  \n", xoff);
				// printf("wp*hp: %d, x + y*hp: %d \n", wp*hp, x + y*hp );
				color_part[x + y*hp] = cal_pixel(d, B, N);
				// printf("color_part[%d + %d*%d] = %d\n", x, y, hp, color_part[x + y*hp]);	
				// printf("malloc(%d*%d * sizeof(unsigned int) = %d, size = %d\n", wp, hp, (int) (wp*hp * sizeof(unsigned int)), size);	
			} 
		}
		// for (int x = 0; x < wp; x++) {
		// 	for (int y = 0; y < hp; y++) {
		// 		printf("color_part[%d + %d*%d] = %d\n", x, y, hp, color_part[x + y*hp]);	
		// 	}	
		// }
		rc = MPI_Send(color_part, wp*hp*sizeof(unsigned int), MPI_UNSIGNED, 0, tag, MPI_COMM_WORLD);
	}

}

// int main();
