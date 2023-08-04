from PyQt5.QtCore import Qt
from PyQt5 import QtGui
from PyQt5.QtWidgets import QWidget, QGridLayout, QLabel, QSlider, QLineEdit

SLIDER_RESOLUTION = 100



def parameter_value_float2slider(parameter):
    return int((parameter.value - parameter.min_value) * SLIDER_RESOLUTION / (parameter.max_value - parameter.min_value))

def parameter_value_slider2float(parameter, slider_int_value):
    return float(slider_int_value) / SLIDER_RESOLUTION * (parameter.max_value - parameter.min_value) + parameter.min_value


class ConfigurationWindow(QWidget):
    def __init__(self, parameters):
        super(ConfigurationWindow, self).__init__()
        self.setWindowTitle("Configuration")
        grid_layout = QGridLayout()
        self.add_row_to_layout(grid_layout, self.create_header_row())
        for p in parameters:
            row = self.create_parameter_row(p)
            self.add_row_to_layout(grid_layout, row)
        grid_layout.setColumnStretch(0, 0)  # Fix the first column width
        grid_layout.setColumnStretch(1, 0)  # Fix the second column width
        grid_layout.setColumnStretch(2, 1)  # Allow the slider column to stretch
        grid_layout.setColumnStretch(3, 0)  # Fix the fourth column width
        grid_layout.setColumnStretch(4, 0)  # Fix the fifth column width
        self.setLayout(grid_layout)
        self.setGeometry(100, 100, 500, 200)  # Set a wider window size

    def create_header_row(self):
        result = [
            QLabel('parameter'),
            QLabel('min value'),
            QLabel('slider'),
            QLabel('cur value'),
            QLabel('max value'),
        ]
        # make bold
        font = result[0].font()
        font.setBold(True)
        for e in result:
            e.setFont(font)
        return result

    def create_parameter_row(self, parameter):
        name_label = QLabel(parameter.name)
        min_label = QLabel(str(parameter.min_value))
        slider = QSlider(Qt.Horizontal)
        value_label = QLabel()
        value_label.setAlignment(Qt.AlignRight)
        value_label.setMinimumWidth(60)
        max_label = QLabel(str(parameter.max_value))

        slider.setMinimumWidth(200)

        if parameter.value_type == float:
            slider.setRange(0, SLIDER_RESOLUTION)
            slider.setSingleStep(1)
            slider.setValue(parameter_value_float2slider(parameter))
        else:
            slider.setRange(parameter.min_value, parameter.max_value)
            slider.setSingleStep(1)
            slider.setValue(parameter.value)

        slider.valueChanged.connect(lambda value: self.on_slider_value_changed(value, parameter, value_label, value_edit))

        value_edit = QLineEdit()
        value_edit.setAlignment(Qt.AlignRight)
        value_edit.setValidator(QtGui.QDoubleValidator())
        value_edit.editingFinished.connect(lambda: self.update_slider(parameter, value_edit, slider))

        self.set_label_text(parameter, value_label, value_edit)

        row = [
            name_label,
            min_label,
            slider,
            value_edit,
            max_label
        ]

        return row

    def update_slider(self, parameter, value_edit, slider):
        parameter.set(value_edit.text())
        if parameter.value_type == float:
            slider.setValue(parameter_value_float2slider(parameter))
        else:
            slider.setValue(parameter.value)

    def add_row_to_layout(self, layout, row):
        row_index = layout.rowCount()
        for col_index, widget in enumerate(row):
            layout.addWidget(widget, row_index, col_index)

    def on_slider_value_changed(self, new_value, parameter, value_label, value_edit):
        if parameter.value_type == float:
            parameter.set(parameter_value_slider2float(parameter, new_value))
        else:
            parameter.set(new_value)
        self.set_label_text(parameter, value_label, value_edit)

    def set_label_text(self, parameter, value_label, value_edit):
        if parameter.value_type == float:
            value_label.setText(f'{parameter.value:.3f}')
            value_edit.setText(f'{parameter.value:.3f}')
        else:
            value_label.setText(f'{parameter.value:d}')
            value_edit.setText(f'{parameter.value:d}')

