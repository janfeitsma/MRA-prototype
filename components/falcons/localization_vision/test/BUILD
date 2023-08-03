load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")


pybind_extension(
    name = "pybind_ext",
    srcs = ["pybind_ext.cpp"],
    deps = [
        "//components/falcons/localization_vision:implementation",
        "@pybind11_protobuf//pybind11_protobuf:native_proto_caster",
    ],
)

