from collections import OrderedDict

class Param:
    def __init__(self, name, value_type, min_value, max_value, default_value=None):
        self.name = name
        self.value_type = value_type
        self.min_value = min_value
        self.max_value = max_value
        self.default_value = default_value if default_value is not None else min_value
        self.value = self.default_value # current value is modifiable by sliders

    def set(self, value):
        assert value >= self.min_value
        assert value <= self.max_value
        self.value = value


class Parameters:
    def __init__(self):
        self.params = OrderedDict()

    def add(self, *args, **kwargs):
        param = Param(*args, **kwargs)
        self.params[param.name] = param

    def get(self, name):
        return self.params[name].value

    def set(self, name, value):
        self.params[name].set(value)

    def __iter__(self):
        return iter(self.params.values())

