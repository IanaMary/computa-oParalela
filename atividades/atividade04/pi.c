#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

double monteCarloPi(int n){
	
	double x = 0;
	double y = 0;
	double acertos = 0;
	for(int i = 0; i < n; i++) {
		x = (double)(rand())/(double)(RAND_MAX);
		y = (double)(rand())/(double)(RAND_MAX);

		if((x*x+y*y) < 1) {
			acertos++;
		}	
	}

	return acertos;
	
}

int main(int argc, char* argv[]){
	
	srand(time(NULL));
	
	double mcp, valorAcePro, soma = 0.0;
	int n, nIterPro, nTask, rank = 0;
	MPI_Status status;
	
	MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nTask);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		
	n = atoi(argv[1]);
	nIterPro = n/nTask;
	
	valorAcePro = monteCarloPi(nIterPro);
	
	MPI_Reduce(&valorAcePro, &soma, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	
	
	if(rank == 0){
		  
		mcp = (4.0*(soma/n));
		printf("pi = %.5lf", mcp);
	}

	MPI_Finalize();
	
	return 0;
	
}

