#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double *lerMatriz(char arquivo[], int *linha, int *coluna);
void imprimirMatriz(double *m, int linha, int coluna);
void escreverMatriz(char *nomeDoArquivo, double *m, int linha, int coluna);
void liberarMatriz(double *m);
