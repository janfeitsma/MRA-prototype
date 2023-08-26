
cc_library(
    name = "logging",
    hdrs = [
        "logging.hpp", # main header, others are helpers/subcomponents
        "frontend.hpp",
        "backend.hpp",
        "logtick.hpp",
        "macros.hpp",
        "levels.hpp",
        "control.hpp",
    ],
    srcs = [
        "backend.cpp",
    ],
    visibility = ["//visibility:public"],
    includes = ["."],
    deps = [
    	"@gabime_spdlog",
        "//base:abstract_interface",
    ],
)

