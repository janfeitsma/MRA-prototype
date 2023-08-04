# python modules
import struct
import json
import google.protobuf.timestamp_pb2
from google.protobuf import json_format

# our modules
from components.falcons.localization_vision.interface import Input_pb2, Params_pb2, State_pb2, Output_pb2, Local_pb2

# data elements
# mutable elements such as state get also a _before and _after variant, as stored in tick file
DATA_ELEMENTS = (
    ('input', False),
    ('params', False),
    ('state', True),
    ('output', False),
    ('local', False))
FILE_ELEMENTS = (
    'input',
    'params',
    'state_before',
    'output',
    'local',
    'state_after')



class Data():

    def __init__(self, filename = None):
        self.reset()
        if filename:
            self.loadTickFile(filename)

    def reset(self):
        self.t = google.protobuf.timestamp_pb2.Timestamp()
        for (key, mutable) in DATA_ELEMENTS:
            msg = eval('{0}_pb2.{0}()'.format(key.capitalize())) # example Input_pb2.Input()
            setattr(self, key, msg)
            if mutable:
                setattr(self, key+'_before', msg)
                setattr(self, key+'_after', msg)

    def setDefaultParams(self):
        self.readJsonFileIntoMessage('components/falcons/localization_vision/interface/DefaultParams.json', self.params)

    def readJsonFileIntoMessage(self, filename, msg):
        with open(filename, 'r') as file:
            json_data = json.load(file)
        json_format.ParseDict(json_data, msg)

    def loadTickFile(self, filename):
        # data is serialized, see MRA libraries/logging/logging.hpp dumpToFile
        # each protobuf object is an int (#bytes) followed by serialized protobuf bytes
        bytedata = None
        with open(filename, 'rb') as f:
            for key in FILE_ELEMENTS:
                # read next
                n = struct.unpack('i', f.read(4))[0]
                data = f.read(n)
                getattr(self, key).ParseFromString(data)


