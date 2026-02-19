#include <iostream>
#include <omp.h>
#include <format>
#include <string>

int read_value(int& val)
{
    if (!(std::cin >> val))
    {
        std::cout << "Invalid input.";
        return 0;
    }
    return 1;
}

int read_var(int& var, std::string var_name, int lower_bound = 1, int upper_bound = 10)
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

void sections(int threads_num)
{
#pragma omp parallel num_threads(threads_num) 
    {
#pragma omp sections
        {
#pragma omp section
            {
                std::cout << std::format("[{}]: came in section {}\n", omp_get_thread_num(), 1);
            }

#pragma omp section
            {
                std::cout << std::format("[{}]: came in section {}\n", omp_get_thread_num(), 2);
            }

#pragma omp section
            {
                std::cout << std::format("[{}]: came in section {}\n", omp_get_thread_num(), 3);
            }
        }
        std::cout << std::format("[{}]: parallel region\n", omp_get_thread_num());
    }

}

int main()
{
    int k;
    if (!read_var(k, "k")) exit(-1);

    sections(k);

    return 0;
}
