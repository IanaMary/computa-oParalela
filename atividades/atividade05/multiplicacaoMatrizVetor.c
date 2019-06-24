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
	
	int pLinhaMat1, pColunaMat1, pLinhaMat2, pColunaMat2, rank, nTask = 0;
	int info[4];
	float *pMat1, *pMat2, *pMatRes;
	
	
	 MPI_Status status;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nTask);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	
    
    if(rank == 0) {
		pMat1 = lerMatriz(argv[1], &pLinhaMat1, &pColunaMat1);
		pMat2 = lerMatriz(argv[2], &pLinhaMat2, &pColunaMat2); 
		
		pMatRes = (float*) malloc(pLinhaMat1*pColunaMat2*sizeof(float));   
		
		info[0] = pLinhaMat1/nTask;
		info[1] = pColunaMat1;
		info[2] = pLinhaMat2;
		info[3] = pColunaMat2; 
		
	
		for(int i=1; i < nTask; i++){
            MPI_Send(info, 4, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(pMat1 + i * info[0]*info[1], info[0]*info[1], MPI_FLOAT, i, 1, MPI_COMM_WORLD);
            MPI_Send(pMat2, info[2]*info[3], MPI_FLOAT, i, 2, MPI_COMM_WORLD);
        }
        	
        	
        
        multiplicaMatrizes(pMatRes, pMat1, pMat2, info[0], info[1],  info[2], info[3]);
        
         for(int i = 1; i < nTask; i++){
            MPI_Recv(pMatRes + i*(info[0]*info[3]), info[0]*info[3], MPI_FLOAT, i, 0, MPI_COMM_WORLD, &status);
        }
        
		escreverMatriz(argv[3] ,pMatRes, pLinhaMat1, pColunaMat2);
		imprimirMatriz(pMatRes, pLinhaMat1, pColunaMat2);
        
        liberarMatriz(pMat1);
        liberarMatriz(pMat2);
        liberarMatriz(pMatRes);
      
	}
        
    else {
        
        MPI_Recv(info, 4, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        pMat1 = (float*) malloc(info[0]*info[1]*sizeof(float));         
		pMat2 = (float*) malloc(info[2]*info[3]*sizeof(float));   
 
 
        MPI_Recv(pMat1, info[0]*info[1], MPI_FLOAT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(pMat2, info[2]*info[3], MPI_FLOAT, 0, 2, MPI_COMM_WORLD, &status);

		
		pMatRes = (float*) malloc(info[0]*info[3]*sizeof(float));   
		
		multiplicaMatrizes(pMatRes, pMat1, pMat2, info[0], info[1],  info[2], info[3]);
       
        MPI_Send(pMatRes, info[0]*info[3], MPI_FLOAT, 0, 0, MPI_COMM_WORLD);

        liberarMatriz(pMat1);
        liberarMatriz(pMat2);
        liberarMatriz(pMatRes);
		 
	}
	
    
     MPI_Finalize();
	
	return 0;
	
}

