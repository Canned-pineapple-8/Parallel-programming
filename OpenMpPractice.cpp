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

double calc_pi(int N, int thread_amt)
{
    double pi = 0;

#pragma omp parallel num_threads(thread_amt) reduction(+:pi)
    {
        int thread_number = omp_get_thread_num();

#pragma omp for schedule(guided)
        for (int i = 0; i < N; ++i)
        {
            double x = (i + 0.5) / (double)N;
            pi += 4 / (1 + x * x);
        }

    }

    return pi / N;
}

int main()
{
    int N, thread_amount;

    if (!read_int_var(thread_amount, "thread amount", 0)) return 0;
    if (!read_int_var(N, "N (precision)", 0)) return 0;

    double pi = calc_pi(N, thread_amount);

    std::cout << std::format("PI = {}\n", pi);

    return 0;
}
