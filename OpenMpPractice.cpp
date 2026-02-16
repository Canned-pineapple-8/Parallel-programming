#include <iostream>
#include <omp.h>
#include <format>


int main()
{
    double pi = 0;
    int N, thread_amt;

    std::cout << "Enter k: ";
    std::cin >> thread_amt;

    std::cout << "Enter N: ";
    std::cin >> N;

    if (N < 1 || thread_amt < 1)
    {
        std::cout << "N and k must be at least 1.\n";
        exit(-1);
    }


#pragma omp parallel num_threads(thread_amt) reduction(+:pi)
    {
        int thread_number = omp_get_thread_num();

#pragma omp for schedule(guided)
        for (int i = 0; i < N; ++i)
        {
            double x = (i + 0.5) / (double)N;
            pi += 4 / ((double)N * (1 + x * x));
        }

    }

    std::cout << std::format("PI = {}\n", pi);

    return 0;
}
