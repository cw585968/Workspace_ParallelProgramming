#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mpi.h"

/*
 * mpicc -std=c99 recvandwait.c -o recvandwait
 * mpiexec -np 2 recvandwait
 */
 
// start with 2 procs

int main(int argc, char** argv) 
{
	int procRank,procCount;

	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD,&procCount);
	MPI_Comm_rank(MPI_COMM_WORLD,&procRank);
		
  printf("Start[%d]/[%d] \n",procRank,procCount);

  int message = -1;
  enum { tagSend = 1 };
   
  printf("Message undef.: %i\n",message);

  if (0==procRank)
  {
    message=42;
    sleep(5);
    MPI_Send(&message, 1, MPI_INT, 1, tagSend, MPI_COMM_WORLD);
	  printf("Sent Message \n");
  }
  else
  {
    MPI_Request req;
    MPI_Irecv(&message, 1, MPI_INT, 0, tagSend, MPI_COMM_WORLD, 
        &req); // no status

    int flag=0;
    while (1) {
      MPI_Test(&req,&flag,MPI_STATUS_IGNORE);
      if (1==flag)
        break;
      printf("wait ...\n");
      sleep(1);
    }
	  printf("Recv. Message: %i\n",message);
  }

  MPI_Finalize();

  return EXIT_SUCCESS;
}
