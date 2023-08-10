load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")


pybind_extension(
    name = "pybind_ext",
    srcs = ["pybind_ext.cpp"],
    visibility = ["//visibility:public"],
    deps = [
        "//components/falcons/localization_vision:implementation",
        "@pybind11_protobuf//pybind11_protobuf:native_proto_caster",
    ],
)

py_binary(
    name = "tune",
    srcs = [
        "tune.py",
        "common.py",
        "gui.py",
        "gui_opencv.py",
        "gui_config.py",
        "gui_parameters.py",
        "parameters.py",
        "tracing.py",
    ],
    python_version = "PY3",
    visibility = ["//visibility:public"],
    deps = [
        "//components/falcons/localization_vision/interface:interface_py_proto",
    ],
    data = [
        ":pybind_ext.so",
        "@com_google_protobuf//:protobuf_python",
    ]
)

