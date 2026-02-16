#include <iostream>
#include <omp.h>
#include <format>


int main()
{
    int total_sum = 0, N, thread_amt;

    std::cout << "Enter k: ";
    std::cin >> thread_amt;

    std::cout << "Enter N: ";
    std::cin >> N;


    if (N < 0 || thread_amt < 1)
    {
        std::cout << "N must be non-negative, k must be at least 1.\n";
        exit(-1);
    }

#pragma omp parallel num_threads(thread_amt) reduction(+:total_sum)
    {
        int thread_number = omp_get_thread_num();

#pragma omp for 
        for (int i = 1; i < N + 1; ++i)
        {
            total_sum += i;
        }

        std::cout << std::format("[{}]: Sum = {}\n", thread_number, total_sum);
    }

    std::cout << std::format("Sum = {}\n", total_sum);

    return 0;
}
