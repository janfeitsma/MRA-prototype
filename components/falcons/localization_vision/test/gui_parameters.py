from collections import OrderedDict

class Param:
    def __init__(self, name, value_type, min_value=None, max_value=None, default_value=None, slider=True):
        self.name = name
        self.value_type = value_type
        if value_type is bool:
            min_value = False
            max_value = True
        self.min_value = min_value
        self.max_value = max_value
        self.default_value = default_value if default_value is not None else min_value
        self.value = self.default_value # current value is modifiable by sliders
        self.slider = slider
        self.notify = lambda n, v: None

    def set(self, value):
        # typecast - allow strings on input, from gui input text fields for instance
        value = self.value_type(value)
        # clip
        if value < self.min_value:
            value = self.min_value
        if value > self.max_value:
            value = self.max_value
        self.value = value
        self.notify(self.name, value)

    def __repr__(self):
        return f'{self.name:s}={self.value}'


class Parameters:
    def __init__(self):
        self.params = OrderedDict()

    def _construct(self, *args, **kwargs):
        return Param(*args, **kwargs)

    def add(self, *args, **kwargs):
        param = self._construct(*args, **kwargs)
        self.params[param.name] = param

    def get(self, name):
        return self.params[name].value

    def set(self, name, value):
        self.params[name].set(value)

    def __iter__(self):
        return iter(self.params.values())

