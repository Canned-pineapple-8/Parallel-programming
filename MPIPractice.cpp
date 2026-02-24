#include <mpi.h>
#include <iostream>
#include <format>

void first_second(int rank)
{
    if (rank == 0)
    {
        int size;
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        std::cout << std::format("{} processes\n", size);
    }
    else if (rank % 2 == 0)
    {
        std::cout << std::format("I am process {}: SECOND\n", rank);
    }
    else
    {
        std::cout << std::format("I am process {}: FIRST\n", rank);
    }
    return;
}

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    first_second(rank);

    MPI_Finalize();
    return 0;
}