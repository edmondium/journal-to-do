//1D FDTD simulation in free space
#define _XOPEN_SOURCE
#include <math.h>
#include <stdio.h>
#include <omp.h>
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
	int n;
	int k; 
 	int kc;
	int NSTEPS;
	FILE *ifp;
	char inputFilename[] = "ex.txt";
	FILE *ofp;
	char outputFilename[] = "hy.txt";
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
	for (k=0; k<KE; k++)
	{
		ex[k]=0;
		hy[k]=0.;
	}
	kc=KE/2;
	t0=40.0;
	spread=12;
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
		#pragma omp parallel
		{
		#pragma omp single
		for (n=1; n<=NSTEPS; n++)
		{
			T=T+1;
			//#pragma omp simd
			for (k=1; k<KE; k++)
			{
				ex[k]=ex[k]+.5*(hy[k-1]-hy[k]);
			}
			pulse=exp(-.5*(pow((t0-T)/spread, 2.0)));
			ex[kc]=pulse;
			//#pragma omp simd
			for (k=0; k<KE-1; k++)
			{
				hy[k]=hy[k]+.5*(ex[k]-ex[k+1]);
			}
		}
		}
		#ifdef _OPENMP
		double endtime=omp_get_wtime();
		#else
		end=clock();
		#endif
	/*	printf("k\t ex[k]\t hy[k]\n"); 
		for (k=1; k<=KE; k++)
		{
			printf("%3d | %f | %f\n", k, ex[k], hy[k]);
		}*/
		
		ifp = fopen(inputFilename, "w");
		fprintf(ifp, "# NSTEPS = %d\n", NSTEPS);
		fprintf(ifp, "# t0-T = %5.0f | ex[kc] = %f\n", t0-T, ex[kc]);
		fprintf(ifp, "# k\t ex[k]\n");
		//#pragma omp parallel for
		for (k=0; k<KE; k++)
		{
			fprintf(ifp, "%3d %f\n", k, ex[k]);
		}
		//#ifdef _OPENMP
		//fprintf(ofp, "# Time taken = %f second on %i threads.\n", endtime-starttime, numThreads);
		//#else		
		fprintf(ifp, "# Waktu eksekusi = %f detik pada %i thread.\n", ((double)end-(double)start)/CLOCKS_PER_SEC, numThreads);
		//#endif
		fclose(ifp);

		ofp = fopen(outputFilename, "w");
		fprintf(ofp, "# NSTEPS = %d\n", NSTEPS);
		fprintf(ofp, "# t0-T = %5.0f | ex[kc] = %f\n", t0-T, ex[kc]);
		fprintf(ofp, "# k\t hy[k]\n");
		//for (k=1; k<=KE; k++)
		for (k=0; k<KE; k++)
		{
			fprintf(ofp, "%3d %f\n", k, hy[k]);
		}
		#ifdef _OPENMP
		fprintf(ofp, "# Time taken = %f second on %i threads.\n", endtime-starttime, numThreads);
		#else
		fprintf(ofp, "# Waktu eksekusi = %f detik pada %i thread.\n", ((double)end-(double)start)/CLOCKS_PER_SEC, numThreads);
		#endif
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