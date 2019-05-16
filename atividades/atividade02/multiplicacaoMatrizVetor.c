#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void mulMatVet(int n, int m, float matriz[n][m], float vetor[m], char *fileResultName){
	
	float vetorResultante[n];
	
	for(int i = 0; i < n; i++) {
		vetorResultante[i] = 0;
	}
	
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < m; j++) {
			vetorResultante[i] = vetorResultante[i] + matriz[i][j] * vetor[j];
		}
	}
	
	FILE *arqResult;
	arqResult = fopen(fileResultName, "wt");
	
	fprintf(arqResult, "%d\n", n);
	for(int i = 0; i < n; i++) {
		if(i == (n - 1)) {
			fprintf(arqResult, "%f\n", vetorResultante[i]);
		} else {
			fprintf(arqResult, "%f:", vetorResultante[i]);
		}
	}
	
	fclose(arqResult);
}

int main(int argc, char *argv[]) {
	FILE *arqMatriz;
	FILE *arqVetor;
	
	char vetorAux[4000];
	char *linha;
	
	int n = 0;
	int m = 0;
	
	/* Começar a ler matriz */
	
	arqMatriz = fopen(argv[1], "rt");
	if(arqMatriz == NULL) {
		printf("erro na abertura do arquivo da matriz");
		return 0;
	}
	
	linha = fgets(vetorAux, 4000, arqMatriz);
	n = atoi(linha);
	//printf("%d\n", n);
	
	linha =	fgets(vetorAux, 4000, arqMatriz);
	m = atoi(linha);
	//printf("%d\n", m);
	
	float matriz[n][m];
	
	linha =	fgets(vetorAux, 4000, arqMatriz);
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < m; j++) {
			matriz[i][j] = atof(strtok(linha, ":"));
		}
		linha =	fgets(vetorAux, 4000, arqMatriz);
	}
	
	/*for(int i = 0; i < n; i++) {
		for(int j = 0; j < m; j++) {
			printf("%.2f ", matriz[i][j]);
		}
		printf("\n");
	}*/
	
	fclose(arqMatriz);
	
	/* Começar a ler vetor */
	
	arqVetor = fopen(argv[2], "rt");
	if(arqVetor == NULL) {
		printf("erro na abertura do arquivo do vetor");
		return 0;
	}
	
	linha = fgets(vetorAux, 4000, arqVetor);
	m = atoi(linha);
	//printf("%d\n", m);
	
	float vetor[m];
	
	linha =	fgets(vetorAux, 4000, arqVetor);
	//printf("%s", linha);
	for(int i = 0; i < m; i++) {
		vetor[i] = atof(strtok(linha, ":"));
	}
	
	/*for(int i = 0; i < m; i++) {
		printf("%.2f ", vetor[i]);
	}*/
	
	fclose(arqVetor);
	
	mulMatVet(n, m, matriz, vetor, argv[3]);
	
	return 0;
	
}

