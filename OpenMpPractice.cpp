#include <iostream>
#include <omp.h>
#include <format>

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
