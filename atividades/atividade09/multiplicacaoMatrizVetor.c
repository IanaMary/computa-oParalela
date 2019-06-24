#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operacoesSobreMatrizes.h"
#include <omp.h>

void multiplicaMatrizes(float *pMatResul, float *pMat1, float *pMat2, int pLinhaMat1, int pLinhaMat2, int pColunaMat1, int pColunaMat2){	    
	
	int i, j, k;
	#pragma omp parallel for shared(pMat1, pMat2, pLinhaMat1, pColunaMat2, pColunaMat1, pMatResul) private(i, j, k) schedule(static)
	for(i = 0; i < pLinhaMat1; i++){
		for(j = 0; j < pColunaMat2; j++){
			pMatResul[i*pColunaMat2 + j] = 0;
			for(k = 0; k < pColunaMat1; k++){
				pMatResul[i*pColunaMat2 + j] = pMatResul[i*pColunaMat2 + j] + pMat1[i*pColunaMat1 + k] * pMat2[k*pColunaMat2 + j];
			}
		}
	}
}



int main(int argc, char *argv[]) {
	
	int pLinhaMat1, pColunaMat1, pLinhaMat2, pColunaMat2 = 0;
	float *pMat1, *pMat2;    

    
    pMat1 = lerMatriz(argv[1], &pLinhaMat1, &pColunaMat1);
    
    pMat2 = lerMatriz(argv[2], &pLinhaMat2, &pColunaMat2);  
    
    
    imprimirMatriz(pMat1, pLinhaMat1, pColunaMat1);
    imprimirMatriz(pMat2, pLinhaMat2, pColunaMat2);
    
    float *pMatRes = (float*) malloc(pLinhaMat1*pColunaMat2*sizeof(float));
    
    multiplicaMatrizes(pMatRes, pMat1, pMat2, pLinhaMat1, pLinhaMat2, pColunaMat1, pColunaMat2);	    
	
	imprimirMatriz(pMatRes, pLinhaMat1, pColunaMat2);
    
    escreverMatriz(argv[3], pMatRes, pLinhaMat1, pColunaMat2);
    
    liberarMatriz(pMat1);
    liberarMatriz(pMat2);
    liberarMatriz(pMatRes);
    
	return 0;
	
}

