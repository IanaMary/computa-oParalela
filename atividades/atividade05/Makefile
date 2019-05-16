MPICC = mpicc

all: app

app: main op
	$(MPICC) multiplicacaoMatrizVetor.o operacoesSobreMatrizes.o -o app
	
main:
	$(MPICC) multiplicacaoMatrizVetor.c -c
	
op:
	$(MPICC) operacoesSobreMatrizes.c -c
	
clean:
	rm *.o app
	
	
