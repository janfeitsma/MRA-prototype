#!/usr/bin/env python3

"""
Tuning tool. Requires a binary tick data file.
Calls the C++ implementation via python bindings.

Example:
    bazel run //components/falcons/localization_vision/test:tune tick_20230702_163900_008.bin

see TODO example.png
"""


# python modules
import sys
import copy
import argparse
import google.protobuf.timestamp_pb2

# own modules
import common
from components.falcons.localization_vision.test import pybind_ext


class TuningTool():
    def __init__(self, filename):
        self.tickdata = common.TickData(filename)

    def run(self):
        state = copy.copy(self.tickdata.state_before)
        output = common.Output_pb2.Output()
        local = common.Local_pb2.Local()
        t = google.protobuf.timestamp_pb2.Timestamp()
        print('py before tick')
        ans = pybind_ext.tick(
         #   t, # timestamp
            self.tickdata.input,
            self.tickdata.params,
            state,
            output,
            local)
        print('after tick')
        print(st)
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

