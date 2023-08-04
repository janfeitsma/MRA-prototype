from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QWidget, QGridLayout, QLabel, QSlider

class ConfigurationWindow(QWidget):
    def __init__(self, parameters):
        super(ConfigurationWindow, self).__init__()
        self.setWindowTitle("Configuration")
        grid_layout = QGridLayout()
        for p in parameters:
            row = self.createParameterRow(p)
            self.addRowToLayout(grid_layout, row)
        grid_layout.setColumnStretch(0, 0)  # Fix the first column width
        grid_layout.setColumnStretch(1, 0)  # Fix the second column width
        grid_layout.setColumnStretch(2, 1)  # Allow the slider column to stretch
        grid_layout.setColumnStretch(3, 0)  # Fix the fourth column width
        grid_layout.setColumnStretch(4, 0)  # Fix the fifth column width
        self.setLayout(grid_layout)
        self.setGeometry(100, 100, 500, 200)  # Set a wider window size

    def createParameterRow(self, parameter):
        name_label = QLabel(parameter.name)
        min_label = QLabel(str(parameter.min_value))
        slider = QSlider(Qt.Horizontal)
        value_label = QLabel()
        value_label.setAlignment(Qt.AlignRight)
        value_label.setMinimumWidth(60)
        max_label = QLabel(str(parameter.max_value))

        slider.setMinimumWidth(200)

        if parameter.value_type == int:
            slider.setRange(parameter.min_value, parameter.max_value)
            slider.setSingleStep(1)
        else:
            slider.setRange(0, 100)
            slider.setSingleStep(1)
            slider.setValue(int((parameter.value - parameter.min_value) * 100 / (parameter.max_value - parameter.min_value)))

        slider.valueChanged.connect(lambda value: self.on_slider_value_changed(value, parameter, value_label))

        row = [
            name_label,
            min_label,
            slider,
            value_label,
            max_label
        ]

        return row

    def addRowToLayout(self, layout, row):
        row_index = layout.rowCount()
        for col_index, widget in enumerate(row):
            layout.addWidget(widget, row_index, col_index)

    def on_slider_value_changed(self, value, parameter, value_label):
        if parameter.value_type == int:
            parameter.value = value
        else:
            parameter.value = parameter.min_value + (value / 100) * (parameter.max_value - parameter.min_value)
        value_label.setText(f'{parameter.value:.2f}')  # Format the float value

