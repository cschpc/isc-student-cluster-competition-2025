#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	int ntasks, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("Hello %d %d\n", ntasks, rank);
	MPI_Finalize();
}
