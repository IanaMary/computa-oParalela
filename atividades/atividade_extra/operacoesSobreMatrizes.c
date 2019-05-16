#include "operacoesSobreMatrizes.h"

float *lerMatriz(FILE *arqMatriz, int *linha, int *coluna){    
    
    char *pch;
    size_t tamBuffer = 100;
    char *buffer_linha = (char*) malloc(tamBuffer * sizeof(char));
    int nLinha, nColuna;

    //Lendo o núemro de linhas
    getline(&buffer_linha, &tamBuffer, arqMatriz);
    nLinha = atoi(buffer_linha);
	*linha = nLinha;

    //Lendo o número de colunas
    getline(&buffer_linha, &tamBuffer, arqMatriz);
    nColuna = atoi(buffer_linha);
	*coluna = nColuna;

    float *matriz = (float*) malloc(nLinha*nColuna*sizeof(float));
    
    //Preenchendo a matriz   
    for(int i = 0; i < nLinha; i++){
        if(getline(&buffer_linha, &tamBuffer, arqMatriz) != -1){
            pch = strtok(buffer_linha, ":");
            for(int j = 0; j < nColuna; j++){
                if(pch != NULL){
                    matriz[i*nColuna + j] = atof(pch);
                    pch = strtok(NULL, ":");
                }                                
            }
        }
    }

    free(buffer_linha);
    fclose(arqMatriz);
	return matriz;
}

void imprimirMatriz(float *m, int linha, int coluna){
    
    for(int i = 0; i < linha; i++){
        for (int j = 0; j < coluna; j++ ){
            printf("%.2f ", m[i*coluna + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void escreverMatriz(char *nomeDoArquivo, float *m, int linha, int coluna){
	FILE *arqResposta;
    arqResposta = fopen(nomeDoArquivo, "w");
    fprintf(arqResposta, "%d\n", linha);
    fprintf(arqResposta, "%d\n", coluna);
    int i, j;
    for(i = 0; i < linha; i++){
		for(j = 0; j < coluna-1; j++){
			fprintf(arqResposta, "%.2f:", m[i*coluna + j]);
		}
        fprintf(arqResposta, "%.2f\n", m[i*coluna + j]);
    }
    
    fclose(arqResposta);
}

void liberarMatriz(float *m){

	free(m);
}

