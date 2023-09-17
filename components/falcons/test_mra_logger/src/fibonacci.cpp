#include "fibonacci.hpp"
// TODO #include "logging.hpp"



int fibonacci(int n)
{
    // TODO MRA_TRACE_FUNCTION();
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fibonacci(n-1) + fibonacci(n-2);
}

