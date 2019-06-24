#include "operacoesSobreMatrizes.h"

double *lerMatriz(char arquivo[], int *linha, int *coluna){    
    FILE *arqMatriz;
    arqMatriz = fopen(arquivo, "r");
    
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

    double *matriz = (double*) malloc(nLinha*nColuna*sizeof(double));
    
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


void imprimirMatriz(double *m, int linha, int coluna){
    
    for(int i = 0; i < linha; i++){
        for (int j = 0; j < coluna; j++ ){
            printf("%.4f ", m[i*coluna + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void escreverMatriz(char *nomeDoArquivo, double *m, int linha, int coluna){
	FILE *arqResposta;
    arqResposta = fopen(nomeDoArquivo, "w");
    fprintf(arqResposta, "%d\n", linha);
    fprintf(arqResposta, "%d\n", coluna);
    int i, j;
    for(i = 0; i < linha; i++){
		for(j = 0; j < coluna-1; j++){
			fprintf(arqResposta, "%.4f:", m[i*coluna + j]);
		}
        fprintf(arqResposta, "%.4f\n", m[i*coluna + j]);
    }
    
    fclose(arqResposta);
}

void liberarMatriz(double *m){

	free(m);
}

