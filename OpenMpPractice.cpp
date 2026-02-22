#include <iostream>
#include <omp.h>
#include <format>

#define NUM_THREADS 4
#define N 10

int calc_sum()
{
    int total_sum = 0;

#pragma omp parallel num_threads(NUM_THREADS) reduction(+:total_sum)
    {
        int thread_number = omp_get_thread_num();

#pragma omp for schedule(guided, 2)
        for (int i = 1; i < N + 1; ++i)
        {
            total_sum += i;
            std::cout << std::format("{}: calculation of the iteration number {}\n", thread_number, i);

        }

        std::cout << std::format("[{}]: Sum = {}\n", thread_number, total_sum);
    }

    std::cout << std::format("Sum = {}\n", total_sum);
    return total_sum;

}

int main()
{
    int total_sum = calc_sum();
    std::cout << std::format("Sum = {}\n", total_sum);
}
