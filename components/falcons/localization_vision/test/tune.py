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


# own modules
import common
from components.falcons.localization_vision.test import pybind_ext


class TuningTool():
    def __init__(self, filename):
        self.data = common.Data(filename)

    def run(self):
        print('py before tick')
        #self.data.setDefaultParams()
        self.data.params.solver.linePoints.fit.radiusConstant = 5e-2 # hack for old .bin
        t = self.data.t
        ans = pybind_ext.tick(
            #t,
            self.data.input,
            self.data.params,
            self.data.state,
            self.data.output,
            self.data.local)
        print('py after tick, ans=', ans)
        print(self.data.output)


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

