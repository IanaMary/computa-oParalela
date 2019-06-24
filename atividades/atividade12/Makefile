MPI = mpicc
G = gcc

all: serial paralelo
	
serial:
	$(G) serial.c -o appSerial -lm

paralelo:
	$(MPI) paralelo.c -o appParalelo -lm

clean:
	rm appParalelo appSerial
