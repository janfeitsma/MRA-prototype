import sys
import numpy as np
from PyQt5.QtWidgets import QApplication
from gui_opencv import OpenCVWindow
from gui_config import ConfigurationWindow
from gui_parameters import Parameters, Param


def create_standard_gui_params(params=None):
    if params is None:
        params = Parameters()
    # control buttons
    params.add('active', bool, slider=False)
    params.add('overlay', bool, slider=False, default_value=True)
    params.add('reset', bool, slider=False)
    # sliders: name, type, min, max, default
    params.add('frequency', float, 0.1, 50.0, 10.0)
    params.add('zoom', float, 0.2, 4.0, 0.7)
    return params


def make_gui_params(*custom_params):
    result = create_standard_gui_params()
    for p in custom_params:
        result.add(*p)
    return result


class WindowManager():
    """
    Setup the windows. Arguments:
    * app: a QApplication instance
    * params: a Parameters object, possibly loaded from protobuf Params
    * callback: a function that outputs a opencv object
    """
    def __init__(self, params, callback=None, app=None):
        self.params = params
        self.app = app if app is not None else QApplication([])
        self.callback = callback

        # Create and show Sliders window
        self.config_window = ConfigurationWindow(self.params)
        self.config_window.show()

        # Create and show OpenCV window
        self.opencv_window = OpenCVWindow(self.params, self.callback)
        self.opencv_window.show()

        # Close application when either window is closed
        self.config_window.closeEvent = lambda event: self.shutdown()
        self.opencv_window.closeEvent = lambda event: self.shutdown()

    def shutdown(self):
        self.app.quit()

    def run(self):
        self.app.exec()


def main(qargs=[]):
    # Create some dummy parameters
    params = make_gui_params(
        # name, type, min, max, default
        ('r', int, 0, 255, 100),
        ('g', int, 0, 255, 23),
        ('b', int, 0, 255, 200),
        ('factor', float, 0.0, 1.0, 1.0),
        ('enabled', bool, False, True, True)) # min=False and max=True are obvious and could be omitted maybe ...

    # Create example image generator
    def example_callback(params):
        img = np.zeros((300, 400, 3), dtype=np.uint8)
        r, g, b = params.get('r'), params.get('g'), params.get('b')
        e, f = params.get('enabled'), params.get('factor')
        img[:, :100] = [r*f*e, 0, 0]
        img[:, 100:] = [0, params.get('g')*f*e, params.get('b')*f*e]
        return img

    # Construct and run the application
    app = QApplication(qargs)
    window_manager = WindowManager(params, example_callback, app)
    window_manager.run()


if __name__ == "__main__":
    main(sys.argv)

