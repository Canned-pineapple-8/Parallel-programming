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

void send_message(int* msg, int recipient, MPI_Request* request)
{
    MPI_Isend(msg, 1, MPI_INT, recipient, 0, MPI_COMM_WORLD, request);
}

void receive_message(int* msg, int sender, MPI_Request* request)
{
    MPI_Irecv(msg, 1, MPI_INT, sender, 0, MPI_COMM_WORLD, request);
}

void broadcast(int rank, int size)
{
    MPI_Request* reqs = new MPI_Request[(size - 1) * 2];
    int* received_messages = new int[size - 1];
    int msg_index = 0, req_index = 0;

    for (int rank_i = 0; rank_i < size; ++rank_i)
    {
        if (rank == rank_i) continue;

        send_message(&rank, rank_i, reqs + req_index++);
        receive_message(received_messages + msg_index++, rank_i, reqs + req_index++);
    }

    MPI_Waitall((size - 1) * 2, reqs, MPI_STATUSES_IGNORE);

    for (size_t i = 0; i < msg_index; ++i)
    {
        std::cout << std::format("[{}]: receive message '{}' from process {}\n", rank, received_messages[i], received_messages[i]);
    }

    delete[] reqs;
    delete[] received_messages;
}

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (!check_processes_num(size))
    {
        MPI_Finalize();
        return 0;
    }

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    broadcast(rank, size);

    MPI_Finalize();

    return 0;
}