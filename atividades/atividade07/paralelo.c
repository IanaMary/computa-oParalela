#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"


double f(double x) {
   return x*x;
}



double regra_trapezio(double a, double b) {
	double h = (b - a);
	return  (h/2.0)*(f(a)+f(b));
}


double regra_trapezio_composta(double a, double b, int n){
	double h = (b-a)/n;
	double soma = 0.0;
	double x_0 = a;
	double x_1 = x_0 + h;
	for(int i = 0; i < n; i++){
		soma += regra_trapezio(x_0, x_1);
		
		x_0 = x_1;
		x_1 = x_0 + h;
	}
	return soma;
}

int main(int argc, char *argv[]) {
	// Valor da integral
	double integral, integPro ; 
	// Limites do intervalo
	double a, b;
	// Número de trapézios
	int n;
	// Base do trapézio
	double h;
	double x;
	int nTask, rank, iterPro = 0;
	double soma = 0.0;
   	MPI_Status status;

	a = atof(argv[1]);
	b = atof(argv[2]);
	n = atoi(argv[3]);
   
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nTask);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   
	iterPro = n/nTask;
	
	integPro = regra_trapezio_composta(a, b, iterPro);
	
	MPI_Reduce(&integPro, &soma, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if(rank == 0){
		integral = soma; 
		integral *= h;
		printf("%d trapézios, estimativa de %.2f a %.2f = %.5f\n", n, a, b, integral);
	}

	MPI_Finalize();

   return 0;
}
