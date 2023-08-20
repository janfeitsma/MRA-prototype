py_test(
    name = "python_protobuf",
    size = "small",
    srcs = [
        "demo_protobuf.py",
    ],
    main = "demo_protobuf.py",
    deps = [
        "//components/falcons/velocity_control/interface:interface_py_proto",
    ],
    data = [
        "//components/falcons/velocity_control/interface", # needed for DefaultParams.json
    ],
)

