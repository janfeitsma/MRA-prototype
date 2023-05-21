# this file was produced by MRA-codegen.py from template_interface.BUILD
# it should NOT be modified by user

proto_library(
    name = "interface_proto",
    srcs = glob(["*.proto"]),
    visibility = ["//visibility:public"],
    deps = [
        "@com_google_protobuf//:timestamp_proto",
        "//datatypes:MRA_proto",
        "//components/falcons/velocity-control/interface:interface_proto",
    ],
)

cc_proto_library(
    name = "interface_cc_proto",
    visibility = ["//visibility:public"],
    deps = [
        ":interface_proto",
    ],
)

cc_library(
    name = "interface",
    data = glob(["*DefaultParams.json"]),
    visibility = ["//visibility:public"],
    deps = [
        ":interface_cc_proto",
    ],
)
