#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float *lerMatriz(char arquivo[], int *linha, int *coluna);
void imprimirMatriz(float *m, int linha, int coluna);
void escreverMatriz(char *nomeDoArquivo, float *m, int linha, int coluna);
void liberarMatriz(float *m);
