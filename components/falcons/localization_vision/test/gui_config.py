from PyQt5.QtCore import Qt
from PyQt5 import QtGui
from PyQt5.QtWidgets import *
import logging

SLIDER_RESOLUTION = 100



def parameter_value_float2slider(parameter):
    return int((parameter.value - parameter.min_value) * SLIDER_RESOLUTION / (parameter.max_value - parameter.min_value))

def parameter_value_slider2float(parameter, slider_int_value):
    return float(slider_int_value) / SLIDER_RESOLUTION * (parameter.max_value - parameter.min_value) + parameter.min_value


class CustomToggleButton(QPushButton):
    """
    Specialized button. Can be pressed, unpressed, remembers state and changes text/color.

    In state 'disabled', text1 is shown using style1.
    In state 'enabled', text2 is shown using style2.
    Default is disabled.
    """
    def __init__(self, text1, text2, style1=None, style2=None, enable=False):
        QPushButton.__init__(self, text1)
        self.texts = [text1, text2]
        self.styles = [style1, style2]
        self.setCheckable(True)
        self.clicked.connect(self.update_text_style)
        self.update_text_style(enable)
    def update_text_style(self, active):
        self.setText(self.texts[active])
        self.setStyleSheet(self.styles[active])


class ConfigurationWindow(QWidget):
    def __init__(self, parameters):
        super(ConfigurationWindow, self).__init__()
        self.setWindowTitle("Configuration")
        self.parameters = parameters
        self.setLayout(self.create_layout())
        self.setGeometry(100, 100, 500, 200)

    def setup_buttons(self, layout):
        # Create buttons
        self.reset_button = QPushButton("Reset")
        self.active_button = CustomToggleButton("Activate", "Stop", style1="background-color : green", style2="background-color : red")
        self.overlay_button = CustomToggleButton("Show overlay", "Hide overlay")
        # Connect signals/slots
        self.reset_button.clicked.connect(self.reset_button_clicked)
        self.active_button.clicked.connect(self.active_button_clicked)
        self.overlay_button.clicked.connect(self.overlay_button_clicked)
        # Add buttons to the layout
        layout.addWidget(self.reset_button)
        layout.addWidget(self.active_button)
        layout.addWidget(self.overlay_button)

    def create_layout(self):
        # Create layouts
        main_layout = QVBoxLayout()
        button_layout = QHBoxLayout()
        grid_layout = QGridLayout()
        # Set up buttons
        self.setup_buttons(button_layout)
        # Fill grid layout, configure sliders
        self.add_row_to_layout(grid_layout, self.create_header_row())
        for p in self.parameters:
            row = self.create_parameter_row(p)
            self.add_row_to_layout(grid_layout, row)
        grid_layout.setColumnStretch(0, 0)  # Fix the first column width
        grid_layout.setColumnStretch(1, 0)  # Fix the second column width
        grid_layout.setColumnStretch(2, 1)  # Allow the slider column to stretch
        grid_layout.setColumnStretch(3, 0)  # Fix the fourth column width
        grid_layout.setColumnStretch(4, 0)  # Fix the fifth column width
        # Add button layout and grid layout to the main layout
        main_layout.addLayout(button_layout)
        main_layout.addLayout(grid_layout)
        return main_layout

    def create_header_row(self):
        result = [
            QLabel('parameter'),
            QLabel('minval'),
            QLabel('slider'),
            QLabel('curval'),
            QLabel('maxval'),
        ]
        # make bold
        font = result[0].font()
        font.setBold(True)
        for e in result:
            e.setFont(font)
        return result

    def create_parameter_row(self, parameter):
        name_label = QLabel(parameter.name)
        min_label = QLabel(f'{parameter.min_value:4g}')
        slider = QSlider(Qt.Horizontal)
        value_label = QLabel()
        value_label.setAlignment(Qt.AlignRight)
        value_label.setMinimumWidth(60)
        max_label = QLabel(f'{parameter.max_value:4g}')

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
        self.parameters.set(parameter.name, value_edit.text())
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
            new_value = parameter_value_slider2float(parameter, new_value)
        self.parameters.set(parameter.name, new_value)
        self.set_label_text(parameter, value_label, value_edit)

    def set_label_text(self, parameter, value_label, value_edit):
        if parameter.value_type == float:
            value_label.setText(f'{parameter.value:.3f}')
            value_edit.setText(f'{parameter.value:.3f}')
        else:
            value_label.setText(f'{parameter.value:d}')
            value_edit.setText(f'{parameter.value:d}')

    def reset_button_clicked(self, e):
        logging.info(e)

    def active_button_clicked(self, e):
        logging.info(e)

    def overlay_button_clicked(self, e):
        logging.info(e)

