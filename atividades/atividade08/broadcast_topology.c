#include <stdio.h>
#include "mpi.h"
#include <string.h>
#include <stdlib.h>

#define SIZE 16
#define UP    0
#define DOWN  1
#define LEFT  2
#define RIGHT 3

int printArrayForRank(int rank, int *array,  int arraySize) {
   int i;
   char *output = (char *) malloc((arraySize + 100) * 4 * sizeof(char));
   sprintf(output, "Rank %d: ", rank);
   for (int i = 0; i < arraySize; i++)
      sprintf(output, "%s %d", output, array[i]);
   sprintf(output,"%s\n", output);
   printf("%s", output);
   fflush(stdout);
   free(output);
   return i;
}

 int MPI_Ring_broadcast(int *array, int arraySize, MPI_Comm comm) {
	int rank, nTask;
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &nTask);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	int left = ((rank -1 + nTask) % nTask);
	int right = (rank +1 ) % nTask;
	
	
	if(rank == 0){
		MPI_Send(array, arraySize, MPI_INT, left, 0, MPI_COMM_WORLD);
		MPI_Send(array, arraySize, MPI_INT, right, 0, MPI_COMM_WORLD);        
	}
	else if(rank < 8){
		MPI_Recv(array, arraySize, MPI_INT, left, 0, MPI_COMM_WORLD, &status);		
		MPI_Send(array, arraySize, MPI_INT, right, 0, MPI_COMM_WORLD); 
	}
	else{
		MPI_Recv(array, arraySize, MPI_INT, right, 0, MPI_COMM_WORLD, &status);		
		MPI_Send(array, arraySize, MPI_INT, left, 0, MPI_COMM_WORLD); 	
	}
		

}


int MPI_Mesh_broadcast(int *array, int arraySize, MPI_Comm comm) {
    MPI_Comm cartcomm;
    MPI_Status status;
    
    // Dimensões
    int dims[2]={4,4}, vizinhos[4], rank;
    int periods[2]={0,0}, reorder=0, coords[2];
   
    MPI_Cart_create(comm, 2, dims, periods, reorder, &cartcomm);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Cart_coords(cartcomm, rank, 2, coords);
    
    MPI_Cart_shift(cartcomm, 0, 1, &vizinhos[UP], &vizinhos[DOWN]);    
    MPI_Cart_shift(cartcomm, 1, 1, &vizinhos[LEFT], &vizinhos[RIGHT]);    
    
    if(coords[1] == 0){	
		//printf("rank: %d\n", rank);}
		if(rank != 0){
			//printf("Estou recebendo do de cima, rank %d \n", rank);
			MPI_Recv(array, arraySize, MPI_INT, vizinhos[UP], 0, cartcomm, &status);
		}
				
        MPI_Send(array, arraySize, MPI_INT, vizinhos[RIGHT], 0, cartcomm);
        MPI_Send(array, arraySize, MPI_INT, vizinhos[DOWN], 0, cartcomm);
	}
    
    else{
		//printf("Estou recebendo do da esquerda, rank %d \n", rank);
		MPI_Recv(array, arraySize, MPI_INT, vizinhos[LEFT], 0, cartcomm, &status);
		//printf("Estou enviando do da direita, rank %d \n", rank);
		MPI_Send(array, arraySize, MPI_INT, vizinhos[RIGHT], 0, cartcomm);
	}
}


int main(int argc, char *argv[]) {
   int size, rank;
   int *array, arraySize;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   
   arraySize = atoi(argv[1]);
   array = (int *) malloc(arraySize * sizeof(int));
 
   // Inicializa o array com 0.
   if (rank == 0) {
     for (int i = 0; i < arraySize; i++)
         array[i] = 0;
   }

    // Imprime o array inicial de todos os processos.
    printArrayForRank(rank, array, arraySize);

    // Faz broadcast usando uma topologia anel.
    MPI_Ring_broadcast(array, arraySize, MPI_COMM_WORLD);

    // Barreira de sincronização.
    MPI_Barrier(MPI_COMM_WORLD);

    // Imprime o array após o broadcast na topologia anel.
    printArrayForRank(rank, array, arraySize);
  
    // Barreira de sincronização.
    MPI_Barrier(MPI_COMM_WORLD);
   
    // Reconfigura o array com todos os elementos iguais a 1000.
    if (rank == 0) {
       for (int i = 0; i < arraySize; i++)
          array[i] =  1000;
    }

   // Faz broadcast usando uma topologia mesh.
   MPI_Mesh_broadcast(array, arraySize, MPI_COMM_WORLD);

   // Barreira de sincronização.
   MPI_Barrier(MPI_COMM_WORLD);

   // Imprime o array após o broadcast na topologia mesh.
   printArrayForRank(rank, array, arraySize);
     
   free(array);
   MPI_Finalize();
   return 0;
}
