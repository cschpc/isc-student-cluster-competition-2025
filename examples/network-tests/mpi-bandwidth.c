#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int ntasks, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
        if (2 != ntasks) {
            printf("Needs to be run with exactly two tasks\n");
            return -1;
        }
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        size_t buf_size = 2*1024*1024; // 2 MB
        char *send_buf = (char *) malloc(buf_size);                                       
        char *recv_buf = (char *) malloc(buf_size);

        int dst = (rank + 1) % 2;        
        int src = (rank + 1) % 2;        
        
        // Warm-up
        MPI_Sendrecv(send_buf, buf_size, MPI_CHAR, dst, 1,
                     recv_buf, buf_size, MPI_CHAR, src, 1,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // Measure with five repeats
        double t0, t1;
        t0 = MPI_Wtime();
        for (int r=0; r < 5; r++) {
          MPI_Sendrecv(send_buf, buf_size, MPI_CHAR, dst, 1,
                       recv_buf, buf_size, MPI_CHAR, src, 1,
                       MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        t1 = MPI_Wtime();
        double bw = 5*2 / (t1 - t0);
        printf("Rank: %d  %f (MB/s) \n", rank, bw); 
	
	MPI_Finalize();
}
