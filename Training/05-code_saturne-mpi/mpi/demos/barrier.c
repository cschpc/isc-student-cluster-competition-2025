#include <mpi.h>
#include <cstdio>

int main(int argc, char** argv)
{
    int rank, ntasks;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Barrier can be used for ordered print
    // Makes the code serial DO NOT use in real setups
    for (int i=0; i < ntasks; i++) {
      if (i == rank) {
        printf("I am rank %d in the group of %d\n", rank, ntasks);
        fflush(stdout);
      }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Finalize();
}
