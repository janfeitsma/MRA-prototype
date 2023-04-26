# CODEGEN_NOTE
# it should NOT be modified by user

proto_library(
    name = "interface_proto",
    srcs = glob(["*.proto"]),
    visibility = ["//visibility:public"],
    deps = [
        "//datatypes:MRA_proto",
        BAZEL_INTERFACE_DEPENDENCIES
    ],
)

cc_proto_library(
    name = "interface",
    visibility = ["//visibility:public"],
    deps = [":interface_proto"],
)

