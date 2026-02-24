#include <iostream>
#include <omp.h>
#include <format>
#include <string>
#include <vector>

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

void matrix_free(double** matrix, int n)
{
    for (int i = 0; i < n; ++i)
        delete[] matrix[i];

    delete[] matrix;
}

double** matrix_init(int n)
{
    double** matrix = new double* [n];

    for (int i = 0; i < n; ++i)
        matrix[i] = new double[n];

    return matrix;
}

int read_matrix(double**a, int n)
{
    std::cout << "Enter matrix:\n";
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (!read_value(a[i][j])) return 0;
        }
    }
    std::cout << std::endl;

    return 1;
}

void fill_matrix(double** a, int n, double filler_value)
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            a[i][j] = filler_value;
        }
    }
    std::cout << std::endl;
}

void print_matrix(double** a, int n)
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            std::cout << a[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void multiply(double** a, double** b, double** c, int n, int threads_num, double& exec_time)
{
    double start, end;
   
    start = omp_get_wtime();
#pragma omp parallel for num_threads(threads_num) schedule(static)
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            double sum = 0.0;

            for (int k = 0; k < n; ++k)
                sum += a[i][k] * b[k][j];

            c[i][j] = sum;
        }
    }
    end = omp_get_wtime();

    exec_time = end - start;
}

int main()
{
    std::vector<int> threads_num = { 1, 2, 4, 6, 8, 10, 12, 14, 16, 18, 36 };
    std::vector<int> iterations = { 10, 1000 };

    double exec_time;

    for (size_t i_iter = 0; i_iter < iterations.size(); ++i_iter)
    {
        double** a = matrix_init(iterations[i_iter]);
        double** b = matrix_init(iterations[i_iter]);
        double** c = matrix_init(iterations[i_iter]);

        fill_matrix(a, iterations[i_iter], 10);
        fill_matrix(b, iterations[i_iter], 10);

        for (size_t i_num = 0; i_num < threads_num.size(); ++i_num)
        {
            multiply(a, b, c, iterations[i_iter], threads_num[i_num], exec_time);
            std::cout << std::format("Threads num: {}, iterations: {}\n", threads_num[i_num], iterations[i_iter]);
            std::cout << std::format("\tExec time: {:.10f}\n\n", exec_time);
        }

        matrix_free(a, iterations[i_iter]);
        matrix_free(b, iterations[i_iter]);
        matrix_free(c, iterations[i_iter]);
    }


    return 0;
}
