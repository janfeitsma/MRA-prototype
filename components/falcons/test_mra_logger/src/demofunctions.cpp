#include "demofunctions.hpp"
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

float hypotenuse(float a, float b)
{
    MRA_TRACE_FUNCTION_INPUTS(a, b);
    float result = sqrt(a * a + b * b);
    MRA_TRACE_FUNCTION_OUTPUT(result);
    return result;
}

bool parity(int a, int b, int c)
{
    MRA_TRACE_FUNCTION_INPUTS(a, b, c);
    bool result = (a * b * c) & 1;
    MRA_TRACE_FUNCTION_OUTPUT(result);
    return result;
}

std::string stringReverse(std::string const &s)
{
    MRA_TRACE_FUNCTION_INPUTS(s);
    std::string result(s.rbegin(), s.rend());
    MRA_TRACE_FUNCTION_OUTPUT(result);
    return result;
}

std::string stringRepeater(std::string const &s, int n)
{
    MRA_TRACE_FUNCTION_INPUTS(s, n);
    std::string result;
    while (n--) {
        result += s;
    }
    MRA_TRACE_FUNCTION_OUTPUT(result);
    return result;
}

void mysleep(float duration)
{
    MRA_TRACE_FUNCTION_INPUTS(duration);
    if (duration > 0) {
        usleep(int(1e6 * duration));
    }
}

