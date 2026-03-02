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

void baton(int processes_num)
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        int msg = 0;
        send_message(msg, rank + 1);

        msg = receive_message(processes_num - 1);
        std::cout << std::format("[{}]: receive message '{}'", rank, msg++) << std::endl;
    }
    else
    {
        int msg = receive_message(rank - 1);
        std::cout << std::format("[{}]: receive message '{}'", rank, msg++) << std::endl;

        int recipient = rank == processes_num - 1 ? 0 : rank + 1;

        send_message(msg, recipient);
    }
}

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (!check_processes_num(size)) return 0;

    baton(size);

    MPI_Finalize();
    return 0;
}