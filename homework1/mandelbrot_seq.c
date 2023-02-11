#include <stdio.h>
// #include <tgmath.h>
#include <complex.h>

#define N 32
#define B 2
#define W 128
#define H 128
#define dx 2*B/(W-1)
#define dy 2*B/(H-1)

unsigned char color[W*H];

int cal_pixel(double complex d, int b, int n) {

	int count = 1;
	double complex z = 0;
	while (cabs(z) < b && count < n) {
		z = z*z + d;
		count += 1;
	}

	return count;	
}

int main () { 
	for (int x = 0; x < W; x++) {
		double dreal = x * dx - B;
		for (int y = 0; y < H; y++) {
			double dimag = y * dy - B;
			double complex d = dreal + I * dimag;  
			color[x+y*H] = cal_pixel(d, B, N);
		} 
	}



	FILE *fp;
	fp = fopen("color.txt","w");

	// for (j = 0; j < N; j++) {
	// 	for (i = 0; i < M; i++) {
	// 		fprintf(fp, "%hhu ", color[i+j*M]);
	// 	}
	// 	fprintf(fp, "\n");
	// }
	// fclose(fp);
	
	return 1;
}

main();
