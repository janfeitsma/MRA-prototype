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

# python part disabled for now, because it is hardly used and creates some spam during bazel build:
#     INFO: From ProtoCompile datatypes/ActionResult_pb2.py:
#     external/com_google_protobuf/python: warning: directory does not exist.
# see also:
#     https://github.com/protocolbuffers/protobuf/issues/6049
# which is solved in a newer version
#
#load("@com_google_protobuf//:protobuf.bzl", "py_proto_library")
#
#py_proto_library(
#    name = "MRA_proto_py",
#    srcs = glob(["*.proto"]),
#    visibility = ["//visibility:public"],
#    deps = [
#        "@com_google_protobuf//:protobuf_python",
#    ],
#)

