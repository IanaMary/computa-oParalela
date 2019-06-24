#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

double f(double x) {
   double return_val = 0.0;
   return_val = cos(x * log(1.0/x));
   return return_val;
}

int main(int argc, char *argv[]) {
   // Valor da integral
   double integral; 
   // Limites do intervalo
   double a, b;
   // Número de trapézios
   int n;
   // Base do trapézio
   double h;
   double x;
   int i, inicio, fim;

   a = atof(argv[1]);
   b = atof(argv[2]);
   n = atoi(argv[3]);

   h = (b - a) / n;
   integral = (f(a) + f(b))/2.0;

   
   #pragma omp parallel shared(a,h,n) private(i,x,inicio,fim) reduction (+:integral)
   {
		inicio = omp_get_thread_num() * ((n)/omp_get_num_threads()) + 1;
		fim = ( omp_get_thread_num() + 1) *  ((n)/omp_get_num_threads());
		if(omp_get_thread_num() == (omp_get_num_threads()-1)){
			fim = n-1;
		}
		
		x = a + inicio * h;
		integral += f(x);
		for (i = inicio+1; i <= fim; i++) {
			x += h;
			integral += f(x);
		}
	}
   integral *= h;
   printf("%d trapézios, estimativa de %.2f a %.2f = %.5f\n", n, a, b, integral);

   return 0;
}
