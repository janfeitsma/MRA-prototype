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
import time
import argparse
import types
import numpy as np
import cv2
import threading

# own modules
import common
import gui
import parameters
from components.falcons.localization_vision.test import pybind_ext


# parameter range hints, used to determine min/max limits of the GUI sliders
# TODO: find a more structural way, maybe in .proto comments and parse somehow?
RANGE_HINTS = {
    'pixelsPerMeter': (10, 200),
    'floorBorder': (0.1, 2.0),
    'blurFactor': (0.0, 5.0),
    'actionRadius.x': (0.0, 2.0),
    'actionRadius.y': (0.0, 2.0),
    'actionRadius.rz': (0.0, 2.0),
    'maxCount': (0, 1000),
    'epsilon': (0.0, 1e-2),
    'linePoints.fit.radiusConstant': (0.0, 1.0),
    'linePoints.fit.radiusScaleFactor': (-0.1, 0.1),
    'linePoints.fit.radiusMinimum': (-0.1, 0.5),
    'linePoints.plot.radius': (0.0, 0.3),
    'guessing.random.count': (0, 10),
    'guessing.random.searchRadius': (0.0, 5.0),
    'guessing.random.exclusionRadius': (0.0, 5.0),
    'guessing.random.maxTries': (0, 10),
}



class TuningTool():
    def __init__(self, filename):
        self.image = None
        self.data = common.Data(filename)
        self.params = parameters.ParametersProxy(self.data.params, RANGE_HINTS)
        self.gui = gui.WindowManager(self.params, callback=self.get_image)
        self.thread = threading.Thread(target=self.runner)
        self.thread.start()

    def get_image(self, dummy):
        return self.image

    def runner(self):
        while True:
            self.tick()
            time.sleep(1)

    def tick(self):
        print('py before tick')
        t = self.data.t
        ans = pybind_ext.tick(
            #t,
            self.data.input,
            self.data.params,
            self.data.state,
            self.data.output,
            self.data.local) # local is a synonym for diagnostics. If too confusing, then lets bulk-rename? (Naming came from FMI standard.)
        print('py after tick, ans=', ans)
        print(self.data.output)

        # convert CvMatProto object to opencv object
        c = self.data.local.floor
        np_data = np.frombuffer(c.data, dtype=np.uint8)
        self.image = cv2.cvtColor(np_data.reshape(c.height, c.width, -1), cv2.COLOR_BGR2RGB)


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
    t.gui.run()


if __name__ == '__main__':
    main(parse_args(sys.argv[1:]))

