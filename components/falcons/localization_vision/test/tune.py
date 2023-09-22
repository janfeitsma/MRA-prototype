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
import signal
from collections import deque
import argparse
import types
import numpy as np
import cv2
import logging
import threading
threading.current_thread().name = "main-gui" # instead of default MainThread

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
    'blurFactor': (0.0, 1.0),
    'actionRadius.x': (0.0, 5.0),
    'actionRadius.y': (0.0, 5.0),
    'actionRadius.rz': (0.0, 7.0),
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
INFO_LINES = [
    'mean tick',
    'return code',
    'best candidate',
]



class TuningTool():
    def __init__(self, filename):
        self.image = None
        self.data = common.Data(filename)
        self.params = parameters.ParametersProxy(self.data.params.solver, RANGE_HINTS)
        self.gui = gui.WindowManager(self.params, callback=self.get_image)
        self._stop = False
        self.max_ticks = None
        # Prepare overlay information
        self.elapsed_stats = deque(maxlen=20)
        self.reset_info()
        # Prepare worker thread (next to GUI thread)
        self.thread = threading.Thread(target=self.loop_tick, name="worker-tick")
        # Set up a signal handler for SIGINT (Ctrl-C)
        signal.signal(signal.SIGINT, self.signal_handler)

    def reset_info(self):
        self.info = {k: '' for k in INFO_LINES}

    def reset_data(self):
        self.data.state.Clear()

    def get_image(self, dummy):
        return self.image

    def run(self):
        self.thread.start()
        self.gui.run()
        self._stop = True

    def stop(self):
        self._stop = True
        #time.sleep(5) # HACK to let the gui show for a while after using -a -n 1 options
        self.gui.app.quit()

    def wait_active(self):
        while not self.params.gui_params.get('active'):
            time.sleep(0.1)

    def loop_tick(self):
        it = 0
        self.wait_active()
        while not self._stop:
            it += 1
            frequency = self.params.gui_params.get('frequency')
            nominal_dt = 1.0 / frequency
            t0 = time.time()
            self.tick()
            elapsed = time.time() - t0
            # sleep according to configured frequency (for which we have a slider)
            tts = nominal_dt - elapsed
            if tts > 0:
                time.sleep(tts)
            # early exit? for testing/debugging
            if self.max_ticks and it >= self.max_ticks:
                self.stop()

    def tick(self):
        # GUI sliders are connected directly to protobuf self.data.params
        # GUI control params, buttons, frequency however need to be handled specially
        # for instance when hitting the reset button, state needs to be wiped

        # Handle reset
        self.reset_info() # refreshed each tick
        if self.params.gui_params.get('reset'):
            # clear state
            self.reset_data()
            # TODO reload params (and update sliders accordingly)
            # all done, reset the reset flag :)
            self.params.gui_params.set('reset', False)

        # If active, then call C++ tick
        # while updating self.image and self.data
        if self.params.gui_params.get('active'):
            t0 = time.time()
            self.tick_call()
            elapsed = time.time() - t0
            # Statistics on effective frequency and duty-cycle
            # (which may include some overhead from pybind data marshalling and overlay creation)
            self.elapsed_stats.append(elapsed)
        else:
            self.elapsed_stats.clear()

        # Make info lines; these are important enough to also dump to stdout
        info_lines = self.make_info_lines()
        if self.params.gui_params.get('active'):
            print('\n'.join(info_lines))

        # Optional overlay
        if self.params.gui_params.get('overlay'):
            self.add_overlay(info_lines)

    def tick_call(self):
        t = self.data.t
        return_tuple = pybind_ext.tick(
            #t,
            self.data.input,
            self.data.params,
            self.data.state)
        return_code = return_tuple[0]
        self.info['return code'] = 'return code: {:d}'.format(return_code)
        self.data.state = return_tuple[1]
        self.data.output = return_tuple[2]
        self.data.local = return_tuple[3] # local is a synonym for diagnostics. If too confusing, then lets bulk-rename? (Naming came from FMI standard.)
        print(self.data.output)
        # convert CvMatProto object to opencv object
        c = self.data.local.floor
        np_data = np.frombuffer(c.data, dtype=np.uint8)
        self.image = cv2.cvtColor(np_data.reshape(c.height, c.width, -1), cv2.COLOR_BGR2RGB)

    def make_info_lines(self):
        # Determine text lines to show
        if len(self.elapsed_stats):
            self.info['mean tick'] = 'mean tick: {:.1f}ms (n={:d})'.format(1e3 * np.mean(self.elapsed_stats), len(self.elapsed_stats))
        self.info['best candidate'] = 'N/A'
        if len(self.data.output.candidates):
            c = self.data.output.candidates[0]
            self.info['best candidate'] = 'best candidate: x={:7.3f} y={:7.3f} rz={:7.3f} conf={:5.3f}'.format(c.pose.x, c.pose.y, c.pose.rz, c.confidence)
        # Make array of lines (using OrderedDict and layout specification preference from INFO_LINES)
        # Replace missing data with empty strings, to keep consistent line layout/spacing
        info_lines = self.info.values()
        return info_lines

    def add_overlay(self, info_lines):
        # Add text overlay
        for i, line in enumerate(info_lines):
            #cv2.putText(self.image, line, (10, 30 + i*20), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (255, 0, 0), 2)
            cv2.putText(self.image, line, (10, 30 + i*20), 1, 1, (255, 0, 0), 1)

    def signal_handler(self, s, frame):
        print() # so ^C appears on its own line
        self.stop()


def parse_args(args: list) -> argparse.Namespace:
    """
    Use argparse to parse command line arguments.
    """
    descriptionTxt = __doc__
    exampleTxt = ''
    class CustomFormatter(argparse.ArgumentDefaultsHelpFormatter, argparse.RawDescriptionHelpFormatter):
        pass
    parser = argparse.ArgumentParser(description=descriptionTxt, epilog=exampleTxt, formatter_class=CustomFormatter)
    parser.add_argument('-a', '--activate', help='start ticking, do not wait for user to press the GUI button', action='store_true')
    parser.add_argument('-r', '--reset', help='clear state data instead of using it', action='store_true')
    parser.add_argument('-n', '--ticks', help='run for n ticks, default forever', type=int)
    parser.add_argument('-d', '--debug', help='enable highly experimental autologging/tracing', action='store_true')
    parser.add_argument('datafile', help='data file to load')
    return parser.parse_args(args)


def main(args: argparse.Namespace) -> None:
    """
    Run the tuning tool.
    """
    if args.debug:
        # JFEI-private debugging (based on autologging extensions in my repo https://github.com/janfeitsma/extendedlogging)
        # slap some tracing decorators onto our code, automagically!
        import tracing
    # setup and run the tuning tool
    t = TuningTool(args.datafile)
    t.max_ticks = args.ticks
    if args.activate:
        t.params.set('active', True)
    if args.reset:
        t.reset_data()
    t.run()


if __name__ == '__main__':
    main(parse_args(sys.argv[1:]))

