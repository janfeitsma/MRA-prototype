cc_library(
    name = "clipping",
    hdrs = [
        "clipping.hpp",
    ],
    srcs = [
        "clipping.cpp",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "pose",
    hdrs = [
        "pose.hpp",
        "position.hpp",
        "velocity.hpp",
    ],
    srcs = [
        "pose.cpp",
        "position.cpp",
        "velocity.cpp",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":clipping",
        "//datatypes:MRA_cc_proto",
    ],
)

cc_library(
    name = "geometry",
    hdrs = [
        "geometry.hpp",
    ],
    visibility = ["//visibility:public"],
    includes = ["."],
    deps = [
        ":pose",
        ":clipping",
    ],
)

