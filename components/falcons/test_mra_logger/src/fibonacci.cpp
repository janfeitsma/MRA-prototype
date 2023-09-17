#include "fibonacci.hpp"
#include "logging.hpp"



int fibonacci(int n)
{
    MRA_TRACE_FUNCTION_INPUT(n);
    int result = 0;
    assert(n < 10); // prevent things taking too long and tracing filling up the disk
    if (n <= 0) result = 0;
    if (n == 1) result = 1;
    if (n > 1) result = fibonacci(n-1) + fibonacci(n-2);
    MRA_TRACE_FUNCTION_OUTPUT(result);
    return result;
}

