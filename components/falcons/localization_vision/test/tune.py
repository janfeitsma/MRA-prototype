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
from google.protobuf.message import Message

# own modules
import common
import gui
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


class ParametersProxy(gui.Parameters):
    """
    Connect sliders with protobuf Params object.
    """
    def __init__(self, params_proto):
        super().__init__()
        self.params_proto = params_proto
        gui.create_standard_gui_params(self) # default zoom etc
        self._init_params(params_proto.solver) # only make solver configurable, not the field dimension model A,B,C etc

    def _init_params(self, proto, parent_name=''):
        for field, value in proto.ListFields():
            name = f'{parent_name}.{field.name}' if parent_name else field.name
            if isinstance(value, str):  # Handling string fields
                raise NotImplementedError('strings not yet supported in tuning')
                self.add(name, str, None, None, value)
            elif isinstance(value, int):  # Handling integer fields
                min_val, max_val = RANGE_HINTS[name]
                self.add(name, int, min_val, max_val, value)
            elif isinstance(value, float):  # Handling float fields
                min_val, max_val = RANGE_HINTS[name]
                self.add(name, float, min_val, max_val, value)
            elif isinstance(value, bool):  # Handling boolean fields
                self.add(name, bool, False, True, value)
            elif isinstance(value, Message):  # Handling nested protobuf messages
                self._init_params(value, name)

    def _construct(self, *args, **kwargs):
        result = gui.Param(*args, **kwargs)
        result.notify = self._update_protobuf
        return result

    def _update_protobuf(self, name, value):
        names = name.split('.')
        current_proto = self.params_proto.solver # only make solver configurable, not the field dimension model A,B,C etc
        for n in names[:-1]:
            current_proto = getattr(current_proto, n)
        setattr(current_proto, names[-1], value)

    def __repr__(self):
        return '\n'.join([repr(p) for p in self.params.values()])


class TuningTool():
    def __init__(self, filename):
        self.data = common.Data(filename)
        self.params = ParametersProxy(self.data.params)
        self.gui = gui.WindowManager(self.params, callback=self.tick)

    def tick(self, dummy):
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
        image = cv2.cvtColor(np_data.reshape(c.height, c.width, -1), cv2.COLOR_BGR2RGB)

        time.sleep(0.5) # TODO
        return image


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

