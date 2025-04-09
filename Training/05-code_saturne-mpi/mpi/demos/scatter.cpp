#include <mpi.h>
#include <cstdio>
#include <vector>
#include <iostream>

int main(int argc, char** argv)
{
    int rank, ntasks;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int local_size = 4;
    int global_size = local_size * ntasks;

    std::vector<float> global_data;
    std::vector<float> local_data(local_size, -1);

    if (0 == rank) printf("Before scatter\n");

    // Ugly hack for ordered print
    for (int i=0; i < ntasks; i++) {
        if (i == rank) {
            printf("%d : ", rank);
            for (float d: local_data)
               printf("%f ", d);
            printf("\n");
            fflush(stdout);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    if ( 0 == rank ) {
        // generate data, real application might read this from file
        for (int i=0; i < global_size; i++) {
            global_data.push_back(i);
        }
    }

    MPI_Scatter(global_data.data(), local_size, MPI_FLOAT, 
                local_data.data(),local_size, MPI_FLOAT,
                0, MPI_COMM_WORLD);

    if (0 == rank) printf("After scatter\n");

    // Ugly hack for ordered print
    for (int i=0; i < ntasks; i++) {
        if (i == rank) {
            printf("%d : ", rank);
            for (float d: local_data)
               printf("%f ", d);
            printf("\n");
            fflush(stdout);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Finalize();
}
