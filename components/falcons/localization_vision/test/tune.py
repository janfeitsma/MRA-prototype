#!/usr/bin/env python3

"""
Tuning tool. Requires a binary tick data file.
Calls the C++ implementation via python bindings.

Example: see TODO example.png
"""


# python modules
import sys
import copy
import argparse
import google.protobuf.timestamp_pb2

# own modules
import common


class TuningTool():
    def __init__(self, filename):
        self.tickdata = common.TickData(filename)

    def run(self):
        print(dir(common.protobuf2python.pybind11_example_module))
        state = copy.copy(self.tickdata.state_before)
        output = common.protobuf2python.Output_pb2.Output()
        local = common.protobuf2python.Local_pb2.Local()
        t = google.protobuf.timestamp_pb2.Timestamp()
        print('before tick')
        ans = common.protobuf2python.pybind11_example_module.cpp_function(
            t, # timestamp
            self.tickdata.input,
            self.tickdata.params,
            state,
            output,
            local)
        print('after tick')
        print(output)


def parse_args(args: list) -> argparse.Namespace:
    """
    Use argparse to parse command line arguments.
    """
    descriptionTxt = __doc__
    exampleTxt = ''
    class CustomFormatter(argparse.ArgumentDefaultsHelpFormatter, argparse.RawDescriptionHelpFormatter):
        pass
    parser = argparse.ArgumentParser(description=descriptionTxt, epilog=exampleTxt, formatter_class=CustomFormatter)
    parser.add_argument('datafile', help='data file to load')
    return parser.parse_args(args)


def main(args: argparse.Namespace) -> None:
    """
    Run the tuning tool.
    """
    t = TuningTool(args.datafile)
    t.run()


if __name__ == '__main__':
    main(parse_args(sys.argv[1:]))

