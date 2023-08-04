# python modules
import struct

# our modules
from components.falcons.localization_vision.interface import Input_pb2, Params_pb2, State_pb2, Output_pb2, Local_pb2


class TickData():

    def __init__(self, filename = None):
        if filename:
            self.load(filename)

    def load(self, filename):
        # data is serialized, see MRA libraries/logging/logging.hpp dumpToFile
        # each protobuf object is an int (#bytes) followed by serialized protobuf bytes
        # dumped data:
        #    input
        #    params
        #    state (before)
        #    output
        #    local
        #    state (after)
        data = None
        with open(filename, 'rb') as f:
            # read next
            n = struct.unpack('i', f.read(4))[0]
            data = f.read(n)
            self.input = Input_pb2.Input()
            self.input.ParseFromString(data)
            # read next
            n = struct.unpack('i', f.read(4))[0]
            data = f.read(n)
            self.params = Params_pb2.Params()
            self.params.ParseFromString(data)
            # read next
            n = struct.unpack('i', f.read(4))[0]
            data = f.read(n)
            self.state_before = State_pb2.State()
            self.state_before.ParseFromString(data)
            # read next
            n = struct.unpack('i', f.read(4))[0]
            data = f.read(n)
            self.output = Output_pb2.Output()
            self.output.ParseFromString(data)
            # read next
            n = struct.unpack('i', f.read(4))[0]
            data = f.read(n)
            self.local = Local_pb2.Local()
            self.local.ParseFromString(data)
            # read next
            n = struct.unpack('i', f.read(4))[0]
            data = f.read(n)
            self.state_after = State_pb2.State()
            self.state_after.ParseFromString(data)

