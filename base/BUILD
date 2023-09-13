cc_library(
    name = "commons",
    includes = ["."],
    hdrs = [
        "params_loader.hpp",
        "json_convert.hpp",
    ],
    srcs = [
        "json_convert.cpp",
    ],
    deps = [
        "@com_google_protobuf//:protobuf",
        "@nlohmann_json",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "testing_base",
    includes = ["."],
    hdrs = [
        "test_factory.hpp",
    ],
    deps = [
        ":commons",
        "//testdata:common_testdata",
        "@com_google_googletest//:gtest_main",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "abstract_interface",
    includes = ["."],
    hdrs = [
        "abstract_interface.hpp",
    ],
    deps = [
        ":commons",
    ],
    visibility = ["//visibility:public"],
)
