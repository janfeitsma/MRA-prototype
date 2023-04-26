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

