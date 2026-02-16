#include <iostream>
#include <omp.h>
#include <format>


int main()
{
    int num_threads;

    std::cout << "Enter threads number: ";
    std::cin >> num_threads;

    if (num_threads < 0)
    {
        std::cout << "\nThreads number must be non-negative.";
        exit(-1);
    }

#pragma omp parallel num_threads(num_threads)
    {
        int thread_number = omp_get_thread_num();
        std::cout << std::format("I am thread {} from {} threads!\n", omp_get_thread_num(), omp_get_num_threads());
    }

    return 0;
}
