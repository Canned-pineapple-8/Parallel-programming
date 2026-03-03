#include <mpi.h>
#include <iostream>
#include <format>
#include <cstring>

void send_message(const char * msg, int len, int recipient)
{
    MPI_Send(&len, 1, MPI_INT, recipient, 0, MPI_COMM_WORLD);
    MPI_Send(msg, len, MPI_CHAR, recipient, 1, MPI_COMM_WORLD);
}

char* receive_message(int sender)
{
    int len;
    MPI_Recv(&len, 1, MPI_INT, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    char* buffer = new char[len];
    MPI_Recv(buffer, len, MPI_CHAR, sender, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    return buffer;
}

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2)
    {
        std::cout << std::format("Not enough processes ({}).", size);
        MPI_Finalize();
        return 0;
    }

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        const char* message = "Hello from process 0!";
        int len = std::strlen(message);

        send_message(message, len + 1, 1);
    }
    else if (rank == 1)
    {
        char* message = receive_message(0);
        std::cout << message << std::endl;

        delete[] message;
    }

    MPI_Finalize();
    return 0;
}