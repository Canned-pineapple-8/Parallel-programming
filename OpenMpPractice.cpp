#include <iostream>
#include <omp.h>
#include <format>


int main()
{
    int total_sum = 0, N;

    std::cout << "Enter N: ";
    std::cin >> N;

    if (N < 0)
    {
        std::cout << "N must be non-negative.\n";
        exit(-1);
    }

#pragma omp parallel num_threads(2) reduction(+:total_sum)
    {
        int thread_number = omp_get_thread_num();
        int start, end;
        if (thread_number)
        {
            start = 0;
            end = N / 2;
        }
        else
        {
            start = N / 2 + 1;
            end = N;
        }

        for (int i = start; i < end + 1; ++i)
        {
            total_sum += i;
        }
        std::cout << std::format("[{}]: Sum = {}\n", thread_number, total_sum);
    }

    std::cout << std::format("Sum = {}\n", total_sum);

    return 0;
}
