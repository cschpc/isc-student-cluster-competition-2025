#include <vector>
#include <cstdio>
#include <mpi.h>

int main(int argc, char** argv)
{
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("Hello from rank %d of %d\n", rank, size);
    std::cout.flush();
    MPI_Barrier(MPI_COMM_WORLD);

    constexpr int num_elements = 1000;
    int padded_num_elements = num_elements + size - num_elements % size;
    int elements_per_rank = padded_num_elements / size;
    constexpr int root = 0;
    std::vector<int> data(padded_num_elements);
    std::vector<int> recv_buf(elements_per_rank);


    if ( root == rank )
    {
        for (int i=0; i < num_elements; i++)
        {
            data[i] = i;
        }
    }

    MPI_Scatter(data.data(), elements_per_rank, MPI_INTEGER, recv_buf.data(), elements_per_rank, MPI_INTEGER, root, MPI_COMM_WORLD);

    int sum = 0;
    for (int i=0; i < elements_per_rank; i++) {
        sum += recv_buf[i];
    }

    printf("part_sum = %d\n", sum);
    std::cout.flush();

    int reduced_sum;

    MPI_Reduce(&sum, &reduced_sum, size, MPI_INTEGER, MPI_SUM, root, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    if ( rank == root )
    {
        int sum_ref = num_elements * (num_elements - 1) / 2;
        std::cout.flush();
        printf("Sum is %d (ref %d)\n", reduced_sum, sum_ref);
    }

    MPI_Finalize();
}