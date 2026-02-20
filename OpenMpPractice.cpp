#include <iostream>
#include <omp.h>
#include <format>

#define ITERATIONS 20

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

double calc_pi_reduction(int N, double& exec_time, int thread_amt = 2)
{
    double pi = 0;
    double start = omp_get_wtime();

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
    double end = omp_get_wtime();
    exec_time = end - start;

    return pi;
}

double calc_pi_critical(int N, double& exec_time, int thread_amt = 2)
{
    double pi = 0;
    double start = omp_get_wtime();
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
    double end = omp_get_wtime();
    
    exec_time = end - start;
    return pi;
}

int main()
{
    double pi = 0;
    int precision, thread_amt;

    if (!read_var(thread_amt, "thread amount")) return 0;
    if (!read_var(precision, "precision", 1, 1e9)) return 0;

    double exec_t_reduction = 0, exec_t_critical = 0;
    double exec_t_local = 0;

    for (size_t i = 0; i < ITERATIONS; ++i)
    {
        std::cout << std::format("\tIteration #{}\n", i);
        
        pi = calc_pi_reduction(precision, exec_t_local, thread_amt);
        exec_t_reduction += exec_t_local;

        pi = calc_pi_critical(precision, exec_t_local, thread_amt);
        exec_t_critical += exec_t_local;
    }
    
    exec_t_reduction = exec_t_reduction / ITERATIONS;
    exec_t_critical = exec_t_critical / ITERATIONS;

    std::cout << std::format("PI = {}\n", pi);
    //std::cout << std::format("Execution time = {}\n", exec_time);

    std::cout << std::format("Exec time reduction = {}\n", exec_t_reduction);
    std::cout << std::format("Exec time critical = {}\n", exec_t_critical);

    return 0;
}
