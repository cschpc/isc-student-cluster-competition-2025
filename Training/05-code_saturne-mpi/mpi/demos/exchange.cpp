#include <mpi.h>
#include <cstdio>
#include <vector>

int main(int argc, char** argv)
{
    int rank, ntasks;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Find "left" and "right" neighbours, use module operation for wrapping
    // around for periodic boundary condition

    int left = (rank - 1 + ntasks) % ntasks;
    int right = (rank + 1) % ntasks;

    // Use short array of floats equal to rank as message
    std::vector<float> send_msg(20, rank);
    std::vector<float> recv_msg(20, -1);

    // Send to right, receive from left
    MPI_Sendrecv(send_msg.data(), 20, MPI_FLOAT, right, 1,
                 recv_msg.data(), 20, MPI_FLOAT, left, 1, 
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("Rank %d : received %f from rank %d\n", rank, recv_msg[0], left);

    MPI_Finalize();
}
