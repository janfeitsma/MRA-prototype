import cv2
import numpy as np
from PyQt5.QtCore import QTimer
from PyQt5.QtGui import QImage, QPixmap
from PyQt5.QtWidgets import QLabel, QWidget, QVBoxLayout

class OpenCVWindow(QWidget):
    def __init__(self):
        super(OpenCVWindow, self).__init__()
        self.setWindowTitle("OpenCV image")

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
        height, width, channel = self.image.shape
        self.setWindowTitle(f"OpenCV image ({height}x{width})")
        bytes_per_line = 3 * width
        q_image = QImage(
            self.image.data,
            width,
            height,
            bytes_per_line,
            QImage.Format_RGB888
        )
        pixmap = QPixmap.fromImage(q_image)
        self.image_label.setPixmap(pixmap)

