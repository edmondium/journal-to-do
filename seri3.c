//Simulation of a pulse hitting a dielectric medium
#define _XOPEN_SOURCE
#include <math.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#define KE 20001
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
	clock_t start;
	clock_t end;
	int numThreads;
	#pragma omp parallel default(none) shared(numThreads)
	{
		#pragma omp master
		{
			#ifdef _OPENMP
			numThreads = omp_get_num_threads();
			#else
			numThreads = 1;
			#endif
		}
	}	
	//Initialize to free space
	#pragma omp parallel for
	for (k=0; k<KE; k++)
	{
		cb[k]=.5;
		ex[k]=0;
		hy[k]=0.;
	}
	printf ("Dielectric starts at --> ");
	if (scanf("%d", &kstart)==1)
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
		printf("Failed to read float.\n");
	}
	
	for (k=kstart; k<=KE; k++)
	{
		cb[k]= .5/epsilon;
	}
	kc=KE/2;
	t0=4000.0;
	spread=1200;
	T=0;
	NSTEPS=1;
	
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
		#ifdef _OPENMP
		double starttime=omp_get_wtime();
		#else
		start=clock();
		#endif		
		for (n=1; n<=NSTEPS; n++)
		{
			T=T+1;
			#pragma omp parallel for
			for (k=0; k<KE; k++)
			{
			 	ex[k]=ex[k]+cb[k]*(hy[k-1]-hy[k]);
			}
			pulse=exp(-.5*(pow((t0-T)/spread, 2.0)));
		    ex[5]=ex[5]+pulse;
			#pragma omp parallel for
			for (k=0; k<KE-1; k++)
			{
				hy[k]=hy[k]+.5*(ex[k]-ex[k+1]);
			}
		}
		#ifdef _OPENMP
		double endtime=omp_get_wtime();
		#else		
		end=clock();
		#endif

		ifp = fopen(inputFilename, "w");
		fprintf(ifp, "# NSTEPS = %d\n", NSTEPS);
		fprintf(ifp, "# T = %5.0f | ex[5] = %f | pulse = %f\n", T, ex[5], pulse);
		#ifdef _OPENMP
		fprintf(ifp, "# Time taken = %f second on %i threads.\n", endtime-starttime, numThreads);
		#else
		fprintf(ifp, "# Waktu eksekusi = %f detik pada %i thread.\n", ((double)end-(double)start)/CLOCKS_PER_SEC, numThreads);
		#endif		
		fprintf(ifp, "# k\t ex[k]\n");
		#pragma omp for
		for (k=0; k<KE; k++)
		{
			fprintf(ifp, " %3d %f\n", k, ex[k]);
		}
		fclose(ifp);

		ofp = fopen(outputFilename, "w");
		fprintf(ofp, "# NSTEPS = %d\n", NSTEPS);
		fprintf(ofp, "# T = %5.0f | ex[5] = %f | pulse = %f\n", T, ex[5], pulse);
		#ifdef _OPENMP
		fprintf(ofp, "# Time taken = %f second on %i threads.\n", endtime-starttime, numThreads);
		#else
		fprintf(ofp, "# Waktu eksekusi = %f detik pada %i thread.\n", ((double)end-(double)start)/CLOCKS_PER_SEC, numThreads);
		#endif		
		fprintf(ofp, "# k\t hy[k]\n");
		#pragma omp for
		for (k=0; k<KE; k++)
		{
			fprintf(ofp, "%3d %f\n", k, hy[k]);
		}
		fclose(ofp);

		printf("T = %5.0f\n", T);
		#ifdef _OPENMP
		printf("Time taken = %f second on %i threads.\n", endtime-starttime, numThreads);
		#else
		printf("Waktu eksekusi = %f detik pada %i thread.\n",((double)end-(double)start)/CLOCKS_PER_SEC, numThreads);
		#endif		
	break;
	}
	return 0;
}