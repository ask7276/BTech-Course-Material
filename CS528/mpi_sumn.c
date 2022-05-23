#include <mpi.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
// size of array 
#define n 16
#define Master 0
int a[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}; 
  
// Temporary array for slave process 
int a2[16]; 
  
int main(int argc, char* argv[]) { 
  
    int rank, size, Chunk, ne_recv; 
    MPI_Status status; 
    MPI_Init(&argc, &argv); 
   
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
  
    // master process 
    if (rank == Master) { 
        int index, i; 
        Chunk = n / size;//Assume n is multiple of size
        // check if more than 1 processes are run 
        if (size > 1) { 
            // distributes the portion of array to calculate their partial sums 
            for (i = 1; i < size ; i++) { 
                index = i * Chunk; 
                MPI_Send(&Chunk, 1, MPI_INT, i, 0, MPI_COMM_WORLD); 
                MPI_Send(&a[index], Chunk, MPI_INT, i, 0, MPI_COMM_WORLD); 
            } 
        } 
        // master process add its own sub array 
        int sum = 0; 
        for (i = 0; i < Chunk; i++) sum += a[i]; 
        // collects partial sums from other processes 
        int tmp; 
        for (i = 1; i < size; i++) { 
            MPI_Recv(&tmp, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status); 
              sum += tmp; 
        } 
  
        // prints the final sum of array 
        printf("Sum of array is : %d\n", sum); 
    } 
    // slave processes 
    else { 
        MPI_Recv(&ne_recv,  1, MPI_INT, 0, 0,  MPI_COMM_WORLD,  &status); 
        // stores the received array segment in local array a2 
        MPI_Recv(&a2, ne_recv,  MPI_INT, 0, 0, MPI_COMM_WORLD, &status); 
  
        // calculates its partial sum 
        int partial_sum = 0; 
        for (int i = 0; i < ne_recv; i++) 
            partial_sum += a2[i]; 
        MPI_Send(&partial_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); 
    } 
    MPI_Finalize(); 
  
    return 0; 
} 

