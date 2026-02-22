#include <iostream>
#include <omp.h>
#include <format>

int read_int_value(int& val)
{
    if (!(std::cin >> val))
    {
        std::cout << "Invalid input.";
        return 0;
    }
    return 1;
}

int read_int_var(int& var, std::string var_name, int lower_bound = 1, int upper_bound = 10e9)
{
    std::cout << std::format("Enter {}: ", var_name);
    if (!read_int_value(var))
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

int calc_sum(int N, int thread_amt)
{
    int total_sum = 0;

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

    return total_sum;
}


int main()
{
    int N, thread_amount;

    if (!read_int_var(thread_amount, "thread amount", 0)) return 0;
    if (!read_int_var(N, "N", 0)) return 0;

    int total_sum = calc_sum(N, thread_amount);
    std::cout << std::format("Sum = {}\n", total_sum);

    return 0;
}
