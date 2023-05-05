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
    ],
    srcs = [
        "pose.cpp",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":clipping",
        "//datatypes:MRA_cc_proto",
    ],
)

cc_library(
    name = "position",
    hdrs = [
        "position.hpp",
    ],
    srcs = [
        "position.cpp",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":pose",
    ],
)

cc_library(
    name = "velocity",
    hdrs = [
        "velocity.hpp",
    ],
    srcs = [
        "velocity.cpp",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":pose",
        ":clipping",
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
        ":position",
        ":velocity",
        ":clipping",
    ],
)

cc_test(
    name = "test-pose",
    srcs = [
        "test-pose.cpp",
    ],
    deps = [
        ":pose",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "test-position",
    srcs = [
        "test-position.cpp",
    ],
    deps = [
        ":position",
        "@com_google_googletest//:gtest_main",
    ],
)

