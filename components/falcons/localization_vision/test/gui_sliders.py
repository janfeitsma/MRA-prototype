from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QSlider, QWidget, QVBoxLayout

class SlidersWindow(QWidget):
    def __init__(self):
        super(SlidersWindow, self).__init__()
        self.setWindowTitle("Sliders")

        self.slider_r = QSlider(Qt.Horizontal)
        self.slider_r.setRange(0, 255)

        self.slider_g = QSlider(Qt.Horizontal)
        self.slider_g.setRange(0, 255)

        self.slider_b = QSlider(Qt.Horizontal)
        self.slider_b.setRange(0, 255)

        layout = QVBoxLayout()
        layout.addWidget(self.slider_r)
        layout.addWidget(self.slider_g)
        layout.addWidget(self.slider_b)
        self.setLayout(layout)

