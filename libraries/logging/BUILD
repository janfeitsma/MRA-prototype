cc_library(
    name = "datatypes",
    hdrs = [
        "datatypes.hpp",
    ],
    visibility = ["//visibility:public"],
    deps = [
    ],
)


cc_library(
    name = "logging",
    hdrs = [
        "logging.hpp",
        "MRA_logger.hpp",
    ],
    srcs = [
        "logging.cpp", 
        "MRA_logger.cpp", 
    ],
    visibility = ["//visibility:public"],
    includes = ["."],
    deps = [
    	"@gabime_spdlog",
        "//base:abstract_interface",
    ],
)

