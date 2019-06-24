#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"
#include <omp.h>

double f(double x) {
   double return_val = 0.0;
   return_val = cos(x * log(1.0/x));
   return return_val;
}



double g(double inicio, double fim, double h, double x, int n) {
	double integral=0;
	

	int i;
	#pragma omp parallel shared(h, n, inicio, fim) private(i, x) reduction(+:integral)
	{
		x = inicio+((fim-inicio)/omp_get_num_threads())*omp_get_thread_num();
		integral+=(f(x)+f(inicio+((fim-inicio)/omp_get_num_threads())*(omp_get_thread_num()+1)))/2;
		for (i = 1; i < (n/omp_get_num_threads()); i++) {
			x += h;
			integral += f(x);
		}
	}
	integral *= h;
    return integral; 
}

int main(int argc, char *argv[]) {
   // Valor da integral
   double integralTotal, integralProcesso;

   // Limites do intervalo
   double a, b;
   // Número de trapézios
   int nTrapezios;
   // Base do trapézio
   double h, x;
   int i;

   int nTask, rank;

   double info[3];
   double inicio, fim;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &nTask);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   nTrapezios = atoi(argv[3]);

   inicio = rank * ((nTrapezios)/nTask);
   fim = (rank + 1) *  ((nTrapezios)/nTask);


   if(rank == 0){
      a = atof(argv[1]);
      b = atof(argv[2]);
  
      h = (b - a) / nTrapezios;

      x = a;
      

      info[0] = a;
      info[1] = h;
      info[2] = nTrapezios/nTask;

      MPI_Bcast(info, 3, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      
      integralProcesso = g(info[0]+inicio*info[1], (info[0]+inicio*info[1])+info[2]*info[1], info[1], x, info[2]);
      
      MPI_Reduce(&integralProcesso, &integralTotal, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

      printf("%d trapézios, estimativa de %.2f a %.2f = %.5f\n", nTrapezios, a, b, integralTotal);


   }
   else{
	   
	
      MPI_Bcast(info, 3, MPI_DOUBLE, 0, MPI_COMM_WORLD);

      integralProcesso = g(info[0]+inicio*info[1], (info[0]+inicio*info[1])+info[2]*info[1], info[1], x, info[2]);

      MPI_Reduce(&integralProcesso, &integralTotal, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
   }


   MPI_Finalize();
   return 0;
}

