load("@rules_proto//proto:defs.bzl", "proto_library")
load("@com_github_grpc_grpc//bazel:python_rules.bzl", "py_proto_library")

proto_library(
    name = "MRA_proto",
    srcs = glob(["*.proto"]),
    visibility = ["//visibility:public"],
    deps = [
        "@com_google_protobuf//:timestamp_proto",
    ],
)

cc_proto_library(
    name = "MRA_cc_proto",
    visibility = ["//visibility:public"],
    deps = [":MRA_proto"],
)

py_proto_library(
    name = "MRA_proto_py",
    visibility = ["//visibility:public"],
    deps = [
        ":MRA_proto",
    ],
)

