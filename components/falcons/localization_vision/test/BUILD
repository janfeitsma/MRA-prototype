
#load("@rules_python//python:defs.bzl", "py_extension")
load("//dependencies/bazel/python:py_extension.bzl", "py_extension")
#load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")

cc_library(
    name = "pywrap",
    hdrs = [
        "wrap2.hpp",
    ],
    srcs = ["wrap2.cpp"],
    copts = ["-fPIC"],
    linkopts = ["-shared"],
    visibility = ["//visibility:public"],
    includes = ["."],
    deps = [
        "@pybind11",
        "//components/falcons/localization_vision:implementation",
    ],        
)

py_extension(
    name = "pybind11_example_module",
    visibility = ["//visibility:public"],
    deps = [
        "@python_linux//:python38-lib",
        ":pywrap",
    ],
)

