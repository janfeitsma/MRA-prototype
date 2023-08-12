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

        self.image = self.initial_image() # NO DATA (yet)
        self.image_label = QLabel()

        layout = QVBoxLayout()
        layout.addWidget(self.image_label)
        self.setLayout(layout)

        # Default config window appears top-left, let's try to put image next to it
        self.move(750, 100)

        # Create a timer to update the image periodically (e.g., every 100ms)
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.update_image)
        self.timer.start(100)

    def initial_image(self):
        # Image dimensions
        width = 400
        height = 300
        # Create a black background image
        image = np.zeros((height, width, 3), dtype=np.uint8)
        # Define the text and its properties
        text = 'NO DATA'
        font_scale = 2
        font_thickness = 3
        font_color = (255, 0, 0)  # Red color in BGR format
        # Calculate text size
        text_size = cv2.getTextSize(text, cv2.FONT_HERSHEY_SIMPLEX, font_scale, font_thickness)[0]
        text_width, text_height = text_size
        # Calculate text position for centering
        x_position = (width - text_width) // 2
        y_position = (height + text_height) // 2
        # Put the text on the black background
        cv2.putText(image, text, (x_position, y_position), cv2.FONT_HERSHEY_SIMPLEX, font_scale, font_color, font_thickness)
        return image

    def update_image(self):
        # retrieve via callback
        if self.callback:
            cb_image = self.callback(self.parameters)
            if not cb_image is None:
                self.image = cb_image
            # otherwise: no data yet, do not modify initial_image
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


