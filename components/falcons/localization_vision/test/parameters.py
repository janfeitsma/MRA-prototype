# python modules
import types
from google.protobuf.message import Message

# own modules
import gui


class ParametersProxy(gui.Parameters):
    """
    Connect sliders with protobuf Params object.
    """
    def __init__(self, params_proto, range_hints_proto):
        super().__init__()
        self.params_proto = params_proto
        self.range_hints_proto = range_hints_proto
        gui.create_standard_gui_params(self) # default zoom etc
        self._init_params(params_proto.solver) # only make solver configurable, not the field dimension model A,B,C etc

    def _init_params(self, proto, parent_name=''):
        for field, value in proto.ListFields():
            name = f'{parent_name}.{field.name}' if parent_name else field.name
            if isinstance(value, str):  # Handling string fields
                raise NotImplementedError('strings not yet supported in tuning')
                self.add(name, str, None, None, value)
            elif isinstance(value, int):  # Handling integer fields
                min_val, max_val = self.range_hints_proto[name]
                self.add(name, int, min_val, max_val, value)
            elif isinstance(value, float):  # Handling float fields
                min_val, max_val = self.range_hints_proto[name]
                self.add(name, float, min_val, max_val, value)
            elif isinstance(value, bool):  # Handling boolean fields
                self.add(name, bool, False, True, value)
            elif isinstance(value, Message):  # Handling nested protobuf messages
                self._init_params(value, name)

    def _construct(self, *args, **kwargs):
        result = gui.Param(*args, **kwargs)
        result.notify = self._update_protobuf
        return result

    def _update_protobuf(self, name, value):
        names = name.split('.')
        current_proto = self.params_proto.solver # only make solver configurable, not the field dimension model A,B,C etc
        for n in names[:-1]:
            current_proto = getattr(current_proto, n)
        try:
            setattr(current_proto, names[-1], value)
        except:
            pass # TODO

    def __repr__(self):
        return '\n'.join([repr(p) for p in self.params.values()])

