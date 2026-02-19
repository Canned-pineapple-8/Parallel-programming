#include <iostream>
#include <omp.h>
#include <format>
#include <string>

int read_value(int&);


void matrix_free(int** matrix, int n)
{
    for (int i = 0; i < n; ++i)
        delete[] matrix[i];

    delete[] matrix;
}


int** matrix_init(int n)
{
    int** matrix = new int* [n];

    for (int i = 0; i < n; ++i)
        matrix[i] = new int[n];

    return matrix;
}

int read_matrix(int **a, int n)
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

void print_matrix(int** a, int n)
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

int read_value(int& val)
{
    if (!(std::cin >> val))
    {
        std::cout << "Invalid input.";
        return 0;
    }
    return 1;
}

int read_var(int& var, std::string var_name)
{
    std::cout << std::format("Enter {}: ", var_name);
    if (!read_value(var))
    {
        return 0;
    }

    if (var < 1 || var > 10)
    {
        std::cout << std::format("Value {} must be between 1 and 10.\n", var_name);
        return 0;
    }
    return 1;
}

void multiply(int** a, int** b, int** c, int n, int threads_num)
{
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
}

int main()
{
    int n, k;

    if (!read_var(n, "n")) exit(-1);
    if (!read_var(k, "k")) exit(-1);


    int **a, **b, **c;

    a = matrix_init(n);
    b = matrix_init(n);
    c = matrix_init(n);

    if (!read_matrix(a, n) || !read_matrix(b, n)) exit(-1);

    multiply(a, b, c, n, k);

    print_matrix(c, n);

    matrix_free(a,n);
    matrix_free(b, n);
    matrix_free(c, n);

    return 0;
}
