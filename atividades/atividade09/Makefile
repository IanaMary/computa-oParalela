GCC = gcc -fopenmp

all: app

app: main op
	$(GCC) multiplicacaoMatrizVetor.o operacoesSobreMatrizes.o -o app
	
main:
	$(GCC) multiplicacaoMatrizVetor.c -c
	
op:
	$(GCC) operacoesSobreMatrizes.c -c
	
clean:
	rm *.o app
	
	
