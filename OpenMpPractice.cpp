#include <iostream>
#include <omp.h>
#include <format>
#include <windows.h>


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

void hidden_error(int thread_amt)
{
#pragma omp parallel num_threads(thread_amt)
    {
        int thread_number = omp_get_thread_num();
        Sleep(1000);
        std::cout << std::format("I am thread {} from {} threads!\n", thread_number, omp_get_num_threads());
    }
}

int main()
{
    int num_threads;

    if (!read_int_var(num_threads, "thread amount")) return(0);

    hidden_error(num_threads);

    return 0;
}
