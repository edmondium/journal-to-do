//1D FDTD simulation of a lossy dielectric medium
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
	float ca[KE];
	float cb[KE];
	float ddx;
	float dt;
	float epsz;
	float sigma;
	float eaf;
	float freq_in;
	int n;
	int k;
	int kc;
	int kstart;
	int NSTEPS;
	FILE *ifp;
	char inputFilename[] = "ex5.txt";
	FILE *ofp;
	char outputFilename[] = "hy5.txt";
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
	#pragma omp parallel for
	for (k=0; k<KE; k++)
	{
		ex[k]=0;
		hy[k]=0.;
		ca[k] = 1.;
		cb[k] = .5;
	}
	kc=KE/2;
	t0=4000.0;
	spread=1200;
	epsz = 8.85419e-12;
	ddx = .01;
	dt = ddx/(2*3e8);
	T=0;
	NSTEPS=1;

	printf ("Input freq (MHz) --> ");
	if (scanf ("%f",&freq_in)==1)
	{
	freq_in = freq_in*1e6;
	printf ("%f\n", freq_in);
	}
	else
	{
		printf("Failed to read float.\n");
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
		printf("Failed to read float.\n");
	}	
	printf ("Conductivity --> ");
	if (scanf ("%f",&sigma)==1)
	{eaf = dt*sigma/(2*epsz*epsilon);
	printf ("eaf = %f\n",eaf);}
	else
	{
		printf("Failed to read float.\n");
	}
	
	for (k=kstart; k<=KE; k++)
	{
		ca[k] = (1.- eaf) / (1 + eaf);
		cb[k] = .5/(epsilon*(1 + eaf));
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
				ex[k]=ca[k]*ex[k]+cb[k]*(hy[k-1]-hy[k]);
			}
			pulse=sin(2*M_PI*freq_in*dt*T);
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
			fprintf(ifp, "%3d %f\n", k, ex[k]);
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