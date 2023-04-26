# this file was produced by codegen.py from template_interface.BUILD
# it should NOT be modified by user

proto_library(
    name = "interface_proto",
    srcs = glob(["*.proto"]),
    visibility = ["//visibility:public"],
    deps = [
        "//MRA/proto:MRA_proto_lib",
        
    ],
)

cc_proto_library(
    name = "interface",
    visibility = ["//visibility:public"],
    deps = [":interface_proto"],
)

