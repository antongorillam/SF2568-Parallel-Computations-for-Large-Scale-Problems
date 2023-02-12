#include <stdio.h>
#include <complex.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h>

#define N 256
#define B 2
#define W 2048
#define H 2048


int cal_pixel(double complex d, int b, int n) {

	int count = 0;
	double complex z = 0;

	while (cabs(z) < b && count < n) {
		z = z*z + d;
		// printf("cabs(z) %.2f, z = %.2f %+.2fi, count = %d\n", cabs(z), creal(z), cimag(z), count);
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

	int wp = W/size; 
	int hp = H;
	printf("rank: %d, size: %d  \n", rank, size);

	double dx = 2*((double) B/((double) W-1));
	double dy = 2*((double) B/((double) H-1));
	unsigned int * color_part;
	color_part = malloc(wp*hp * sizeof(unsigned int));
	int xoff = rank*(W/size);
	int yoff = 0;

	for (int x = 0; x < wp; x++) {
		double dreal = (x+xoff) * dx - B;
		for (int y = 0; y < hp; y++) {
			double dimag = (y+yoff) * dy - B;
			double complex d = dreal + I * dimag;
			color_part[x + y*hp] = cal_pixel(d, B, N);
		} 
	}
	
	

	rc = MPI_Send(color_part, wp*hp, MPI_UNSIGNED_CHAR, 0, tag, MPI_COMM_WORLD);
	free(color_part);
	printf("Process %d finished\n", rank);

	if (rank == 0) { 
		unsigned char colors_tot[H * W];
		MPI_Gather(color_part, wp * H, MPI_UNSIGNED_CHAR, &colors_tot,
			wp * H, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
		// unsigned int *colors_tot;
		// colors_tot = malloc(wp*hp * sizeof(unsigned int));
		// for (int x = 0; x < wp; x++) {
		// 	for (int y = 0; y < hp; y++) {
		// 		colors_tot[x + xoff + y*hp] = color_part[x + y*hp];
		// 	}	
		// }
		// free(color_part);
		// unsigned int *color_part;
		// color_part = malloc(wp*hp * sizeof(unsigned int));

		// // unsigned int colors_tot[wp*hp];
		// for (int p = 1; p < size; p++) {
		// 	// Do receive msg from processors 
		// 	rc = MPI_Recv(color_part, wp*hp, MPI_UNSIGNED_CHAR, p, tag, MPI_COMM_WORLD, &status);
		// 	int xoff = (p)*(W/(size));
		// 	for (int x = 0; x < wp; x++) {
		// 		for (int y = 0; y < hp; y++) {
		// 			colors_tot[x + xoff + y*hp] = color_part[x + y*hp];
		// 		}	
		// 	}
		// 	free(color_part);	
		FILE *fp;
		char file_name[30];
		char size_str[12];
		sprintf(size_str, "%d", size);
		strcpy(file_name, "color_parallel_new_dardel_size_");
		strcat(file_name, size_str);

		fp = fopen(file_name, "w");
		for (int j = 0; j < W; j++) {
			for (int i = 0; i < H; i++) {
				fprintf(fp, "%hhu ", colors_tot[j + i*H]);
			}
			fprintf(fp, "\n");
		}
		// free(colors_tot);
		fclose(fp);			
		printf("Size: %s Finished\n", size_str);
		printf("Saved as: %s \n", file_name);
		}
	MPI_Finalize();
	return 0;
}

// int main();
