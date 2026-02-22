#include <iostream>
#include <omp.h>
#include <format>
#include <string>

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

void multiply(double** a, double** b, double** c, int n, int threads_num)
{
#pragma omp parallel for num_threads(threads_num) schedule(guided)
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


    double **a, ** b, ** c;

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
