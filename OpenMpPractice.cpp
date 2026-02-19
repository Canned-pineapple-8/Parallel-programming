#include <iostream>
#include <omp.h>
#include <format>

int read_value(int& val)
{
    if (!(std::cin >> val))
    {
        std::cout << "Invalid input.";
        return 0;
    }
    return 1;
}

int read_var(int& var, std::string var_name, int lower_bound = 1, int upper_bound = 1000)
{
    std::cout << std::format("Enter {}: ", var_name);
    if (!read_value(var))
    {
        return 0;
    }

    if (var < lower_bound || var > upper_bound)
    {
        std::cout << std::format("Value {} must be between {} and {}.\n", var_name, lower_bound, upper_bound);
        return 0;
    }
    return 1;
}

int calc_sum(int N, int threads_num = 2)
{
    int total_sum = 0;

#pragma omp parallel for num_threads(threads_num)
    for (int i = 1; i <= N; ++i)
    {
#pragma omp atomic
        total_sum += i;
    }

    return total_sum;
}

int main()
{
    int N, thread_amt;

    if (!read_var(thread_amt, "threads amount")) return 0;
    if (!read_var(N, "N")) return 0;


    int total_sum = calc_sum(N, thread_amt);

    std::cout << std::format("Sum = {}\n", total_sum);

    return 0;
}
