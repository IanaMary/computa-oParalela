#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	
	int pLinhaMat1, pColunaMat1, pLinhaMat2, pColunaMat2 = 0;
	float *pMat1, *pMat2;
	
	
    arqMatriz = fopen(argv[1], "r");
    arqVetor = fopen(argv[2], "r");

    
    if((arqMatriz == NULL) && (arqVetor == NULL)){
        printf("Problema no arquivo\n");
        return 0;
    }
    
    pMat1 = lerMatriz(arqMatriz, &pLinhaMat1, &pColunaMat1);
    
    pMat2 = lerMatriz(arqVetor, &pLinhaMat2, &pColunaMat2);  
    
    float *pMatRes = (float*) malloc(pLinhaMat1*pColunaMat2*sizeof(float));
    
    multiplicaMatrizes(pMatRes, pMat1, pMat2, pLinhaMat1, pLinhaMat2, pColunaMat1, pColunaMat2);	    
	
	imprimirMatriz(pMatRes, pLinhaMat1, pColunaMat2);
    
    escreverMatriz(argv[3], pMatRes, pLinhaMat1, pColunaMat2);
    
    liberarMatriz(pMat1);
    liberarMatriz(pMat2);
    liberarMatriz(pMatRes);
    
	return 0;
	
}

