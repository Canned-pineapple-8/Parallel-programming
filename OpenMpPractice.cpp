#include <iostream>
#include <omp.h>
#include <format>
#include <string>
#include <Windows.h>

template<typename T>
int read_value(T& val)
{
    if (!(std::cin >> val))
    {
        std::cout << "Invalid input.\n";
        std::cin.clear();
        return 0;
    }
    return 1;
}

template<typename T>
int read_var(T& var, std::string name, T lower_bound = (T)0, T upper_bound = T(1e9))
{
    std::cout << std::format("Enter {}: ", name);

    if (!read_value(var))
        return false;

    if (var < lower_bound || var > upper_bound)
    {
        std::cout << std::format(
            "Value {} must be between {} and {}.\n",
            name, lower_bound, upper_bound);
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
                Sleep(1000);
                std::cout << std::format("[{}]: came in section {}\n", omp_get_thread_num(), 1);
            }

#pragma omp section
            {
                Sleep(1000);
                std::cout << std::format("[{}]: came in section {}\n", omp_get_thread_num(), 2);
            }

#pragma omp section
            {
                Sleep(1000);
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
