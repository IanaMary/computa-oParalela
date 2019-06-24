#include "mpi.h"
#include <omp.h>
#include "operacoesSobreMatrizes.h"

void multiplicaMatrizes(double *pMatResul, double *pMat1, double *pMat2, int pLinhaMat1, int pLinhaMat2, int pColunaMat1, int pColunaMat2){	    
	
	int i, j, k;
	
	#pragma omp parallel for shared(pMat1, pMat2, pLinhaMat1, pColunaMat2, pColunaMat1, pMatResul) private(i, j, k) schedule(static)
	for(i = 0; i < pLinhaMat1; i++){
		for(j = 0; j < pColunaMat2; j++){
			pMatResul[i*pColunaMat2 + j] = 0;
			for(int k = 0; k < pColunaMat1; k++){
				pMatResul[i*pColunaMat2 + j] = pMatResul[i*pColunaMat2 + j] + pMat1[i*pColunaMat1 + k] * pMat2[k*pColunaMat2 + j];
			}
		}
	}	
}



int main(int argc, char *argv[]) {
	
	int pLinhaMat1, pColunaMat1, pLinhaMat2, pColunaMat2, rank, nTask;
	int info[4];
	double *pMat1, *pMat2, *pMatResul, *pMatFinal, *pMatLocal;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nTask);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    omp_set_num_threads(atoi(argv[4]));
    
	
    
    if(rank == 0) {
		pMat1 = lerMatriz(argv[1], &pLinhaMat1, &pColunaMat1);
		pMat2 = lerMatriz(argv[2], &pLinhaMat2, &pColunaMat2); 
		
		
		info[0] = pLinhaMat1/nTask;
		info[1] = pColunaMat1;
		info[2] = pLinhaMat2;
		info[3] = pColunaMat2; 
		
		imprimirMatriz(pMat1, pLinhaMat1, pColunaMat1);
		imprimirMatriz(pMat2, pLinhaMat2, pColunaMat2);
		
		pMatFinal = (double*) malloc(pLinhaMat1*pColunaMat2*sizeof(double));
		pMatResul = (double*) malloc(info[0]*info[3]*sizeof(double));   
		pMatLocal = (double*) malloc(info[0]*info[1]*sizeof(double));
		
        MPI_Bcast(info, 4, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(pMat2, info[2]*info[3], MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Scatter(pMat1, info[0]*info[1], MPI_DOUBLE, pMatLocal, info[0]*info[1], MPI_DOUBLE, 0, MPI_COMM_WORLD);
      
		multiplicaMatrizes(pMatResul, pMatLocal, pMat2, info[0], info[2],  info[1], info[3]);
        
        MPI_Gather(pMatResul, info[0]*info[3], MPI_DOUBLE, pMatFinal,info[0]*info[3], MPI_DOUBLE, 0, MPI_COMM_WORLD);
        
		escreverMatriz(argv[3] , pMatFinal, pLinhaMat1, pColunaMat2);
        
        liberarMatriz(pMat1);
        liberarMatriz(pMat2);
        liberarMatriz(pMatResul);
        liberarMatriz(pMatLocal);
        liberarMatriz(pMatFinal);
      
	}else{
        
		MPI_Bcast(info, 4, MPI_INT, 0, MPI_COMM_WORLD);
		//pMat1 = (float*) malloc(info[0]*info[1]*sizeof(float));  
		pMat2 = (double*) malloc(info[2]*info[3]*sizeof(double));
		pMatLocal = (double*) malloc(info[0]*info[1]*sizeof(double));  
 
		MPI_Bcast(pMat2, info[2]*info[3], MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Scatter(pMat1, info[0]*info[1], MPI_DOUBLE, pMatLocal, info[0]*info[1], MPI_DOUBLE, 0, MPI_COMM_WORLD);
		
		
		imprimirMatriz(pMatLocal, info[0], info[1]);
		imprimirMatriz(pMat2, info[2], info[3]);
		
		
		pMatResul = (double*) malloc(info[0]*info[3]*sizeof(double));
		
		multiplicaMatrizes(pMatResul, pMatLocal, pMat2, info[0], info[2],  info[1], info[3]);
       
        MPI_Gather(pMatResul, info[0]*info[3], MPI_DOUBLE, pMatFinal, info[0]*info[3], MPI_DOUBLE, 0, MPI_COMM_WORLD);

        liberarMatriz(pMat2);
        liberarMatriz(pMatResul);
        liberarMatriz(pMatLocal);
		 
	}
    
    MPI_Finalize();
	
}
