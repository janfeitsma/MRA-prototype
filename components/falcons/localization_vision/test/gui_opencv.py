import cv2
import numpy as np
from PyQt5.QtCore import QTimer
from PyQt5.QtGui import QImage, QPixmap
from PyQt5.QtWidgets import QLabel, QWidget, QVBoxLayout

class OpenCVWindow(QWidget):
    def __init__(self, parameters, callback=None):
        super(OpenCVWindow, self).__init__()
        self.setWindowTitle('OpenCV image')
        self.parameters = parameters
        self.callback = callback

        # Dummy OpenCV object (blue rectangle)
        self.image = np.zeros((300, 400, 3), dtype=np.uint8)
        self.image[:] = (255, 0, 0)
        self.image_label = QLabel()

        layout = QVBoxLayout()
        layout.addWidget(self.image_label)
        self.setLayout(layout)

        # Create a timer to update the image periodically (e.g., every 100ms)
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.update_image)
        self.timer.start(100)

    def update_image(self):
        # retrieve via callback
        if self.callback:
            self.image = self.callback(self.parameters)
        image = self.image
        # inspect image, set title
        zoom = self.parameters.get('zoom')
        height, width, channel = image.shape
        title = f'OpenCV image ({height}x{width})'
        if zoom != 1.0:
            title = f'OpenCV image ({height}x{width}, zoom={zoom:.2f})'
        self.setWindowTitle(title)
        # zoom and scale
        if zoom != 1.0:
            width = int(image.shape[1] * zoom)
            height = int(image.shape[0] * zoom)
            image = cv2.resize(image, (width, height), interpolation=cv2.INTER_LINEAR)
        # convert to QImage and QPixmap
        bytes_per_line = 3 * width
        q_image = QImage(
            image.data,
            width,
            height,
            bytes_per_line,
            QImage.Format_RGB888
        )
        pixmap = QPixmap.fromImage(q_image)
        self.image_label.setPixmap(pixmap)
        self.adjustSize()


