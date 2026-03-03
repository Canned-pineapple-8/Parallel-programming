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


void send_message(const char * msg, int len, int recipient, MPI_Request* requests)
{
    MPI_Isend(&len, 1, MPI_INT, recipient, 0, MPI_COMM_WORLD, requests);
    MPI_Isend(msg, len, MPI_CHAR, recipient, 1, MPI_COMM_WORLD, requests + 1);

    MPI_Wait(requests, MPI_STATUS_IGNORE);
    MPI_Wait(requests + 1, MPI_STATUS_IGNORE);
}

char* receive_message(int sender, MPI_Request* requests)
{
    int len;
    MPI_Irecv(&len, 1, MPI_INT, sender, 0, MPI_COMM_WORLD, requests);

    char* buffer = new char[len];
    MPI_Irecv(buffer, len, MPI_CHAR, sender, 1, MPI_COMM_WORLD, requests + 1);

    MPI_Wait(requests, MPI_STATUS_IGNORE);
    MPI_Wait(requests + 1, MPI_STATUS_IGNORE);

    return buffer;
}

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (!check_processes_num(size)) return 0;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Request* reqs = new MPI_Request[2];

    if (rank == 0)
    {
        const char* message = "Hello from process 0!";
        int len = std::strlen(message);

        send_message(message, len + 1, 1, reqs);
    }
    else if (rank == 1)
    {
        char* message = receive_message(0, reqs);
        std::cout << message << std::endl;

        delete[] message;
    }

    MPI_Finalize();
    return 0;
}