/* simulation of a sinusoidal wave hitting a dielectric medium*/
#define _XOPEN_SOURCE
#include <math.h>
#include <stdio.h>
#include <time.h>
#define KE 201
int main ()
{
	double ex[KE];
	double hy[KE]; 
	double T;
	double t0;
	double spread;
	double pulse;
	float epsilon;
	float cb[KE];
	float ddx;
	float dt;
	float freq_in;
	int n;
	int k; 
	int kc;
	int kstart;
	int NSTEPS; 
	FILE *ifp;
	char inputFilename[] = "ex4.txt";
	FILE *ofp;
	char outputFilename[] = "hy4.txt";
	clock_t start = clock();
	clock_t end = 0;
	for (k=0; k<KE; k++)
	{
		ex[k]=0;
		hy[k]=0.;
		cb[k] = .5;
	}
	kc=KE/2;
	t0=40.0;
	spread=12;
	ddx = .01;
	dt = ddx/(2*3e8);
	T=0;
	NSTEPS=1;
	
	printf ("Input freq (MHz) --> ");
	if (scanf ("%f", &freq_in)==1)
	{
	freq_in = freq_in*1e6;
	printf ("%f\n", freq_in);
	}
	else
	{
		printf("Failed to read integer.\n");
	}
	
	printf ("Dielectric starts at --> ");
	if (scanf ("%d", &kstart)==1)
	{printf("Cell = %d\n", kstart);}
	else
	{
		printf("Failed to read integer.\n");
	}	
	printf ("Epsilon --> ");
	if (scanf ("%f", &epsilon)==1)
	{printf("Dielectric constant = %6.2f\n", epsilon);}
	else
	{
		printf("Failed to read integer.\n");
	}	
	for (k=kstart;k<=KE; k++)
	{
		cb[k] = .5/epsilon;
	}
		
	while (NSTEPS>0)
	{
		printf("NSTEPS --> ");
		if (scanf("%d", &NSTEPS)==1)
		{printf("NSTEPS = %d\n", NSTEPS);}
		else
		{
			printf("Failed to read integer.\n");
		}		
		n=0;
		for (n=1; n<=NSTEPS; n++)
		{
			T=T+1;
			for (k=0; k<KE; k++)
			{
				ex[k]=ex[k]+cb[k]*(hy[k-1]-hy[k]);
			}
			pulse=sin(2*M_PI*freq_in*dt*T);
			ex[5]=ex[5]+pulse;
			for (k=0; k<KE-1; k++)
			{
				hy[k]=hy[k]+.5*(ex[k]-ex[k+1]);
			}
		}
		/*printf("k\t ex[k]\t hy[k]\n"); 
		for (k=1; k<=KE;   k++)
		{
			printf("%3d | %f | %f\n", k, ex[k], hy[k]);
		}*/
		end=clock();
		ifp = fopen(inputFilename, "w");
		fprintf(ifp, "# NSTEPS = %d\n", NSTEPS);
		fprintf(ifp, "# t0-T = %5.0f | ex[kc] = %f\n", t0-T, ex[kc]);
		fprintf(ifp, "# k\t ex[k]\n");
		for (k=0; k<KE; k++)
		{
			fprintf(ifp, "%3d %f\n", k, ex[k]);
		}
		fprintf(ifp, "# Waktu eksekusi = %f detik.\n",((double)end-(double)start)/CLOCKS_PER_SEC);
		fclose(ifp);
		ofp = fopen(outputFilename, "w");
		fprintf(ofp, "# NSTEPS = %d\n", NSTEPS);
		fprintf(ofp, "# t0-T = %5.0f | ex[kc] = %f\n", t0-T, ex[kc]);
		fprintf(ofp, "# k\t hy[k]\n");
		for (k=0; k<KE; k++)
		{
			fprintf(ofp, "%3d %f\n", k, hy[k]);
		}
		fprintf(ofp, "# Waktu eksekusi = %f detik.\n",((double)end-(double)start)/CLOCKS_PER_SEC);
		fclose(ofp);
		printf("T = %5.0f\n", T);
		printf("Waktu eksekusi = %f detik.\n",((double)end-(double)start)/CLOCKS_PER_SEC);
		break;
	}
	return 0;
}

