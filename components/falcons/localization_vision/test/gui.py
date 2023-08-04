from PyQt5.QtWidgets import QApplication
from gui_opencv import OpenCVWindow
from gui_sliders import SlidersWindow

class WindowManager():
    def __init__(self):

        # Create and show Sliders window
        self.sliders_window = SlidersWindow()
        self.sliders_window.show()

        # Create and show OpenCV window, TODO connect with sliders
        self.opencv_window = OpenCVWindow()
        self.opencv_window.show()


def main(qargs = []):
    app = QApplication(qargs)
    window_manager = WindowManager()
    sys.exit(app.exec_())


if __name__ == "__main__":
    import sys
    main(sys.argv)

