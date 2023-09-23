
cc_library(
    name = "logging",
    hdrs = [
        "logging.hpp", # main header, others are helpers/subcomponents
        "frontend.hpp",
        "backend.hpp",
        "logtick.hpp",
        "macros.hpp",
        "macromap.h",
        "spdlogformatter.hpp",
        "levels.hpp",
        "control.hpp",
    ],
    srcs = [
        "backend.cpp",
        "control.cpp",
    ],
    visibility = ["//visibility:public"],
    includes = ["."],
    linkopts = ["-lrt"],  # for shm_open etc
    deps = [
    	"@gabime_spdlog",
        "//base:abstract_interface",
        "//datatypes:MRA_cc_proto",
    ],
)

py_test(
    name = "check_logfolder",
    srcs = ["check_logfolder.py"],
)

