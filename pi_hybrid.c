#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>


int main(int argc,char* argv[]) { 
    int N = atoi(argv[1]); //Total number of points
    
    int rank, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double sum = 0; //sum of numbers of points in circle
    int i = 0;
   
    
    int task = N/size;
    int r = N % size;
    
    int idx = rank*task ;
    int nextidx = task*(rank+1);
    if(rank == size-1)
    {
        nextidx =  N;
    }
	//#pragma omp parallel for private(i) reduction(+:sum) 
    for(i = idx ; i < nextidx; i++) { 
		double x = (double)i / N;
        sum = sum + sqrt(1 - x * x);
    } 
    
    if (rank != 0) 
    {   
        //MPI_Send(buffer,count,type,dest,tag,comm)
        MPI_Send(&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD); 
    } 
    else if (rank == 0) 
    {   
        for(i = 0; i < size-1; i++)
        {
            double tem;
            //MPI_Recv(buffer,count,type,source,tag,comm,status) 
            MPI_Recv(&tem, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
            sum=sum+tem;
        }
        
    }
    
    if(rank==0)
    {
        
		printf("%f\n", (double) 4 * sum / (double)N ); 
    }
     
    MPI_Finalize();
    
    //return 0; 
} 
