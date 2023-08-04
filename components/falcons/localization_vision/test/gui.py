import sys
import cv2
import numpy as np
from PyQt5.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QWidget, QSlider, QLabel
from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtGui import QImage, QPixmap
from dataclasses import dataclass

@dataclass
class Param:
    min_val: int
    max_val: int
    default_val: int

class Gui(QMainWindow):
    def __init__(self, params, callback):
        super().__init__()
        self.params = params
        self.callback = callback
        self.initUI()

    def initUI(self):
        self.setWindowTitle('Tuning Tool')
        self.setGeometry(100, 100, 800, 600)

        layout = QVBoxLayout()
        self.central_widget = QWidget()
        self.central_widget.setLayout(layout)
        self.setCentralWidget(self.central_widget)

        self.sliders = {}
        for param_name, param_info in self.params.items():
            slider = QSlider(Qt.Horizontal)
            slider.setMinimum(param_info.min_val)
            slider.setMaximum(param_info.max_val)
            slider.setValue(param_info.default_val)
            slider.setTickInterval((param_info.max_val - param_info.min_val) // 10)
            slider.valueChanged.connect(self.updateSliderValue)
            layout.addWidget(QLabel(param_name))
            layout.addWidget(slider)
            self.sliders[param_name] = slider

        self.image_label = QLabel(self)
        layout.addWidget(self.image_label)

        # Create a timer to update the image periodically (e.g., every 100ms)
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.updateImage)
        self.timer.start(100)

    def updateSliderValue(self, value):
        # This function will be called whenever a slider value changes
        # You can access the current values of the sliders using self.sliders dictionary
        # and call your C++ function with the updated parameter values to get a new image
        img = self.callback({param_name: slider.value() for param_name, slider in self.sliders.items()})
        self.displayImage(img)

    def updateImage(self):
        # This function will be called periodically by the timer to update the displayed image
        # You can call your C++ function here with the current slider values to get the image
        img = self.callback({param_name: slider.value() for param_name, slider in self.sliders.items()})
        self.displayImage(img)

    def displayImage(self, img):
        # Convert the OpenCV image to QImage and display it on the GUI
        height, width, channel = img.shape
        bytes_per_line = 3 * width
        q_img = QImage(img.data, width, height, bytes_per_line, QImage.Format_RGB888)
        pixmap = QPixmap.fromImage(q_img)
        self.image_label.setPixmap(pixmap)

if __name__ == '__main__':
    # For standalone testing only
    app = QApplication(sys.argv)
    params = {
        'param1': Param(min_val=0, max_val=100, default_val=50),
        'param2': Param(min_val=0, max_val=255, default_val=127),
    }

    def dummy_callback(params):
        # For standalone testing, provide a dummy callback function
        # Replace this with your actual C++ function using OpenCV in "tune.py"
        img = np.zeros((200, 200, 3), dtype=np.uint8)
        img[:, :100] = [params.get('param1'), 0, 0]
        img[:, 100:] = [0, params.get('param2'), 0]
        return img

    gui = Gui(params, dummy_callback)
    gui.show()
    sys.exit(app.exec_())
