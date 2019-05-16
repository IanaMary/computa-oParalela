#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double monteCarloPi(int n){
	
	double x = 0;
	double y = 0;
	double acertos = 0;
	for(int i = 0; i < n; i++) {
		x = (double)(rand())/(double)(RAND_MAX);
		y = (double)(rand())/(double)(RAND_MAX);

		if((x*x+y*y) < 1) {
			acertos++;
		}	
	}

	return (4.0*(acertos/n));
	
}

int main(){
	srand(time(NULL));
	double mcp = 0.0;
	int n = 0;
	printf("digite um número ");
	scanf("%i", &n);
	mcp = monteCarloPi(n);
	printf("pi = %.5lf", mcp);
	
	return 0;
	
}

