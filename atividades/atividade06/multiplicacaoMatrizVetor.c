#include "mpi.h"
#include "operacoesSobreMatrizes.h"

void multiplicaMatrizes(float *pMatResul, float *pMat1, float *pMat2, int pLinhaMat1, int pLinhaMat2, int pColunaMat1, int pColunaMat2){	    
	
	for(int i = 0; i < pLinhaMat1; i++){
		for(int j = 0; j < pColunaMat2; j++){
			pMatResul[i*pColunaMat2 + j] = 0;
			for(int k = 0; k < pColunaMat1; k++){
				pMatResul[i*pColunaMat2 + j] = pMatResul[i*pColunaMat2 + j] + pMat1[i*pColunaMat1 + k] * pMat2[k*pColunaMat2 + j];
			}
		}
	}	
}



int main(int argc, char *argv[]) {
	FILE *arqMatriz;
	FILE *arqVetor;
	
	int pLinhaMat1, pColunaMat1, pLinhaMat2, pColunaMat2, rank, nTask = 0;
	int info[4];
	float *pMat1, *pMat2, *pMatRes, *pMatFinal;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nTask);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	
    arqMatriz = fopen(argv[1], "r");
    arqVetor = fopen(argv[2], "r");

    if((arqMatriz == NULL) && (arqVetor == NULL)){
        printf("Problema no arquivo\n");
        return 0;
    }
    
   
    
    
    if(rank == 0) {
		pMat1 = lerMatriz(arqMatriz, &pLinhaMat1, &pColunaMat1);
		pMat2 = lerMatriz(arqVetor, &pLinhaMat2, &pColunaMat2); 
		
		pMatFinal = (float*) malloc(pLinhaMat1*pColunaMat2*sizeof(float));
		pMatRes = (float*) malloc(info[0]*info[3]*sizeof(float));   
		
		info[0] = pLinhaMat1/nTask;
		info[1] = pColunaMat1;
		info[2] = pLinhaMat2;
		info[3] = pColunaMat2; 
		
        MPI_Bcast(info, 4, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(pMat1 + rank * info[0]*info[1], info[0]*info[1], MPI_FLOAT, 0, MPI_COMM_WORLD);
        MPI_Bcast(pMat2, info[2]*info[3], MPI_FLOAT, 0, MPI_COMM_WORLD);

        multiplicaMatrizes(pMatRes, pMat1, pMat2, info[0], info[1],  info[2], info[3]);
        
        MPI_Gather(pMatRes, info[0]*info[3], MPI_FLOAT, pMatFinal, info[0]*info[3], MPI_FLOAT, 0, MPI_COMM_WORLD);
        
		escreverMatriz(argv[3] , pMatFinal, pLinhaMat1, pColunaMat2);
        
        liberarMatriz(pMat1);
        liberarMatriz(pMat2);
        liberarMatriz(pMatRes);
        liberarMatriz(pMatFinal);
      
	}
        
    else {
        
		MPI_Bcast(info, 4, MPI_INT, 0, MPI_COMM_WORLD);
        pMat1 = (float*) malloc(info[0]*info[1]*sizeof(float));         
		pMat2 = (float*) malloc(info[2]*info[3]*sizeof(float));   
 
 
        MPI_Bcast(pMat1, info[0]*info[1], MPI_FLOAT, 0, MPI_COMM_WORLD);
        MPI_Bcast(pMat2, info[2]*info[3], MPI_FLOAT, 0, MPI_COMM_WORLD);

		pMatRes = (float*) malloc(info[0]*info[3]*sizeof(float));
		
		multiplicaMatrizes(pMatRes, pMat1, pMat2, info[0], info[1],  info[2], info[3]);
       
        MPI_Gather(pMatRes, info[0]*info[3], MPI_FLOAT, pMatFinal, info[0]*info[3], MPI_FLOAT, 0, MPI_COMM_WORLD);

        liberarMatriz(pMat1);
        liberarMatriz(pMat2);
        liberarMatriz(pMatRes);
        liberarMatriz(pMatFinal);
		 
	}
    
     MPI_Finalize();
	
	return 0;
	
}

