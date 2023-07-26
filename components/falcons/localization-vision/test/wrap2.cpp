#include <pybind11/pybind11.h>
#include <wrap2.hpp>

PYBIND11_MODULE(pybind11_example_module, m) {
    m.doc() = "pybind11 example plugin"; // Optional module docstring
    m.def("cpp_function", &pywrap_tick_standalone, "test function");
}

int pywrap_tick_standalone(
    float a,
    int b)
{
    return (int)(b * a);
}

