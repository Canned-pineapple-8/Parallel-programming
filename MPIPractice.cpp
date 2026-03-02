#include <mpi.h>
#include <iostream>
#include <format>
#include <cstring>

int check_processes_num(int current_num, int min_num = 2)
{
    if (current_num < min_num)
    {
        std::cout << std::format("Not enough processes ({}).", current_num);
        return 0;
    }

    return 1;
}

void send_message(int msg, int recipient)
{
    MPI_Send(&msg, 1, MPI_INT, recipient, 0, MPI_COMM_WORLD);
}

int receive_message(int sender)
{
    int msg;
    MPI_Recv(&msg, 1, MPI_INT, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    return msg;
}

void master_slave(int processes_num)
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        for (int i = rank + 1; i < processes_num; ++i)
        {
            int msg = receive_message(i);
            std::cout << std::format("[{}]: receive message '{}'", rank, msg++) << std::endl;
        }
    }
    else
    {
        send_message(rank, 0);
    }
}

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (!check_processes_num(size)) return 0;

    master_slave(size);

    MPI_Finalize();
    return 0;
}