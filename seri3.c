#include <math.h>
#include <stdio.h>
#include <time.h>
#define KE 201
int main()
{
	double ex[KE];
	double hy[KE];
	double T;
	double t0;
	double spread;
	double pulse;
	float epsilon;
	float cb[KE];
	int n;
	int k; 
	int kc;
	int kstart;
	int NSTEPS; 
	FILE *ifp;
	char inputFilename[] = "ex3.txt";
	FILE *ofp;
	char outputFilename[] = "hy3.txt";
	clock_t start = clock();
	clock_t end = 0;
	for (k=0; k<KE; k++)
	{
		cb[k]=.5;
		ex[k]=0;
		hy[k]=0.;
	}
		printf ("dielectric start at --> ");
		scanf("%f", &kstart);
		printf ("epsilon --> ");
		scanf ("%f", &epsilon);
		for (k=kstart; k<=KE; k++)
		{
			cb[k]= .5/epsilon;
		}
	kc=KE/2;
	t0=40.0;
	spread=12;
	T=0;
	NSTEPS=1;
	
	while (NSTEPS>0)
	{
		printf("NSTEPS --> ");
		scanf("%d", &NSTEPS);
		printf("NSTEPS = %d\n", NSTEPS);
		n=0;
		for (n=1; n<=NSTEPS; n++)
		{
			T=T+1;
				for (k=0; k<KE; k++)
			{
			 	ex[k]=ex[k]+cb[k]*(hy[k-1]-hy[k]);
			}
			pulse=exp(-.5*(pow((t0-T)/spread, 2.0)));
		    ex[5]=ex[5]+pulse;
			for (k=1; k<KE-1; k++)
			{
				hy[k]=hy[k]+.5*(ex[k]-ex[k+1]);
			}
		}
		/*printf("k\t ex[k]\t hy[k]\n"); 
		for (k=1; k<=KE; k++)
		{
			printf("%3d | %f | %f\n", k, ex[k], hy[k]);
		}*/
		end=clock();

		ifp = fopen(inputFilename, "w");
		fprintf(ifp, "NSTEPS = %d\n", NSTEPS);
		fprintf(ifp, "t0-T = %5.0f | ex[kc] = %f\n", t0-T, ex[kc]);
		fprintf(ifp, " ex[k]\n");
		for (k=1; k<=KE; k++)
		{
			fprintf(ifp, " %3d | %f\n", k, ex[k]);
		}
		fprintf(ifp, "Waktu eksekusi = %f detik.\n",((double)end-(double)start)/CLOCKS_PER_SEC);
		fclose(ifp);
		ofp = fopen(outputFilename, "w");
		fprintf(ofp, "NSTEPS = %d\n", NSTEPS);
		fprintf(ofp, "t0-T = %5.0f | ex[kc] = %f\n", t0-T, ex[kc]);
		fprintf(ofp, "k\t hy[k]\n");
		for (k=1; k<=KE; k++)
		{
			fprintf(ofp, "%3d | %f\n", k, hy[k]);
		}
		fprintf(ofp, "Waktu eksekusi = %f detik.\n", ((double)end-(double)start)/CLOCKS_PER_SEC);
		fclose(ofp);
		printf("T = %5.0f\n", T);
		printf("Waktu eksekusi = %f detik.\n", ((double)end-(double)start)/CLOCKS_PER_SEC);
		break;
	}
	return 0;
}