#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float *lerMatriz(FILE *arqMatriz, int *linha, int *coluna);
void imprimirMatriz(float *m, int linha, int coluna);
void escreverMatriz(char *nomeDoArquivo, float *m, int linha, int coluna);
void liberarMatriz(float *m);
