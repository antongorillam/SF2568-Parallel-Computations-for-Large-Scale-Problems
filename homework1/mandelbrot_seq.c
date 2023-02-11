#include <stdio.h>
// #include <tgmath.h>
#include <complex.h>

#define N 256
#define B 2
#define W 256
#define H 256

#define RE_START -2
#define RE_END 1
#define IM_START -1
#define IM_END 1


// #define dx 2*B/(W)
// #define dy 2*B/(H)

unsigned int color[W][H];

int cal_pixel(double complex d, int b, int n) {

	int count = 0;
	double complex z = 0;

	while (cabs(z) <= b && count < n) {
		z = z*z + d;
		count += 1;
	}
	printf("count = %d\n", count);	
	return count;	
}

int main () { 
	for (int x = 0; x < W; x++) {
		double dreal = RE_START * ((double) x / (double) W) - RE_END - RE_START;

		for (int y = 0; y < H; y++) {
			double dimag = IM_START + ((double) y / (double) H) * (IM_END - IM_START);
			printf("%1d + (%1d / %1d) * (%1d - %1d)\n", IM_START, y, H, IM_END, IM_START);
			printf("%.5f\n", dimag);
			double complex d = dreal + I * dimag;  
			// printf("d = %.2f %+.2fi, ", creal(d), cimag(d));
			color[x][y] = cal_pixel(d, B, N);
			// printf("%hhn", color[x][y]);
		} 
	}
	FILE *fp;
	fp = fopen("color_c.txt","w");

	for (int j = 0; j < W; j++) {
		for (int i = 0; i < H; i++) {
			fprintf(fp, "%d ", color[j][i]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	return 1;
}

int main();
