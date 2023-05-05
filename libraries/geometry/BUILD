cc_library(
    name = "pose",
    hdrs = [
        "pose.hpp",
    ],
    srcs = [
        "pose.cpp",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//datatypes:MRA_cc_proto",
    ],
)

cc_library(
    name = "geometry",
    hdrs = [
        "geometry.hpp",
    ],
    srcs = [
        "geometry.cpp",
    ],
    visibility = ["//visibility:public"],
    includes = ["."],
    deps = [
        ":pose",
    ],
)

