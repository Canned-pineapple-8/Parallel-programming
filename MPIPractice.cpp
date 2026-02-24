#include <mpi.h>
#include <iostream>
#include <format>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::cout << std::format("I am {} process from {} processes!\n", rank, size);

    MPI_Finalize();
    return 0;
}