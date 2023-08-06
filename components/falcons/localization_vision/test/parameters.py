# python modules
from google.protobuf.message import Message

# own modules
import gui


class ParametersProxy:
    """
    Connect sliders with protobuf Params object.
    This class behaves as gui.Parameters, such that the GUI (sliders etc) remains clean.
    """
    # choose composition over inheritance

    def __init__(self, params_proto, range_hints_proto):
        self.params_proto = params_proto
        self.range_hints_proto = range_hints_proto
        self.gui_params = gui.create_standard_gui_params() # system parameters from GUI (zoom, frequency etc)
        self.pb_params = gui.Parameters() # component-specific parameters from protobuf
        self._init_params(params_proto)

    def _init_params(self, proto, parent_name=''):
        for field, value in proto.ListFields():
            name = f'{parent_name}.{field.name}' if parent_name else field.name
            if isinstance(value, str):  # Handling string fields
                raise NotImplementedError('strings not yet supported in tuning')
                # self.add(name, str, None, None, value)
            elif isinstance(value, int):  # Handling integer fields
                min_val, max_val = self.range_hints_proto[name]
                self.pb_params.add(name, int, min_val, max_val, value)
            elif isinstance(value, float):  # Handling float fields
                min_val, max_val = self.range_hints_proto[name]
                self.pb_params.add(name, float, min_val, max_val, value)
            elif isinstance(value, bool):  # Handling boolean fields
                self.pb_params.add(name, bool, False, True, value)
            elif isinstance(value, Message):  # Handling nested protobuf messages
                self._init_params(value, name)

    def get(self, name):
        if name in self.gui_params.params:
            return self.gui_params.get(name)
        return self.pb_params.get(name)

    def set(self, name, value):
        if name in self.gui_params.params:
            self.gui_params.set(name, value)
        else:
            self.pb_params.set(name, value)
            self._update_protobuf(name, value)

    def __iter__(self):
        yield from self.gui_params
        yield from self.pb_params

    def __repr__(self):
        gui_repr = '\n'.join([repr(p) for p in self.gui_params])
        pb_repr = '\n'.join([repr(p) for p in self.pb_params])
        return f"{gui_repr}\n{pb_repr}"

    def _update_protobuf(self, name, value):
        names = name.split('.')
        current_proto = self.params_proto
        for n in names[:-1]:
            current_proto = getattr(current_proto, n)
        setattr(current_proto, names[-1], value)

