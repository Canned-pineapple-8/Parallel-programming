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

double calc_pi(int precision, int thread_amt = 2)
{
    int N = precision;
    double pi = 0;

#pragma omp parallel num_threads(thread_amt)
    {
#pragma omp for
        for (int i = 0; i < N; ++i)
        {
            double x = (i + 0.5) / (double)N;
#pragma omp critical
            pi += 4 / ((double)N * (1 + x * x));
        }

    }

    return pi;
}

int main()
{
    double pi = 0;
    int precision, thread_amt;

    if (!read_var(thread_amt, "thread amount")) return 0;
    if (!read_var(precision, "precision", 1, 1e9)) return 0;

    pi = calc_pi(precision, thread_amt);

    std::cout << std::format("PI = {}\n", pi);

    return 0;
}
