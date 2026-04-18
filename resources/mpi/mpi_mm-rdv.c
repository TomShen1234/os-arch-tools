/******************************************************************************
* FILE: mpi_mm.c
* DESCRIPTION:  
*   MPI Matrix Multiply - C Version
*   In this code, the master task distributes a matrix multiply
*   operation to numtasks-1 worker tasks.
*   NOTE:  C and Fortran versions of this code differ because of the way
*   arrays are stored/passed.  C arrays are row-major order but Fortran
*   arrays are column-major order.
* AUTHOR: Blaise Barney. Adapted from Ros Leibensperger, Cornell Theory
*   Center. Converted to MPI: George L. Gusciora, MHPCC (1/95)
* LAST REVISED: 04/13/05
* 2021/07/05: rdv, upped size, added performance code
******************************************************************************/
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

// for flexible compilation, set this on the command line at compile time
// #define MPITESTPHASES 5

#define NRA 2048               /* number of rows in matrix A, orig 62 */
#define NCA 1024               /* number of columns in matrix A, orig 15 */
#define NCB 512                /* number of columns in matrix B, orig 7 */
#define MASTER 0               /* taskid of first task */
#define FROM_MASTER 1          /* setting a message type */
#define FROM_WORKER 2          /* setting a message type */

/* rdv made these global */
double	a[NRA][NCA],           /* matrix A to be multiplied */
	b[NCA][NCB],           /* matrix B to be multiplied */
	c[NRA][NCB];           /* result matrix C */

int main (int argc, char *argv[])
{
  int	numtasks,              /* number of tasks in partition */
	taskid,                /* a task identifier */
	numworkers,            /* number of worker tasks */
	source,                /* task id of message source */
	dest,                  /* task id of message destination */
	mtype,                 /* message type */
	rows,                  /* rows of matrix A sent to each worker */
	averow, extra, offset, /* used to determine rows sent to each worker */
	i, j, k, rc;           /* misc */
  MPI_Status status;
  double startwtime = 0.0, endwtime;

  ////// Phase 0, basic comm setup. O(k) total work for k tasks.
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
  MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
  if (numtasks < 2 ) {
    printf("Need at least two MPI tasks. Quitting...\n");
    MPI_Abort(MPI_COMM_WORLD, rc);
    exit(1);
  }
  numworkers = numtasks-1;


/**************************** master task ************************************/
   if (taskid == MASTER)
   {
     printf("\nSFC Computer Architecture, Spring 2021.\nMPI parallel multiply program demonstration, adapted from ancient LLNL code. Not efficient.\nNote that O() messages are based solely on matrix size and number of tasks, not including constant factors.\n");
      printf("mpi_mm has started with %d tasks.\n",numtasks);
      printf("Initializing arrays...\n");
      startwtime = MPI_Wtime(); 
#if MPITESTPHASES >= 1
      ////// Phase 1, coordinator creates A, B O(NRA*NCA + NCA*NCB) work
      printf("Phase 1: O(%d) work\n", NRA*NCA + NCA*NCB);
      for (i=0; i<NRA; i++)
         for (j=0; j<NCA; j++)
            a[i][j]= i+j;
      for (i=0; i<NCA; i++)
         for (j=0; j<NCB; j++)
            b[i][j]= i*j;
#endif

#if MPITESTPHASES >= 2
      /* Send A matrix data to the worker tasks */
      printf("Phase 2: O(%d) work\n", NRA*NCA);
      averow = NRA/numworkers;
      extra = NRA%numworkers;
      offset = 0;
      mtype = FROM_MASTER;
      for (dest=1; dest<=numworkers; dest++)
      {
         rows = (dest <= extra) ? averow+1 : averow;   	
         printf("Sending %d rows to task %d offset=%d\n",rows,dest,offset);
         MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
         MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
         MPI_Send(&a[offset][0], rows*NCA, MPI_DOUBLE, dest, mtype,
                   MPI_COMM_WORLD);
         offset = offset + rows;
      }
      MPI_Barrier(MPI_COMM_WORLD); /* Phase 2 barrier */
#endif
      
#if MPITESTPHASES >= 3
      /* send B to everyone (expensive operation!) 
	 O(k*NCA*NCB) total work; can the interconnect help?
	 In theory, a broadcast here could be more efficient. */
      printf("Phase 3: O(%d) work\n", numworkers*NCA*NCB);
      for (dest=1; dest<=numworkers; dest++)
      {
	printf("Sending B (size: %d) to task %d\n",NCA*NCB,dest);
         MPI_Send(&b, NCA*NCB, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
      }
      MPI_Barrier(MPI_COMM_WORLD); /* Phase 3 barrier */
      printf("Coordinator done sending B.\n");
#endif

#if MPITESTPHASES >= 4
      printf("Phase 4: O(%d) total work\n", NRA*NCA*NCB);
      MPI_Barrier(MPI_COMM_WORLD); /* Phase 4 barrier */
#endif

#if MPITESTPHASES >= 5
      printf("Phase 5: O(%d) total work\n", NRA*NCB);
      /* Receive results from worker tasks */
      mtype = FROM_WORKER;
      for (i=1; i<=numworkers; i++)
      {
         source = i;
         MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
         MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
         MPI_Recv(&c[offset][0], rows*NCB, MPI_DOUBLE, source, mtype, 
                  MPI_COMM_WORLD, &status);
         printf("Received results from task %d\n",source);
      }

#endif
      MPI_Barrier(MPI_COMM_WORLD); /* Phase 5 (final) barrier */
      endwtime = MPI_Wtime();
#if MPITESTPHASES >= 5
      
      /* Print results */
      FILE *fp = fopen("delme.txt","w");
      fprintf(fp,"******************************************************\n");
      fprintf(fp,"Result Matrix:\n");
      for (i=0; i<NRA; i++)
      {
	fprintf(fp,"\n"); 
         for (j=0; j<NCB; j++) 
	   fprintf(fp,"%6.2f   ", c[i][j]);
      }
      fprintf(fp,"\n******************************************************\n");
      fclose(fp);
      printf ("Done.\n");
#endif

      printf("wall clock time for %d phases = %f\n", MPITESTPHASES,
	     endwtime-startwtime);          

   }


/**************************** worker task ************************************/
   if (taskid > MASTER)
   {
#if MPITESTPHASES >= 2
      mtype = FROM_MASTER;
      MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&a, rows*NCA, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Barrier(MPI_COMM_WORLD); /* Phase 2 barrier */
#endif
#if MPITESTPHASES >= 3
      MPI_Recv(&b, NCA*NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Barrier(MPI_COMM_WORLD); /* Phase 3 barrier */
#endif

#if MPITESTPHASES >= 4
      for (k=0; k<NCB; k++)
         for (i=0; i<rows; i++)
         {
            c[i][k] = 0.0;
            for (j=0; j<NCA; j++)
               c[i][k] = c[i][k] + a[i][j] * b[j][k];
         }
      MPI_Barrier(MPI_COMM_WORLD); /* Phase 4 barrier */
#endif
#if MPITESTPHASES >= 5
      mtype = FROM_WORKER;
      MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
      MPI_Send(&c, rows*NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
#endif
      MPI_Barrier(MPI_COMM_WORLD); /* Phase 5 (final) barrier */
   }
   MPI_Finalize();
}
