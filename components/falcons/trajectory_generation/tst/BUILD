
cc_binary(
    name = "trajectory-generator",
    srcs = [
        "trajectory-main.cpp",
    ],
    deps = [
        "//components/falcons/trajectory_generation:implementation",
    ],
)

sh_test(
    name = "cli-tests",
    size = "small",
    srcs = ["cli-tests.sh"],
    data = glob(["*.txt"]) + [
        "trajectory.py",
        ":trajectory-generator",
    ],
    args = ["$(location :trajectory-generator)"],
)

