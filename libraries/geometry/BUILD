cc_library(
    name = "angles",
    hdrs = [
        "angles.hpp",
    ],
    srcs = [
        "angles.cpp",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "point",
    hdrs = [
        "point.hpp",
    ],
    srcs = [
        "point.cpp",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//datatypes:MRA_cc_proto",
    ],
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
        ":point",
        ":angles",
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
        "tmp_vector2d.hpp",
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
        "tmp_vector2d.hpp",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":pose",
        ":angles",
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
        ":point",
        ":pose",
        ":position",
        ":velocity",
        ":angles",
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

