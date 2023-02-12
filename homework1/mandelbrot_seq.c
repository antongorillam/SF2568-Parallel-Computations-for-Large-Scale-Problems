#include <stdio.h>
#include <complex.h>

#define N 256
#define B 2
#define W 256
#define H 256

unsigned int color[W*H];

int cal_pixel(double complex d, int b, int n) {

	unsigned int count = 1;
	double complex z = 0;

	while (cabs(z) <= b && count < n) {
		// printf("cabs(z) %.2f, z = %.2f %+.2fi, count = %d\n", cabs(z), creal(z), cimag(z), count);
		z = z*z + d;
		count += 1;
	}
	// printf("count = %d\n", count);	
	return count;	
}

int main () { 

	double dx = 2*((double) B/((double) W-1));
	double dy = 2*((double) B/((double) H-1));

	for (int x = 0; x < W; x++) {
		double dreal = x * dx - B;
		for (int y = 0; y < H; y++) {
			double dimag = y * dy - B;
			double complex d = dreal + I * dimag; 
			printf("dy %.2f, dx %.2f\n", dy, dx); 
			printf("From rank: d = %.2f %+.2fi, abs: %.2f\n", creal(d), cimag(d), cabs(d));
			color[x + y*H] = cal_pixel(d, B, N);
			// printf("W*H: %d, x + y*H: %d, color[x + y*H]: %d\n", W*H, x + y*H, color[x + y*H]);
		} 
	}
	// for (int x = 0; x < W; x++) {
	// 	for (int y = 0; y < H; y++) {
	// 		if (color[x + y*H] != -1) {printf("color_part[%d + %d*%d] = %d\n", x, y, H, color[x + y*H]);}	
	// 	}	
	// }
	FILE *fp;
	fp = fopen("color_seq_c.txt","w");

	for (int j = 0; j < W; j++) {
		for (int i = 0; i < H; i++) {
			fprintf(fp, "%d ", color[j + i*H]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	
	return 1;
}

int main();
