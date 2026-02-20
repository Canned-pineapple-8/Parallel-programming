#include <iostream>
#include <omp.h>

#define NUM_THREADS 4

int main()
{

#pragma omp parallel num_threads(NUM_THREADS)
    {
        std::cout << "Hello, world!\n";
    }

    return 0;
}
